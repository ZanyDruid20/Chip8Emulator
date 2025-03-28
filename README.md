[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XpvI6cTa)

# STARTING
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


        







