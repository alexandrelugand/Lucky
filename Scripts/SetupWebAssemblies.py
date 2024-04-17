# import os
# import os.path
import re
import glob

#from sys import argv
from lxml import etree

class WebAssembliesConfiguration:

	@classmethod
	def SearchProjects(self):
		for projectPath in glob.glob("**/*.Web.vcxproj", recursive=True):
			self.PatchProject(projectPath)
			self.PathSolution(projectPath)

		return

	@classmethod
	def PatchProject(self, projectPath):
		requestedProjFile = projectPath

		f = open(requestedProjFile, "r")
		oldBuffer = str(f.read())
		f.close()

		# replace x64 by Emscripten target
		regex = f"x64"
		pattern = re.compile(regex, re.DOTALL)
		newBuffer = re.sub(pattern, "Emscripten", oldBuffer) 

		f = open(requestedProjFile, "w")
		f.write(newBuffer)
		f.close()

		root = etree.parse(requestedProjFile)
		namespaces = {'ns':'http://schemas.microsoft.com/developer/msbuild/2003'}

		# Clean <PropertyGroup Label="Globals"> node
		globals = root.xpath("//ns:PropertyGroup[@Label='Globals']", namespaces=namespaces)[0]

		for bad in globals.xpath("//ns:IgnoreWarnCompileDuplicatedFilename", namespaces=namespaces):
			bad.getparent().remove(bad)
			
		for bad in globals.xpath("//ns:Keyword", namespaces=namespaces):
			bad.getparent().remove(bad)
			
		# Patch <PropertyGroup Label="Configuration"> nodes
		for configuration in root.xpath("//ns:PropertyGroup[@Label='Configuration']", namespaces=namespaces):
			configuration.xpath("ns:PlatformToolset", namespaces=namespaces)[0].text = "emcc"
			emscriptenDir = etree.SubElement(configuration, "EmscriptenDir")
			emscriptenDir.text = "$(EMSDK)\\upstream\\emscripten\\"
			for bad in configuration.xpath("ns:UseDebugLibraries", namespaces=namespaces):
				bad.getparent().remove(bad)
			for bad in configuration.xpath("ns:CharacterSet", namespaces=namespaces):
				bad.getparent().remove(bad)

		for bad in root.xpath("//ns:ImportGroup[@Label='ExtensionSettings']", namespaces=namespaces):
			bad.getparent().remove(bad)
			
		for bad in root.xpath("//ns:ImportGroup[@Label='PropertySheets']", namespaces=namespaces):
			bad.getparent().remove(bad)
			
		for bad in root.xpath("//ns:ImportGroup[@Label='UserMacros']", namespaces=namespaces):
			bad.getparent().remove(bad)
			
		with open(requestedProjFile, "wb") as f:
			root.write(f, encoding="utf-8")
		return
	
	@classmethod
	def PathSolution(self, projectPath):
		solutionPath = glob.glob("*.sln")[0]
		f = open(solutionPath, "r")
		buffer = str(f.read())
		f.close()

		# search project GUID
		projectIndex = buffer.index(projectPath)
		offset = projectIndex+len(projectPath)+5
		projectGuid = buffer[offset:offset+36]

		offset = buffer.find(projectGuid, offset+1)
		while offset != -1:
			x64Offset = buffer.find("x64", offset+1)
			if x64Offset != -1:
				lineToPatch = buffer[offset:x64Offset+3]
				if lineToPatch.startswith(projectGuid+"}."):
					pattern = re.compile(re.escape(lineToPatch), re.DOTALL)
					buffer = re.sub(pattern, lineToPatch.replace("x64", "Emscripten"), buffer) 
			offset = buffer.find(projectGuid, offset+1)
			
		f = open(solutionPath, "w")
		f.write(buffer)
		f.close()

	@classmethod
	def Setup(self):
		self.SearchProjects()
		return

if __name__ == "__main__":
	print("Generate projects and solution for WebAssembly configuration...")
	WebAssembliesConfiguration.Setup()
