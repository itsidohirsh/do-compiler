@echo off

cd C:\Ido_Hirsh\Projects\Do

if exist src/*.c (
    gcc main.c src/*.c -o main
) else (
    gcc main.c -o main
)

if exist main.exe (
    main.exe
)
