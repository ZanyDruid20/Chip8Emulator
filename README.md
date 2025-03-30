[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XpvI6cTa)

# STARTING
<<<<<<< HEAD
I set up the folder and now i ll begin to code this week, had a long week from exams.

# FRIDAY 28TH OF MARCH 2025
Good Evening I This is my Readme Message for the second day and i initalized the emulator and i started with the execution function by implementing the fetch and decoding and then on the third day im going to implement 10 out of 35 opcodes which means implementing execute, how ever , In the course of implementation I used resources to better help me understand and guide me through the implementation process. One of these resources to help me out was ChatGPT, this were the prompts i gave to it.

FIRST PROMPT: 
    PROMPT 1:
    To properly define chip8_init(void), we need to ensure the following steps are performed:

    Reset the memory using chip8_mem_reset().

    Clear the display (framebuffer) using chip8_clear_frame().

    Reset all general-purpose registers (V0 - VF).

    Set delay (DT) and sound (ST) timers to zero.

    Set the program counter (PC) to 0x200, since CHIP-8 programs start execution from this address.

    Reset the stack pointer (SP) if a stack is used in the implementation.

    Here’s how the implementation of chip8_init(void) could look:

    c
    Copy
    Edit
    void chip8_init(void) {
        // Reset memory and load ROM if applicable
        chip8_mem_reset();

        // Clear the framebuffer (display)
        chip8_clear_frame();

        // Reset all registers (V0 - VF)
        for (uint8_t i = 0; i < 16; i++) {
            chip8_register_write(i, 0);
        }

        // Reset delay and sound timers
        chip8_register_write(CHIP8_REG_DT, 0);
        chip8_register_write(CHIP8_REG_ST, 0);

        // Reset stack pointer and program counter
        sp = 0;  // Assuming there's a stack pointer variable
        pc = 0x200;  // Start execution from address 0x200
    }
    Explanation:

    Memory Reset: Calls chip8_mem_reset() to clear memory and load a ROM if one exists.

    Clear Display: Calls chip8_clear_frame() to ensure the screen is cleared before starting execution.

    Register Reset: Iterates through the general-purpose registers (V0 - VF) and sets them to zero.

    Timer Reset: Sets the delay (DT) and sound (ST) timers to zero.

    Stack & Program Counter: Ensures execution starts at the correct memory location (0x200) and resets the stack pointer.

    Next Steps:

    If sp and pc are not declared in your project, make sure they exist as global variables:

    uint16_t pc;
    uint8_t sp;
    Call chip8_init() at the start of execution to ensure your CHIP-8 emulator starts in a clean state every time it initializes.
SECOND PROMPT:
    Prompt 2:
        "So I got this from the resource that my teacher sent, it’s from Tobias Langoff, one of the resources you gave me and he says this:

    Fetch:
    Read the instruction that the Program Counter (PC) is currently pointing at from memory. An instruction is two bytes, so you will need to read two successive bytes from memory and combine them into one 16-bit instruction.
    Then, I came across another resource with the following execute() function:
        void execute() {
            uint8_t X, Y, nn, n;
            uint16_t nnn;

            // Fetch
            opcode = memory[PC] << 8 | memory[PC + 1];
            PC += 2;

            // Decoding
            X = (opcode & 0x0F00) >> 8;
            Y = (opcode & 0x00F0) >> 4;
            n = (opcode & 0x000F) >> 2;
            kk = (opcode & 0x00FF);
            nnn = (opcode & 0x0FFF);
            printf("Opcode: %x\n", opcode);
            printf("Program Counter: %x\n", PC);
            printf("I: %x\n", I);
    }
    This explanation was from lepuri.net. I need guidance on how to implement the fetch and execute cycle. Can you provide me with pseudocode of the full cycle to help me get started?"
    Response 2:
    Here’s a structured pseudocode outline for implementing the fetch and execute cycle in your CHIP-8 emulator. You can use this as a reference for implementing fetching, decoding, and executing CHIP-8 instructions.

    1. Fetch:

    Read the instruction at the memory address stored in PC (Program Counter).

    Combine two successive bytes from memory into one 16-bit opcode.

    Increment PC by 2 (since each instruction is 2 bytes).
    opcode = chip8_mem_read(PC) << 8 | chip8_mem_read(PC + 1)
    PC += 2

    2. Decode:
        Extract the different parts of the opcode (e.g., X, Y, n, nn, nnn) based on the structure of the opcode.

        X: The register index (upper 4 bits of the opcode).

        Y: The register index (next 4 bits of the opcode).

        n: The 4-bit value (used in some instructions).

        kk: The 8-bit immediate value (lower 8 bits of the opcode).

        nnn: The 12-bit address (lowest 12 bits of the opcode).
    X = (opcode & 0x0F00) >> 8   // Register index
    Y = (opcode & 0x00F0) >> 4   // Register index
    n = opcode & 0x000F          // 4-bit value
    nn = opcode & 0x00FF         // 8-bit immediate value
    nnn = opcode & 0x0FFF        // 12-bit address
