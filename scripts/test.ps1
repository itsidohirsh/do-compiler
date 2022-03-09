# Script to test the code. All the tests are from the ../tests folder. A test is passed if the exit code of it is NOT 0 #

# Build
& .\build.ps1

# Test
$passed_tests = @()
$failed_tests = @()

# Run every test in the tests folder
foreach ($file_path in $(Get-ChildItem ..\tests -Include '*.do' -Recurse | Resolve-Path -Relative)) {
    # Run test without it's output
    .\run.ps1 $file_path | Out-Null

    # If passed test, add to $passed_tests array
    # -ne 0 because the test is checking for error catching, and the error code will not be 0 in that casetests
    if ($LASTEXITCODE -ne 0) {
        $passed_tests += $file_path
    }
    # If passed test, add to $failed_tests array
    else {
        $failed_tests += $file_path
    }
}

# Output tests results
# Prints passed tests
foreach ($file_path in $passed_tests) {
    Write-Host "V " -ForegroundColor green -NoNewline
    Write-Host $file_path -NoNewline
    Write-Host " passed" -ForegroundColor green
}
# Prints failed tests
foreach ($file_path in $failed_tests) {
    Write-Host "X " -ForegroundColor red -NoNewline
    Write-Host $file_path -NoNewline
    Write-Host " failed" -ForegroundColor red
}
# Print summary results
Write-Host
Write-Host $passed_tests.length -ForegroundColor green -NoNewline
Write-Host " / " -NoNewline
Write-Host $($passed_tests.length + $failed_tests.length) -ForegroundColor yellow -NoNewline
Write-Host " tests passed successfuly"

# Clean
& .\clean.ps1
