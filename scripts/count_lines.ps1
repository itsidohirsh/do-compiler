Set-Location ..

Write-Host "# of code lines in the project: " -NoNewline
Write-Host (Get-ChildItem -Recurse -File | Get-Content).Length -ForegroundColor Green
