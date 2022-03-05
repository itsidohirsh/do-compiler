# Script to delete the executable of the project #

# Go up a directory
Set-Location .. 

# Delete the executable
if (Test-Path -Path .\main.exe) {
    Remove-Item .\main.exe
}

# Return to the scripts directory
Set-Location scripts 