Edits Made: I reviewed and understood the information provided. The response was left largely intact as it served as a helpful structure for implementing the fetch and decode cycle. I focused on integrating this pseudocode into my project and adjusted the language for clarity within my coding context.

How It Was Used: I used ChatGPT to generate a clear breakdown of the fetch and decode process for my CHIP-8 emulator's instruction cycle. I then applied the pseudocode provided in my implementation


        


# SATURDAY 29TH OF MARCH 2025
Good evening , I have finally implemented 10 out of 35 opcodes, In the course of this implementation and the first checkpoint, I was able to use resources to help me with planning the logic, implementing the function.Additionally, I encountered a few issues related to debugging, such as unexpected behavior when running test ROMs. One of the resources I used today was ChatGPT, https://lepuri.net/making-a-chip-8-emulator-in-c/ and
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/

Prompts Given:
0NNN: Execute machine language routinePermalink
We’ll start out with an instruction that you actually don’t want to implement! In the original CHIP-8 interpreters, this would pause execution of the CHIP-8 program and call a subroutine written in machine language at address NNN instead.

This routine would be written in the machine language of the computer’s CPU; on the original COSMAC VIP and the ETI-660, this was 1802 machine code, and on the DREAM 6800, M6800 code. Unless you’re making an emulator for either of those computers, skip this one.

00E0: Clear screenPermalink
This is pretty simple: It should clear the display, turning all pixels off to 0.

1NNN: JumpPermalink
This instruction should simply set PC to NNN, causing the program to jump to that memory location. Do not increment the PC afterwards, it jumps directly there.

00EE and 2NNN: SubroutinesPermalink
2NNN calls the subroutine at memory location NNN. In other words, just like 1NNN, you should set PC to NNN. However, the difference between a jump and a call is that this instruction should first push the current PC to the stack, so the subroutine can return later.

Returning from a subroutine is done with 00EE, and it does this by removing (“popping”) the last address from the stack and setting the PC to it.

3XNN, 4XNN, 5XY0 and 9XY0: Skip conditionallyPermalink
These instructions do the same thing: They either do nothing, or they skip one two-byte instruction (increment PC by 2) if some condition is true. (If you didn’t increment PC in the “fetch” stage above, they will obviously increment PC by either 4 or 2.)

Or, put another way, they execute the next instruction if and only if the condition is not true. Since these conditional branch instructions can only skip one instruction, they’re usually followed by a jump/call (1NNN/2NNN) instruction which jumps to the actual “if code block” that should be executed if the condition is true.

3XNN will skip one instruction if the value in VX is equal to NN, and 4XNN will skip if they are not equal.

5XY0 skips if the values in VX and VY are equal, while 9XY0 skips if they are not equal.

6XNN: SetPermalink
Simply set the register VX to the value NN.

7XNN: AddPermalink
Add the value NN to VX.: How many opcodes now
Response:
    The opcodes you listed are:

    00E0 - Clear screen

    1NNN - Jump to address NNN

    00EE - Return from subroutine

    2NNN - Call subroutine at NNN

    3XNN - Skip next instruction if VX == NN

    4XNN - Skip next instruction if VX != NN

    5XY0 - Skip next instruction if VX == VY

    9XY0 - Skip next instruction if VX != VY

    6XNN - Set VX to NN

    7XNN - Add NN to VX (without carry)


