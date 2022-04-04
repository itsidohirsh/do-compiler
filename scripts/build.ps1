# Script to compile all the source files of the project #

Clear-Host

# Check if files exist, if so compiles
if ((Test-Path -Path ..\main.c) -and (Test-Path -Path $(Get-ChildItem ..\src -Recurse -Include *.c))) {
    gcc ..\main.c $(Get-ChildItem ..\src -Recurse -Include *.c) -o ..\do.exe
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find source files" -ForegroundColor red
}
