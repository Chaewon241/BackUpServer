pushd %~dp0
pyinstaller --onefile DBToCppGenerator.py
MOVE .\dist\DBToCppGenerator.exe .\DBToCppGenerator.exe
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\DBToCppGenerator.spec
PAUSE