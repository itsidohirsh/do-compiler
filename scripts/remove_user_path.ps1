# Script to remove the path to the bin directory from the PATH environment variable #

# Get the path to the bin directory of the project
$bin_path = (Split-Path -Path $(Get-Location) -Parent) + "\bin"

# Get the user's PATH environment variable from the registry
$user_path = (Get-ItemProperty -path HKCU:\Environment\ -Name Path).Path

# Get the user's PATH without the bin directory's path
$user_path = ($user_path.Split(';') | Where-Object { $_ -ne $bin_path }) -join ';'

# Set the user's PATH to be the previous path - the directory to the bin directory
Set-ItemProperty -path HKCU:\Environment\ -Name Path -Value $user_path
