@echo off

:: Go into the scripts directory
cd scripts

:: Add the bin directory path to the user's PATH environment variable
powershell .\prepend_user_path.ps1

:: Return to the project directory
cd ..

:: Clear the screen
cls
