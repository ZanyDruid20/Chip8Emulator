# CHIP-8 Emulator in C

A fully functional CHIP-8 virtual machine emulator built from scratch in C as part of the CMSC 411 (Computer Architecture) course at UMBC. This emulator implements the core CHIP-8 instruction set, memory and register management, keyboard input handling, and graphics rendering using SDL2.

---

## Project Overview

This emulator recreates the classic CHIP-8 system, which was originally designed as a simple interpreted programming language for early home computers. The project provided hands-on experience with low-level programming concepts such as opcode decoding, memory management, input/output processing, and graphical rendering.

**Why this project?**  
- To deepen understanding of computer architecture and instruction set implementation  
- To practice modular C programming with real-time input and graphics handling  
- To integrate with SDL2 for cross-platform graphical output  
- To solve debugging challenges and maintain clean code structure  

---

## Features

- Complete implementation of CHIP-8 fetch-decode-execute cycle  
- Support for all major CHIP-8 opcodes including drawing sprites, timers, and input  
- Memory management with proper font data loading  
- Keyboard input handling with key press detection and input-wait opcode  
- Graphics rendered via SDL2 with sprite drawing support  
- Modular and maintainable codebase  

---

## Usage

- Load any valid CHIP-8 ROM file to start emulation  
- Use the mapped keyboard keys to simulate CHIP-8 keypad input  
- The emulator will render graphics in a window via SDL2  
- Sound and timers emulate original CHIP-8 behavior  

*Note: Key mappings and controls are documented within the code and README.*

---

## Challenges & Learning

- Handling opcode decoding and branching logic correctly  
- Managing shared state between multiple modules in C  
- Debugging graphical output and input timing issues  
- Ensuring font memory persistence after resets  
- Using SDL2 for real-time rendering without modifying base SDL code  

---

## Credits

- Project completed for CMSC 411, UMBC  
- CHIP-8 technical references:  
  - [Cowgod CHIP-8 Documentation](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#keyboard)  
  - [Lepuri's CHIP-8 Guide](https://lepuri.net/making-a-chip-8-emulator-in-c/)  
  - [Tobias Langhoff's Emulator Blog](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)  
- AI-assisted debugging and logic with ChatGPT  

---


---

## Video Demo 1
---
https://www.loom.com/share/5f5c938413224f41b164d2b3d35e7397

## Video Demo 2
https://www.loom.com/share/0de6db8584604ae995a05909e30b894f

