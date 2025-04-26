#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

// Function Declarations for CHIP-8 operations

// Memory operations
uint8_t chip8_mem_read(uint16_t addr);     // Read from memory at specified address
void chip8_mem_write(uint16_t addr, uint8_t val);  // Write value to memory at specified address
void chip8_mem_clear();                    // Clear the memory
void chip8_mem_reset();                    // Reset memory

// Register operations
uint8_t chip8_register_read(uint8_t reg);  // Read the value of a register
void chip8_register_write(uint8_t reg, uint8_t val);  // Write value to a register

// Screen operations
void chip8_clear_frame();                  // Clear the screen/frame buffer
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);  // Draw a sprite at the specified location

// System operations
void chip8_reset();                        // Reset the CHIP-8 system
void chip8_shutdown();                     // Shut down the CHIP-8 system
void fill_audio();                         // Handle the audio (sound)


// SDL related functions
int SDL_main(int argc, char *argv[]);      // SDL's main entry point

/* Key statuses are defined as registers 0x00-0x0F */

#define CHIP8_REG_DT        0x10
#define CHIP8_REG_ST        0x11



#endif // CHIP8_H
