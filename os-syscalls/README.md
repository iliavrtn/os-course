# Linux Shell with Extended Functionality

## Overview

This project is an implementation of a custom shell in C for the Linux operating system. 
The shell not only supports all the standard commands available in a typical shell but also
introduces additional commands to enhance its capabilities. The focus of this implementation
is on utilizing fundamental Linux operating system calls for file-related operations,
specifically using `open`, `read`, and `write`. Additionally, the creation of new processes
is achieved through the use of `fork` and `exec`.

## Features

- **Standard Shell Commands:** The shell supports all standard commands available in a regular shell,
-  allowing users to execute common tasks.

- **Extended Commands:** Introduces additional commands that go beyond the functionality of a typical shell,
-  providing users with enhanced capabilities.

- **File Operations:** For commands related to the content of files, the implementation exclusively relies on
-  the Linux operating system calls `open`, `read`, and `write`.

- **Process Management:** Creation of new processes is handled using the `fork` system call, and command
-  execution within these processes is achieved through the `exec` system call.
