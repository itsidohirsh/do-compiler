# Do Compiler
The Do Programming Language Compiler for Windows 💡🧐

My programming language that helped me learn about compilers and language design.


## Setup
Follow this steps to setup the `Do Programming Language Compiler for Windows` correctly on your machine.

### Use it anywhere
Add the path to the `bin` directory to the `PATH` environment variable. It contains the `Do Programming Language Compiler for Windows` as well as all the files needed to compile the `x64 MASM assembly`.

### Compile the compiler
1. Make sure you have `C` compiler installed.
    - I used `gcc (x86_64-win32-seh-rev0, Built by MinGW-W64 project) 8.1.0` while building the compiler.
2. To build the do compiler simply run the `build.bat` script that is located in the project's directory. The `do.exe` executable will appear in the `bin` directory.


## Use
Now I'll explain how to use the do compiler to compile `do` code to `x64 MASM assembly` code to a `.exe` executable.

### Compile Do code
Run `do /path/to/source.do [/path/to/destination.asm]` in the `cmd` to compile your do code into `x64 MASM assembly` code.
- Run `do` without any arguments to see how to use the command.

### Compile x64 MASM assembly code
Run `ml64 filename.asm /link /subsystem:windows /entry:main` in the `cmd` to compile the `.asm` file to a `.exe` executable.

### Example
The following is an example do code that you can use to try out the do compiler.

```c
prog my_prog:
	char ch;
        set ch = 'A';

	int i;
	set i = 0;
	while (i < 10):
		set ch = ch + i;
		set i = i + 1;
	done
done :)
```
