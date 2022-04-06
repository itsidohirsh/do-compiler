# Script to compile all the source files of the project #

Clear-Host

# Check if files exist, if so compiles
if (Test-Path -Path $(Get-ChildItem ..\src -Recurse -Include *.c)) {
    gcc $(Get-ChildItem ..\src -Recurse -Include *.c) -o ..\do.exe
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find source files" -ForegroundColor red
}
