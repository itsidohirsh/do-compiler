@echo off

cd ..

@REM Check if main.c exists
@REM If not, exiting with exit code 1
if not exist main.c (
    echo Couldn't find main.c
    exit 1
)

@REM Check if all the source files exists
@REM If not, exiting with exit code 1
if not exist src/*.c (
    echo Couldn't find source files
    exit 1
)

@REM Compiling and exiting with exit code 0
gcc main.c src/*.c -o main
exit 0