Purpose: I copied this from the tobias lngoff web page and i asked it to make the list of 10 opcodes for me implement

Second Prompt:
 I am using ChatGPT to help me learn how to write the ten opcodes. I want to **incrementally** understand and explore the problem, but I do **not** want a direct solution or any code provided unless I specifically ask for it. Please respond by guiding my thought process, asking clarifying questions, and providing conceptual explanations rather than writing out code and pls help me visualise it by writing the pseudocode, not the actual code


Response:
Great approach! Let's break this down step by step.

You need to implement 10 opcodes. From what you've shared, here are some candidates that are relatively straightforward:

00E0 - Clear Screen

This opcode clears the display, setting all pixels to 0.

1NNN - Jump

This sets the program counter (PC) to the given address NNN.

6XNN - Set Register VX

This sets the value of register VX to NN.

7XNN - Add to Register VX

This adds NN to VX (without affecting a carry flag).

ANNN - Set Index Register I

This sets the index register I to NNN.

DXYN - Display/Drawing Instruction

This draws a sprite at (VX, VY) with a height of N, reading sprite data from I.

2NNN - Call Subroutine

This pushes the current PC onto the stack and jumps to NNN.

00EE - Return from Subroutine

This pops the last address from the stack and sets PC to that address.

3XNN - Skip if Equal

If VX == NN, skip the next instruction.

4XNN - Skip if Not Equal

If VX != NN, skip the next instruction.

purpose: It helped me to list what i should do.

Third Prompt:
    switch (opcode)
    {
    case 0X00E0:
        /* code */
        chip8_clear_frame();
        break;
    
    default:
        break;
    } is this case right give me one word answer yes or no

Repsonse:
Yes
Purpose: After implementing this I asked if this code was on the right path.

Third Prompt:
case 0X1NNN:
        pc = nnn;
        break; OX1NNN shows red why


Response:
The issue is likely due to a typo. It should be 0x1NNN instead of OX1NNN. The 0x prefix denotes hexadecimal values in C/C++ (and similar languages), so OX1NNN is not recognized as valid syntax.

The correct case would look like this:
case 0x1NNN:
    pc = nnn;
    break;
Fourth Prompt:
0x1NNN still shows red
Response:
The issue is likely related to the way the value is being parsed or used. In the case of 0x1NNN, it should be part of the instruction's decoding process, where NNN is extracted from the opcode and replaced with the proper value.

Here’s a quick breakdown:

The 0x prefix is correct for hexadecimal notation.

NNN is not directly a part of the instruction and should be parsed out of the instruction.

To fix this, you need to extract the NNN part from the opcode. For example, if your opcode is a 16-bit value, the NNN value is typically the lower 12 bits.

Example of extracting NNN:
uint16_t opcode = 0x1ABC;  // Example opcode
uint16_t nnn = opcode & 0x0FFF;  // Mask the lower 12 bits to get NNN

switch (opcode) {
    case 0x1000:  // This is just an example
        pc = nnn;
        break;
    
    default:
        break;
}

edit:
    switch (opcode & 0XF000)
    {
    case 0x00E0:
        /* code */
        chip8_clear_frame();
        break;
    case 0x1000:
        nnn = opcode & 0x0FFF;
        pc = nnn;
        break;
    }

FIFTH Prompt:
case 0X6NNN:
        chip8_register_write(X,nnn);
        break; is this alright

Response:
case 0x6000: // For example, if the opcode is 0x6XNN
    // Extract the register and the value
    X = (opcode & 0x0F00) >> 8;  // Extract the register (X)
    nn = opcode & 0x00FF;         // Extract the 8-bit value (NN)
    chip8_register_write(X, nn);  // Write the value to the register
    break;
Edits:
case 0x6000:
        chip8_register_write(X,kk);
        break;
