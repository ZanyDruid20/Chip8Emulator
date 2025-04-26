#define main SDL_main
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <SDL/SDL.h>
#include "chip8.h"



/* These functions are available to you to use in your code. */
uint8_t chip8_mem_read(uint16_t addr);

void chip8_mem_write(uint16_t addr, uint8_t val);
uint8_t chip8_register_read(uint8_t reg);
void chip8_register_write(uint8_t reg, uint8_t val);
void chip8_clear_frame(void);
void chip8_mem_clear(void);
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);
void chip8_mem_reset(void);

// initialization of the program counters, stack pointers, stack arrays, I registers and the array registers
uint16_t pc; 
uint16_t sp; 
uint16_t opcode;
uint16_t I;
uint16_t stack[16];
uint8_t registers[16];

/*
    
*/
void chip8_init(void) {
    chip8_mem_reset();
    pc = 0x200; // Program counter
    I = 0;
    
    sp = 0;

    chip8_clear_frame();
    for(int i = 0; i < 16; i++) {
        registers[i] = 0;
    }
    registers[CHIP8_REG_DT] = 0;
    registers[CHIP8_REG_ST] = 0;

    for(int i = 0; i < 80; i++) {
        chip8_mem_write(0x050 + i, chip8_mem_read(i));
    }
}

/*
    Reset the state of your CHIP-8 Emulation code, including resetting any CPU registers, clearing the stack, etc. 
    The program counter must be initialized to 0x200, the i register to 0, and the stack pointer to 0. 
    The general-purpose (Vx) registers must be cleared to 0.
*/
void chip8_reset(void) {
    pc = 0x200;
    I = 0;
    sp = 0;
    for(int i = 0; i < 16; i++) {
        registers[i] = 0;
    }
    chip8_register_write(CHIP8_REG_DT, 0);
    chip8_register_write(CHIP8_REG_ST, 0);


}
/*
    Clean up anything your emulator requirese to be cleaned up on shut down. 
    This function may be empty if it is not required in your design, but it must be provided.

*/
void chip8_shutdown(void) {
    return;
}

