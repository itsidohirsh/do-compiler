# Script to delete the executable of the project #

Clear-Host

# Delete the executable
if (Test-Path -Path ..\main.exe) {
    Remove-Item ..\main.exe
}
