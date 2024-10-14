# RISC-V Emulator Project

## Overview

Welcome to the RISC-V Emulator project! This project allows you to simulate the execution of RISC-V instructions, complete with an assembler that converts RISC-V assembly code into machine code and an emulator that runs that code. To add a bit more realism, it also features a basic Virtual File System (VFS) to handle simple file I/O operations.

## Components

1. **Assembler**: Turns RISC-V assembly code into binary machine code.
2. **Emulator**: Simulates a RISC-V processor by executing the machine code.
3. **Virtual File System (VFS)**: Supports basic file read/write operations within the emulated environment.

## Features

- Supports the RV32I base integer instruction set.
- Simulates a basic in-memory Virtual File System for file operations.
- Provides detailed output of instruction execution and register states.
- Supports basic system calls for handling file input/output

## What I've Learned

Throughout this project, I've gained valuable insights and skills:

1. **RISC-V Architecture**: I’ve gained a solid understanding of how the RISC-V instruction set and CPU architecture work.
2. **Emulation Techniques**: I’ve learned how to create a software-based emulator that mimics hardware behavior.
3. **Assembly and Machine Code**: I now have a clearer understanding of how assembly language maps to machine code.
4. **System Calls**: Implementing basic system calls helped me understand the interaction between user code and operating system-level functionality.OS-level operations.
5. **Virtual File Systems**: I got to explore how file systems work, albeit at a simple level.
6. **C Programming**: My skills in C, especially with bitwise operations and memory handling, improved significantly.
7. **Software Architecture**: I’ve learned to architect and manage a project that has multiple interconnected components.

## How to Use

1. **Assembling RISC-V Code**:

```bash
./assembler input.s output.bin
```
This will convert your RISC-V assembly code (`input.s`) into a binary file (`output.bin`).

2. **Running the Emulator**:
```bash
./emulator output.bin
```

This will start the emulator and execute the machine code from `output.bin`

## Project Structure

- `assembler.c`: Contains the assembler logic that converts assembly to machine code.
- `emulator.c`: The main code for the emulator, simulating RISC-V instruction execution.
- `vfs.c` & `vfs.h`: The Virtual File System, allowing basic file I/O operations.
- `sample.s`: Example RISC-V assembly code to test the assembler and emulator.

## Future Improvements

1. **Extended Instruction Support**: Implement additional RISC-V extensions (e.g., RV32M for multiplication).
2. **Improved VFS**: Enhance the Virtual File System with more advanced features like directories and file permissions.
3. **Memory Management**: Implement a more sophisticated memory model with different memory regions.
4. **Debugging Features**: Add breakpoint support and a step-through debugging mode.
5. **Performance Optimization**: Explore techniques like dynamic recompilation for faster emulation.
6. **GUI**: Develop a graphical user interface for easier interaction and visualization of the emulator's state.
7. **Interrupt Handling**: Implement interrupt and exception handling for a more realistic CPU simulation.
8. **Peripheral Emulation**: Add emulation of common peripherals like UART for console I/O.
9. **Multi-core Support**: Extend the emulator to support multi-core RISC-V configurations.
10. **Floating-Point Support**: Implement the RISC-V floating-point extensions.

## Conclusion

This project has been a deep dive into RISC-V, assembly language, emulation, and low-level programming. It has provided hands-on experience with key computer engineering concepts and helped sharpen my C programming skills. I hope to keep building on this and explore more advanced topics in the world of computer architecture and emulation.

