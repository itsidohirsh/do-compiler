@echo off

cd ..

@REM Check if main.exe exists
@REM If not, exiting with exit code 1
if not exist main.exe (
    echo Couldn't file main.exe
    exit 1
)

@REM Check if the source code file exists
@REM If not, exiting with exit code 1
if not exist examples/example.do (
    echo Couldn't find source code file
    exit 1
)

@REM Running the program and exiting with exit code 0
main.exe examples/example.do
exit 0
