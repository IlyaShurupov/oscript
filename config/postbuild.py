import sys
import os
from pathlib import Path

SolutionDir = Path(sys.argv[1])
OutputDir = Path(sys.argv[2])


grammar_path = str(Path(str(SolutionDir) + "\\rsc\\oscript.grammar").absolute())
cmd = "xcopy " + grammar_path + " " + str(OutputDir) + "\\ /Y /H /E /F"

os.system(cmd)
