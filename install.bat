@echo off

@REM Go into the scripts directory
cd scripts

@REM Build the do compiler using the powershell's build script
powershell .\build.ps1

@REM Returns to the previous directory
cd ..

@REM Add the path to the bin directory to the PATH environment variable
setx PATH "%~dp0bin;%PATH%"
set PATH=%~dp0bin;%PATH%
