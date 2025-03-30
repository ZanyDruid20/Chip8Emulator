#define main SDL_main
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <SDL/SDL.h>
#include "C:/msys64/home/Furnom Dam/chip8Emulator/project-ZanyDruid20/chip8.h"


/* These functions are available to you to use in your code. */
uint8_t chip8_mem_read(uint16_t addr);
void chip8_mem_write(uint16_t addr, uint8_t val);
uint8_t chip8_register_read(uint8_t reg);
void chip8_register_write(uint8_t reg, uint8_t val);
void chip8_clear_frame(void);
void chip8_mem_clear(void);
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);
void chip8_mem_reset(void);

uint16_t pc; 
uint8_t dt, st;
uint16_t sp; 
uint16_t opcode;
uint16_t I;
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
    sp = 0;
    chip8_clear_frame();
    for(int i = 0; i < 16; i++) {
        chip8_register_write(i,0);
    }
    chip8_register_write(CHIP8_REG_DT,0);
    chip8_register_write(CHIP8_REG_ST,0);
    
}
void chip8_reset(void) {
    return;
}
void chip8_shutdown(void) {
    return;
}
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
    printf("Fetched opcode: %04X\n", opcode);
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
    case 0x2000:
        if(sp >= 16) {
            printf("Error \n");
        } else {
            stack[sp++] = pc;
            pc = nnn;
        }
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
    case 0x00EE:
        if (sp == 0) {
            printf("Stack Underflow error \n");
        } else {
            sp--;
            pc = stack[sp];
        }
        break;
    default:
        printf("Unknown opcode: %04X\n", opcode); // Handle unknown opcodes
        break;
    }
}




