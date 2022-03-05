# Script to run the executable of the project #

# Go up a directory
Set-Location .. 

# Check if file exists, if so runs
if ((Test-Path -Path .\main.exe) -and (Test-Path -Path examples/example.do)) {
    .\main.exe examples/example.do
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find the executable or example file" -ForegroundColor red
}

# Return to the scripts directory
Set-Location scripts 
