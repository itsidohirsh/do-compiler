# Script to prepend the path to the bin directory to the PATH environment variable #

# Get the path to the bin directory of the project
$bin_path = (Split-Path -Path $(Get-Location) -Parent) + "\bin"

# Get the user's PATH environment variable from the registry
$user_path = (Get-ItemProperty -path HKCU:\Environment\ -Name Path).Path

# Set the user's PATH to be the directory to the bin directory + the previous path
Set-ItemProperty -path HKCU:\Environment\ -Name Path -Value "$bin_path;$user_path"
