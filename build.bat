@echo off

@REM Go into the scripts directory
cd scripts

@REM Build the do compiler
powershell .\build.ps1

@REM Returns to the previous directory
cd ..
