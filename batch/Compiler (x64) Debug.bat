@echo off
echo Compile Start

rem Variable
setlocal

set FILES=main.cpp control.cpp menu.cpp log.cpp list.cpp util.cpp process.cpp vk.cpp debug.cpp

rem Path
if not exist ../bin mkdir ..\bin
cd ../src

rem Resource Compile
windres resource.rc -O coff ../bin/resource.res -I ../Include

rem Executable Compile
x86_64-w64-mingw32-g++ -I../include %FILES% -o ../bin/Shortcut.exe ../bin/resource.res -static-libgcc -static-libstdc++ -mwindows -mconsole -D_DEBUG

pause