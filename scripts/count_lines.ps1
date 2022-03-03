Set-Location ..

Write-Host "# of code lines in the project: " -NoNewline
Write-Host (Get-Content $(git ls-files)).Length -ForegroundColor Green
