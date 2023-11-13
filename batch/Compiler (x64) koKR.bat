@echo off
echo Compile Start

rem Variable
setlocal

set FILES=main.cpp control.cpp menu.cpp log.cpp util.cpp process.cpp vk.cpp

rem Path
if not exist ../bin mkdir ..\bin
cd ../src

rem Resource Compile
windres resource.rc -O coff ../bin/resource.res -I ../Include --define LANG=KOKR --define UNICODE

rem Executable Compile
x86_64-w64-mingw32-g++ -I../include %FILES% -o ../bin/Shortcut.exe ../bin/resource.res -mwindows -Wno-int-to-pointer-cast -Wno-write-strings -DLANG=KOKR

pause