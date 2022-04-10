# Do Compiler
The Do Programming Language Compiler for Windows 💡🧐

My programming language that helped me learn about compilers and language design.


## Setup
In the project's directory you can find the `do.exe` executable, this is the Do Programming Language Compiler for Windows!

### Use it anywhere
To make the do compiler globally available, add the path to the folder it is located in to the `PATH` environment variable.

### Compile the compiler
1. Make sure you have a C compiler installed.
    - I used `gcc (x86_64-win32-seh-rev0, Built by MinGW-W64 project) 8.1.0` while building the compiler.
3. To build the do compiler simply run the `build.bat` script that is located in the project's directory, this should give you the `do.exe` executable.


## Use
Now I'll explain how to use the do compiler to compile `do` code to `x64 MASM assembly` code to a `.exe` executable.

### Compile Do code
Run `do /path/to/source.do [/path/to/destination.asm]` in the `cmd` to compile your do code into `x64 MASM assembly` code.
- Run `do` without any arguments to see how to use the command.

### Compile x64 MASM assembly code
1. Add the path to the `masm64` directory to the `PATH` environment variable, it contains all the files needed to compile the `x64 MASM assembly` code.
2. Run `ml64 filename.asm /link /subsystem:windows /entry:main` in the `cmd` to compile the `.asm` file to a `.exe` executable.

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
