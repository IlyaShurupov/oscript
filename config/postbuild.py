import sys
import os
from pathlib import Path

SolutionDir = Path(sys.argv[1])
OutputDir = Path(sys.argv[2])


grammar_path = str(Path(str(SolutionDir) + "\\rsc\\oscript.grammar").absolute())
source_path = str(Path(str(SolutionDir) + "\\rsc\\example.os").absolute())
cmd1 = "xcopy " + grammar_path + " " + str(OutputDir) + "\\ /Y /H /E /F"
cmd2 = "xcopy " + source_path + " " + str(OutputDir) + "\\ /Y /H /E /F"

print("installing resources")

os.system(cmd1)
os.system(cmd2)
