import os
import os.path
import glob

from lxml import etree

class ExternalToolConfiguration:

	@classmethod
	def SearchProjects(self):
		for projectPath in glob.glob("**/*.vcxproj", recursive=True):
			self.PatchProject(projectPath)
		return

	@classmethod
	def PatchProject(self, projectPath):
		if projectPath.startswith("Vendors"):
			return

		requestedProjFile = projectPath

		root = etree.parse(requestedProjFile)
		namespaces = {'ns':'http://schemas.microsoft.com/developer/msbuild/2003'}
				
		# Add tools.props in project if exists
		projectDir = os.path.dirname(projectPath)
		toolsPropsPath = os.path.join(projectDir, "tools.props")
		if os.path.isfile(toolsPropsPath):
			project = root.xpath("//ns:Project", namespaces=namespaces)[0]
			importProps = etree.SubElement(project, "Import")
			importProps.set("Project", "tools.props")

		with open(requestedProjFile, "wb") as f:
			root.write(f, encoding="utf-8")
		return
	
	@classmethod
	def Setup(self):
		self.SearchProjects()
		return

if __name__ == "__main__":
	print("Setup external tool paths in projects...")
	ExternalToolConfiguration.Setup()
