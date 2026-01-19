*This project has been created as part of the 42 curriculum by tafonso and dluis-ma*

# minishell

## Description

**minishell** is a simplified implementation of a Unix shell, developed as part of the 42 curriculum.  
The goal of this project is to gain a deeper understanding of how shells work internally, including process creation, command parsing, environment management, redirections, and signal handling.

This shell replicates a subset of the behavior of **bash**, allowing the user to execute commands, handle pipes and redirections, manage environment variables, and interact with the system through a command-line interface.

The project emphasizes low-level programming concepts such as:
- Process control (`fork`, `execve`, `wait`)
- File descriptors and redirections
- Signals
- Memory management
- Parsing and lexical analysis

---

## Features

- Display a prompt and wait for user input
- Command execution using absolute or relative paths
- Built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Environment variable expansion (`$VAR`)
- Exit status expansion (`$?`)
- Pipes (`|`)
- Redirections:
  - Input (`<`)
  - Output (`>`)
  - Append (`>>`)
  - Heredoc (`<<`)
- Signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`) similar to bash
- Proper error handling and memory management

---

## Instructions

### Requirements

- A Unix-like operating system (Linux or macOS)
- `cc`
- `make`
- `readline` library

### Compilation

To compile the project, run:

```bash
make

This will generate the executable:

./minishell

### Execution

Launch minishell by running:

./minishell


You can then start typing commands as you would in a regular shell.

To clean object files:

make clean


To remove all compiled files:

make fclean


To recompile everything:

make re

## Usage Examples
$ echo Hello World
Hello World

$ ls | grep minishell
minishell.c

$ export NAME=42
$ echo $NAME
42

$ cat << EOF
> hello
> minishell
> EOF
hello
minishell

Technical Choices

The project strictly follows the constraints of the 42 subject.

Parsing is separated from execution to improve readability and maintainability.

Built-in commands are handled without forking when required.

External commands are executed using fork and execve.

Signals are handled differently in the parent and child processes to match bash behavior.

Memory is carefully managed to avoid leaks and undefined behavior.

## Resources
### Documentation & References

Bash manual: https://www.gnu.org/software/bash/manual/

GNU C Library (glibc): https://www.gnu.org/software/libc/manual/

Linux man pages:

man bash

man execve

man fork

man pipe

man dup2

man signal

Advanced Programming in the UNIX Environment – W. Richard Stevens

AI Usage

AI tools were used as a support resource during the development of this project, primarily for:

Clarifying the behavior of system calls and signals

Understanding shell-related concepts (pipes, redirections, heredocs)

Improving code structure and readability

Debugging logic errors and edge cases

All design decisions, implementation, and final code were written and validated by the project authors, in compliance with the 42 rules regarding academic integrity.
