pushd %~dp0

GenProcs.exe --path=../../TrapperGameServer/GameDB.xml --output=GenProcedures.h

IF ERRORLEVEL 1 PAUSE

XCOPY /Y GenProcedures.h "../../TrapperGameServer"

DEL /Q /F *.h

PAUSE