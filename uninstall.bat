@echo off

:: Remove the bin directory path from the user's PATH environment variable
cd scripts
powershell .\remove_user_path.ps1
cd ..

:: Delete the project
cd ..
cls
rmdir /s /q %~dp0
