# Script to delete the executable of the project #

Clear-Host

# Delete the executable and .asm files
if (Test-Path -Path ..\do.exe) {
    Remove-Item ..\do.exe
    Remove-Item ..\*.asm
}
