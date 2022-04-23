@echo off
cls

:: Add the bin directory path to the user's PATH environment variable
cd scripts
powershell -ExecutionPolicy Bypass -File .\prepend_user_path.ps1
cd ..

:: Add the bin directory path to the current session PATH
set PATH=%~dp0bin;%PATH%
