# A script to count how many lines of code there are in the project #

Write-Host "# of code lines in the project: " -NoNewline
Write-Host (Get-ChildItem .. -Recurse -File | Get-Content).Length -ForegroundColor Green
