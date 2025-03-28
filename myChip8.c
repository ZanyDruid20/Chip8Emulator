
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <SDL/SDL.h>


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

/* Key statuses are defined as registers 0x00-0x0F */
#define CHIP8_REG_DT        0x10
#define CHIP8_REG_ST        0x11

void chip8_init(void) {
    /*(Initialize )*/
    chip8_mem_reset();
    pc = 0x200; // Program counter
    dt = 0;
    st = 0;
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
}