Sixth Prompt:
void chip8_execute_instruction(void) {
    uint8_t X, Y, nn, n;
    uint16_t kk, nnn;
    opcode = chip8_mem_read(pc) << 8 |chip8_mem_read(pc+1);
    pc += 2;
    X = (opcode & 0x0F00) >> 8;
    Y = (opcode & 0x00F0) >> 4;
    n = opcode & 0x000F;
    kk = opcode & 0x00FF;
    nnn = opcode & 0x0FFF;
    printf("Value of X is:  %d\n", X);
    printf("Value of Y is:  %d\n", Y);
    printf("Value of n is:  %d\n", n );
    printf("Value of kk is: %d \n", kk);
    printf("Value of nnn is: %d \n", nnn);
    switch (opcode & 0XF000)
    {
    case 0x00E0:
        /* code */
        chip8_clear_frame();
        break;
    case 0x1000:
        nnn = opcode & 0x0FFF;
        pc = nnn;
        break;
    case 0x6000:
        chip8_register_write(X,nnn);
        break;
    default:
        break;
    }
} is this approach getting okay, if not then i need help I just need you to tell me what to do without giving me the code
Response:
Your approach is on the right track, but there are a few things you need to adjust:

Opcode Extraction:

X, Y, and n look correct for extracting the necessary parts from the opcode.

However, the immediate value kk should be extracted as opcode & 0x00FF which seems correct already.

nnn is correct with opcode & 0x0FFF.

For Case 0x6000:

The instruction 0x6000 (set register VX to NN) is not correctly implemented yet. You're passing nnn (which is a 12-bit value) to chip8_register_write(). Instead, you should use kk (the 8-bit immediate value) as the value to store into the register VX.

General Advice for Each Case:

In your switch statement, you’re correctly checking for different opcode prefixes, but you need to ensure that in each case you're extracting the correct parts of the opcode. For example, when you're handling the instruction 0x6000, make sure to use X for the register and kk for the immediate value.

For each case, ensure that you are updating the PC, register values, or memory correctly according to the opcode description.

Steps to Proceed:

1.Check the opcode prefixes correctly, and only handle specific instruction types (like 0x6000).

2.Implement chip8_register_write(X, kk) in case of 0x6000. Ensure that X is the register you want to modify and kk is the 8-bit value you want to load into that register.

3.Handle other cases based on their respective operations (e.g., 0x1NNN, 0x00E0, etc.).

4.Ensure that in your debugging outputs (e.g., printf), you print meaningful values for X, Y, etc., and update the PC when necessary.

Seventh Prompt:
which one should i tackle next in your opinion


Response:
I'd recommend tackling 7XNN next. It's a simple addition operation, which should be relatively straightforward to implement. The logic is clear: you need to add the value NN to the register VX, and there are no complex conditions or additional registers involved.

Once that's done, you can focus on ANNN (setting the index register I), which is also quite simple and involves a straightforward assignment.

Let me know if you'd like guidance on how to implement it!



Eight Prompt:
 case 0xANNN:
        I = nnn;
        break; why does 0xANNN not work


Response:
case 0xA000:
    I = nnn;
    break;
Ninth Prompt:
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <SDL/SDL.h>
#include "sdl-basecode.c"


/* These functions are available to you to use in your code. */
uint8_t chip8_mem_read(uint16_t addr);
void chip8_mem_write(uint16_t addr, uint8_t val);
uint8_t chip8_register_read(uint8_t reg);
void chip8_register_write(uint8_t reg, uint8_t val);
void chip8_clear_frame(void);
void chip8_mem_clear(void);
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);
void chip8_mem_reset(void);

static uint8_t pc; 
static uint8_t dt, st;
static uint8_t sp; 
static uint8_t opcode;
static uint16_t I;
uint16_t stack[16];

/* Key statuses are defined as registers 0x00-0x0F */
#define CHIP8_REG_DT        0x10
#define CHIP8_REG_ST        0x11

