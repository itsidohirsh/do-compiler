@echo off

cd ..

if exist src/*.c (
    gcc main.c src/*.c -o main
) else (
    gcc main.c -o main
)

if exist main.exe if exist examples/example.do (
    main.exe examples/example.do
    del main.exe
)
