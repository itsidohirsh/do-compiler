@echo off
cls

:: Remove the bin directory path from the user's PATH environment variable
cd scripts
powershell -ExecutionPolicy Bypass -File .\remove_user_path.ps1
cd ..

:: Delete the project
cd ..
goto 2>nul & rmdir /s /q %~dp0
