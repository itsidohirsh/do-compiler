# Script to compile all the source files of the project #

Clear-Host

# Get all the source files
$source_files = Get-ChildItem ..\src -Recurse -Include *.c

# Check if source files exist, if so compile
if (Test-Path -Path $source_files) {
    gcc $source_files -o ..\bin\do.exe
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find source files" -ForegroundColor red
}
