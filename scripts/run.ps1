# Script to run the executable of the project with a single source file argument #

# Check if cli arguments exist
if ($args.length -gt 0) {
    $source_file = $args[0]
}
# If not, print out error, return to the scripts directory and exit
else {
    Write-Host "Please specify source file." -ForegroundColor red
    exit
}

# Check if file exists, if so runs
if ((Test-Path -Path ..\main.exe) -and (Test-Path -Path $source_file)) {
    # Return the return value from the executable
    ..\main.exe $source_file
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find the executable or source file" -ForegroundColor red
}
