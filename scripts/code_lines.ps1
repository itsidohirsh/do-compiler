# A script to count how many lines of code there are in the project #

Clear-Host

Write-Host "# of code lines in the project: " -NoNewline
Write-Host (Get-ChildItem .. -Include ('*.c', '*.h', '*.ps1', '*.bat', '*.do', '*.md', '*.gitignore', '*.json') -Recurse | Get-Content).Length -ForegroundColor Green
