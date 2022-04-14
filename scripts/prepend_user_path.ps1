# Script to prepend the path to the bin directory to the PATH environment variable #

# Get the path to the directory of the project
$project_path = Split-Path -Path $(Get-Location) -Parent

# Get the current user PATH from the registry
$user_path = $((Get-ItemProperty -path HKCU:\Environment\ -Name Path).Path)

# Set the user to be the directory to the bin directory + the previous path
Set-ItemProperty -path HKCU:\Environment\ -Name Path -Value "$project_path\bin;$user_path"
