pushd %~dp0

DBToCppGenerator.exe --path=../../TrapperGameServer/GameDB.xml --output=DBManager.h

IF ERRORLEVEL 1 PAUSE

XCOPY /Y DBManager.h "../../TrapperGameServer"

DEL /Q /F *.h

PAUSE