void chip8_init(void) {
    /*(Initialize )*/
    chip8_mem_reset();
    pc = 0x200; // Program counter
    dt = 0;
    st = 0;
    I = 0;
    chip8_clear_frame();
    for(int i = 0; i < 16; i++) {
        chip8_register_write(i,0);
    }
    chip8_register_write(CHIP8_REG_DT,0);
    chip8_register_write(CHIP8_REG_ST,0);
    
}
void chip8_reset(void);
void chip8_shutdown(void);
void chip8_execute_instruction(void) {
    uint8_t X, Y, kk, n;
    uint16_t nn, nnn;
    opcode = chip8_mem_read(pc) << 8 |chip8_mem_read(pc+1);
    pc += 2;
    X = (opcode & 0x0F00) >> 8;
    Y = (opcode & 0x00F0) >> 4;
    n = opcode & 0x000F;
    kk = opcode & 0x00FF;
    nnn = opcode & 0x0FFF;
    printf("Value of X is:  %d\n", X);
    printf("Value of Y is:  %d\n", Y);
    printf("Value of n is:  %d\n", n );
    printf("Value of kk is: %d \n", kk);
    printf("Value of nnn is: %d \n", nnn);
    printf("Value of I: %x\n", I );
    switch (opcode & 0XF000)
    {
    case 0x00E0:
        /* code */
        chip8_clear_frame();
        break;
    case 0x1000:
        nnn = opcode & 0x0FFF;
        pc = nnn;
        break;
    case 0x6000:
        chip8_register_write(X,kk);
        break;
    case 0x7000:
        chip8_register_write(X, chip8_register_read(X) + kk);
        break; 
    case 0xA000:
        I = nnn;
        break;
    case 0xD000:
        uint8_t height = n;
        chip8_draw_sprite(I,chip8_register_read(X),chip8_register_read(Y),height);
        break;
    case 0x2000:
        stack[sp++] = pc;
        pc = nnn;
        break;
    case 0x3000:
        if (chip8_register_read(X) == kk) {
            pc += 2;
        }
        break;
    case 0x4000:
        if (chip8_register_read(X) != kk) {
            pc += 2;
        }
        break;
    case 0x5000:
        if(chip8_register_read(X) == chip8_register_read(Y)) {
            pc += 2;
        }
        break;
    default:
        break;
    }
} are there any issues with this 


Response:
There are a few areas in your code that might need some attention. Here are some suggestions to ensure everything works as expected:

Program Counter Increment (pc): You are modifying the pc in some cases (like 0x1000, 0x2000, etc.) but in some cases (like 0x3000 and 0x4000), you only increment it conditionally. This is correct behavior, but make sure that pc is properly incremented in every situation where it should move to the next instruction (unless explicitly handled by a jump or subroutine call).

Tip: In case of conditional cases, remember that you need to increment pc after executing the instruction unless you want to skip to the next instruction.

Stack Pointer (sp): You use sp for pushing and popping values from the stack. However, sp is declared as a uint8_t (8-bit), which only allows values from 0 to 255. Since you're using sp to push and pop addresses (which are 16-bit), you might run into issues if sp overflows beyond 255.

Suggestion: It would be better to declare sp as a uint16_t to properly handle the stack.

Handling I in 0xD000 (Draw Sprite): In the 0xD000 case, you use I as the address for drawing the sprite. Make sure that I contains the correct value before calling chip8_draw_sprite(). If I hasn't been set to a valid address (for example, through 0xA000 or another instruction), the drawing function may fail or access invalid memory.

Suggestion: Add a check or assert to ensure I is valid before using it for sprite drawing.

Return Stack Pop (0x2000): When you push pc onto the stack (in the 0x2000 case), make sure you later pop it when returning from the subroutine (using 0x00EE or similar instructions). The stack should be properly managed for function calls and returns.

Default Case: Your default case does nothing, which is fine if you don't plan on implementing other opcodes. However, it's a good practice to at least print a message when an unknown opcode is encountered. This will help during debugging.

Suggestion: Add a debug print for unknown opcodes to track unexpected ones during execution.

Reset Conditions: Ensure that all relevant registers and memory are correctly reset and initialized during the initialization phase (such as chip8_mem_reset).

Summary of Suggestions:
Stack Pointer: Change sp to uint16_t for proper handling of stack operations.

Program Counter: Make sure to increment pc correctly and conditionally (in the cases where it’s not explicitly jumped).

I Validity: Ensure that I is set correctly before using it, especially for 0xD000.

Unknown Opcodes: Add a debug print for unknown opcodes in the default case.

Tenth Prompt:
Handling I in 0xD000 (Draw Sprite): In the 0xD000 case, you use I as the address for drawing the sprite. Make sure that I contains the correct value before calling chip8_draw_sprite(). If I hasn't been set to a valid address (for example, through 0xA000 or another instruction), the drawing function may fail or access invalid memory.  case 0xD000:
        if (I == 0) {
            printf("This is Bad \n");
        } else {
            uint8_t height = n;
            chip8_draw_sprite(I,chip8_register_read(X),chip8_register_read(Y),height);
        }
        break; is this good
