pushd %~dp0
protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto


GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=C_ --send=S_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=S_ --send=C_

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../../../TrapperGameServer"
XCOPY /Y Enum.pb.cc "../../../TrapperGameServer"
XCOPY /Y Struct.pb.h "../../../TrapperGameServer"
XCOPY /Y Struct.pb.cc "../../../TrapperGameServer"
XCOPY /Y Protocol.pb.h "../../../TrapperGameServer"
XCOPY /Y Protocol.pb.cc "../../../TrapperGameServer"
XCOPY /Y ClientPacketHandler.h "../../../TrapperGameServer"

XCOPY /Y Enum.pb.h "../../../DummyClient"
XCOPY /Y Enum.pb.cc "../../../DummyClient"
XCOPY /Y Struct.pb.h "../../../DummyClient"
XCOPY /Y Struct.pb.cc "../../../DummyClient"
XCOPY /Y Protocol.pb.h "../../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../../DummyClient"
XCOPY /Y ServerPacketHandler.h "../../../DummyClient"

XCOPY /Y Enum.pb.h "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y Enum.pb.cc "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y Struct.pb.h "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y Struct.pb.cc "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y Protocol.pb.h "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y Protocol.pb.cc "../../../../TrapperProject/Source/TrapperProject/Network"
XCOPY /Y ServerPacketHandler.h "../../../../TrapperProject/Source/TrapperProject"


DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE