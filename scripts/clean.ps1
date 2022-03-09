# Script to delete the executable of the project #

# Delete the executable
if (Test-Path -Path ..\main.exe) {
    Remove-Item ..\main.exe
}