/*
    Initialized the X,Y,kk, n, nn and nnn registers
    Incremented the program counter 2 times each
    used the bit shift for registers X and Y
    Packaged all the opcode instructions into several switch statements
    depending on the last values.

    1. Opcode 00E0 clears the frame
    2. Opcode 0x1000 sets pc to nnn
    3. Opcode 0x6000 sets register X to kk
    4. Opcode 0x7000 adds the value of kk to register X
    5. Opcode 0xA000 sets the register I to nnn
    6. The display opcodes initializes the x and y coordinates , the height and the address in memory where 
    sprite data begins. After that it checks for invalid memory access and if it passes the test meaning no 
    invalid memory access then it will draw the sprite and check for memory collision.
    7. Opcode 0x2000 calls subroutines at address nnn.
    8. Opcodes 0x3000 and 0x4000 have similar instructions, however there is a difference, in 0x3000 if register x equals to kk it skips
    and 0x4000 skips when registers x doesn't equal to kk and they both increments the pc by 2
    9. Opcodes 0x5000 skips instructions if registers X and Y have the same value and it increments the pc by 2
    10. Opcodes 0x8000 and its subordinates opcodes do bitwise operations
    11. Opcodes 0x9000 skips the instruction if registers X and Y are not equal and it incerements the program counter by 2.
    12. Opcodes 0xB000 jumps to offset (pc = registers[0] + nnn;)
    13. Opcodes 0xC000 generates a random number and it performs an AND operation with kk
    14. Opcodes 0xFX07 sets the delay timer
    15. Opcodes 0xFX0A stops the instructions and it waits for key input
    16. 0xFX15 sets delay timer to register X and 0xFX18 sets the sound timer to register X
    17. EX9E will skip an instruction if the key value corresponding to register X is pressed 
    18. EXA1 skips the instructionif the key value corresponding to X is not pressed.
*/
void chip8_execute_instruction(void) {
    uint8_t X, Y, kk, n;
    uint16_t nn, nnn;
    opcode = (chip8_mem_read(pc) << 8) |chip8_mem_read(pc+1);
    pc += 2;
    X = (opcode & 0x0F00) >> 8;
    Y = (opcode & 0x00F0) >> 4;
    n = opcode & 0x000F;
    kk = opcode & 0x00FF;
    nnn = opcode & 0x0FFF;
    switch (opcode & 0xF000)
    {
    case 0x0000:
        if (opcode == 0x00E0) {
            chip8_clear_frame();
        } else if (opcode == 0x00EE) {
            if (sp == 0) {
                printf("Stack Underflow error \n");
            } else {
                pc = stack[sp];
                sp--;
            }
        } else {
            printf("SYS Call Ignored: %04X\n", opcode);
        }
        break;
    case 0x1000:
        pc = nnn;
        break;
    case 0x6000:
        registers[X] =kk;
        break;
    case 0x7000:
        registers[X] += kk;
        break; 
    case 0xA000:
        I = nnn;
        break;
    case 0xD000:
        uint8_t x = registers[X];
        uint8_t y = registers[Y];
        uint8_t height = n;
        uint16_t addr = I;
        printf("X (%d) value: %d\n", X, x);
        printf("Y (%d) value: %d\n", Y, y);
        printf("hEIGHT VALUE : %d\n", height);
        printf("address variable: %X\n", addr);
        if (addr  >= 0x1000) {
            printf("Invalid Access in memory, in draw sprite , I = %X\n", addr);
        } else {
            int collision = chip8_draw_sprite(addr,x,y,height);
            registers[0xF] = collision;
        }
        break;
    case 0x2000:
        if(sp >= 16) {
            printf("Error \n");
        } else {
            sp++;
            stack[sp] = pc;
            pc = nnn;
        }
        break;
    case 0x3000:
        if (registers[X] == kk) {
            pc += 2;
        }
        break;
    case 0x4000:
        if (registers[X] != kk) {
            pc += 2;
        }
        break;
    case 0x5000:
        if(registers[X] == registers[Y]) {
            pc += 2;
        }
        break;
    case 0x8000:
        switch(n) {
            case 0x0:
                registers[X] = registers[Y];
                break;
            case 0x1:
                registers[X] |= registers[Y];
                break;
            case 0x2:
                registers[X] &= registers[Y];
                break;
            case 0x3:
                registers[X] ^= registers[Y];
                break;
            case 0x4:
                uint16_t sum = registers[X] + registers[Y];
                registers[0xF] = (sum > 0xFF) ? 1 : 0;
                registers[X] = sum & 0xFF;
                break;
            case 0x5:
                registers[0xF] = (registers[X] >= registers[Y]) ? 1 : 0;
                registers[X] = (registers[X] - registers[Y]) & 0xFF;
                break;
            case 0x7:
                registers[0xF] = (registers[Y] >= registers[X]) ? 1 : 0;
                registers[X] = (registers[Y] - registers[X]) & 0xFF;
                break;
            case 0x6:
                registers[0xF] = registers[X] & 0x1;
                registers[X]  = registers[X] >> 1;
                break;
            case 0xE:
                registers[0xF] = (registers[X] >> 7) & 0x1;
                registers[X] = registers[X] << 1;
                break;
            default:
                break;
        }
        break;
    case 0x9000:
        if (registers[X] != registers[Y]) {
            pc += 2;
        }
        break;
    case 0xB000:
        pc = registers[0] + nnn;
        break;
    case 0xC000:
        registers[X] = (rand() % 256) & kk;
        break;
    case 0xF000:
        switch(kk) {
            case 0x07:
                registers[X] = chip8_register_read(CHIP8_REG_DT);
                break;
            case 0x0A:
                bool key_pressed = false;
                for (int i = 0; i < 16; i++) {
                    if(chip8_register_read(i)) {
                        registers[X] = i;
                        printf("Button Values:  %d\n" , registers[X]);
                        key_pressed = true;
                        break;
                    }
                }
                if(!key_pressed) {
                    pc -= 2;
                }
                break;
            case 0x15:
                chip8_register_write(CHIP8_REG_DT, registers[X]);
                break;
            case 0x18:
                chip8_register_write(CHIP8_REG_ST, registers[X]);
                break;
            case 0x1E:
                I += registers[X];
                break;
            case 0x29:
                uint8_t myCharacter = registers[X];
                I = 0x050 + (myCharacter * 5);
                break;
            case 0x33:
                uint8_t BCD = registers[X];
                chip8_mem_write(I,BCD/100);
                chip8_mem_write(I + 1, (BCD/10) % 10);
                chip8_mem_write(I + 2, BCD % 10);
                break;
            case 0x55:
                for (int i = 0; i <= X; i++) {
                    chip8_mem_write(I + i, registers[i]);
                }
                break;
            case 0x65:
                for (int i = 0; i <= X; i++) {
                    registers[i] = chip8_mem_read(I + i);
                }
                break;
            default:
                break;
        }
        break;
    case 0xE000:
        switch(kk) {
            case 0x9E:
                if (chip8_register_read(registers[X])) {
                    pc += 2;
                }
                break;
            case 0xA1:
                if (!chip8_register_read(registers[X])) {
                    printf("Key %X is NOT pressed! Skipping instruction\n", registers[X]);
                    pc += 2;
                }
                break;
            default:
                break;
        }
        break;
    default:
        break;
    }
}




