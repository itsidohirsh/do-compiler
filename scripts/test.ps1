# Script to test the code. All the tests are from the ../tests folder. A test is passed if the exit code of it is NOT 0 #

# Build
& .\build.ps1

# Test

# Check if cli arguments exist and the path specified exists
if (($args.length -gt 0) -and (Test-Path -Path $args[0])) {
    # Set the test path to the path specified
    $test_path = $args[0]
}
# If either is false, set ..\tests as the test path
else {
    $test_path = "..\tests"
}

$passed_tests = @()
$failed_tests = @()

# Run every test in the tests folder
foreach ($file_path in $(Get-ChildItem $test_path -Include '*.do' -Recurse | Resolve-Path -Relative)) {
    # Run test
    Write-Host
    $file_path
    ..\bin\do.exe $file_path ..\example.asm

    # If passed test, add to $passed_tests array
    # -ne 0 because the test is checking for error catching, and the error code will not be 0 in that casetests
    # -or the path starts with V because a test that starts with V should return exit code 0
    if (($LASTEXITCODE -ne 0) -or $(Split-Path $file_path -Leaf).StartsWith('V')) {
        $passed_tests += Split-Path $file_path -Leaf
    }
    # If passed test, add to $failed_tests array
    else {
        $failed_tests += Split-Path $file_path -Leaf
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
