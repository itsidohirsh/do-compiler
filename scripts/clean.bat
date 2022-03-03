@echo off

cd ..

@REM Deleting main.exe if exists
if exist main.exe (
    del main.exe
)

exit 0