Then I asked it to give me the pseudocode
Response:
IF I is not set OR I is outside valid memory range THEN
    Print "Error: Invalid I register value"
ELSE
    height = last 4 bits of opcode (n)
    Read X and Y registers
    Call chip8_draw_sprite(I, X, Y, height)
END IF
11th Prompt:
case 0xD000:
        if (I  >= 0x2000 & I < 0xA000) { 
            printf("This is Bad \n");
        } else {
            uint8_t height = n;
            chip8_draw_sprite(I,chip8_register_read(X),chip8_register_read(Y),height);
        }
        break; is this good
Response:
No.

Your condition if (I >= 0x2000 & I < 0xA000) has two issues:

Bitwise AND (&) instead of Logical AND (&&)

& is a bitwise operator, whereas && is the correct logical operator for conditions.

The condition should be if (I >= 0x2000 && I < 0xA000).

Memory Range Check is Incorrect

I should be within valid CHIP-8 memory (typically 0x000 to 0xFFF for a 4KB system).

Checking I >= 0x2000 makes no sense since CHIP-8 has only 4KB of memory (0x1000 max).

A better check would be:

plaintext
Copy
Edit
IF I < 0x200 OR I >= memory_size THEN
    Print "Invalid I value"
ELSE
    Draw sprite
END IF
The 0x200 lower bound ensures I doesn’t access the reserved interpreter memory (font data).

Edit:
case 0xD000:
        uint8_t x = chip8_register_read(X);
        uint8_t y = chip8_register_read(Y);
        uint8_t height = n;
        uint16_t addr = I;
        if (addr  >= 0x1000) {
            printf("Invalid Access in memory, in draw sprite , I = %X\n", addr);
        } else {
            int collision = chip8_draw_sprite(addr,x,y,height);
            chip8_register_write(0xF,collision);
        }
        break;
After many edits, (will update this part tomorrow)
12th Prompt:
do i need to implement 0x200 

Response:
Yes, you should implement 0x2000 (CALL subroutine at NNN) if your CHIP-8 specification supports it. However, I think you're referring to 0x00EE (Return from subroutine) instead, which is the missing counterpart of your 0x2000 (CALL NNN) implementation.

Why You Need 0x00EE
You already implemented 0x2000 (CALL NNN), which pushes the current pc onto the stack and jumps to nnn.

Without 0x00EE (RETURN), your program can call subroutines but can never return!

This will lead to an infinite loop or a crash.

How to Implement 0x00EE (Return from Subroutine)
You need to:

Pop the last program counter (PC) from the stack.

Set pc to the popped value.

Decrease the stack pointer (sp).

Pseudocode:
IF stack is empty THEN
    Print "Stack underflow error"
ELSE
    Decrement stack pointer (sp)
    Set pc = stack[sp]  // Pop last stored address
END IF

13th Prompt:
case 0x00EE:
        if (stack == 0) {
            printf("Stack Underflow error \n");
        } else {
            sp--;
            pc = stack[sp];
        }
Response:
Your implementation is almost correct, but there's a small issue:

Issues
if (stack == 0) is incorrect because stack is an array, not a counter. Instead, you should check if sp == 0 (since sp tracks the stack pointer).

sp-- should be before popping the value from the stack, which you did correctly.

case 0x00EE:
    if (sp == 0) {  // Check if stack pointer is at the bottom
        printf("Stack Underflow error \n");
    } else {
        sp--;  // Decrement stack pointer first
        pc = stack[sp];  // Pop the last stored address into pc
    }
    break;
Even though today Im not able to submit most of the prompts, i promise ill submit the rest because the deadline is fast approaching and i have no time, wll write more on it, from tomorrow, till the end of week.
Reources:
ChatGPT for debugging
Lepuri: https://lepuri.net/making-a-chip-8-emulator-in-c/
Tobias Langoff: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/



=======
I set up the folder and now i ll begin to code this week, had a long week from exams.
>>>>>>> a76e8d97d622dce48af7fe7547b230ca94c45eb0
