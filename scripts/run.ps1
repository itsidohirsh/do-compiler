# Script to run the executable of the project with a single source file argument #

# Check if cli arguments exist
if ($args.length -gt 1) {
    $source_file = $args[0]
    $dest_file = $args[1]
}
# If not, print out error, return to the scripts directory and exit
else {
    Write-Host "Please specify source and destination files." -ForegroundColor red
    exit
}

# Check if file exists, if so runs
if ((Test-Path -Path ..\bin\do.exe) -and (Test-Path -Path $source_file)) {
    # Return the return value from the executable
    ..\bin\do.exe $source_file $dest_file
}
# If does not exist, print error message
else {
    Write-Host "Couldn't find the executable or source file" -ForegroundColor red
}
