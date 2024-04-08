from sys import argv
import os
import re

requestedCSFile = str(argv[1])
requestedClassName = os.path.splitext(os.path.basename(requestedCSFile))[0]

f = open(requestedCSFile, "r")
oldBuffer = str(f.read())
f.close()

regex = f"class {requestedClassName}.*static {requestedClassName}"
pattern = re.compile(regex, re.DOTALL)
newBuffer = re.sub(pattern, "class " + requestedClassName + " {\nstatic " + requestedClassName, oldBuffer) # remove SWIG autogenerated code between

if newBuffer != oldBuffer:
  f = open(requestedCSFile, "w")
  f.write(newBuffer)
  f.close()