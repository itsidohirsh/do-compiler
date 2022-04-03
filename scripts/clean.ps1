# Script to delete the executable of the project #

Clear-Host

# Delete the executable and .asm files
if (Test-Path -Path ..\main.exe) {
    Remove-Item ..\main.exe
    Remove-Item ..\*.asm
}
