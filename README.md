[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XpvI6cTa)

# STARTING
I set up the folder and now i ll begin to code this week, had a long week from exams.

# FRIDAY 28TH OF MARCH 2025
Good Evening I This is my Readme Message for the second day and i initalized the emulator and i started with the execution function by implementing the fetch and decoding and then on the third day im going to implement 10 out of 35 opcodes which means implementing execute, how ever , In the course of implementation I used resources to better help me understand and guide me through the implementation process. One of these resources to help me out was ChatGPT, this were the prompts i gave to it.

# SATURDAY 29TH OF MARCH 2025
USER PROMPT, RESPONSE AND EDITS DOCUMENTS BELOW:
https://docs.google.com/document/d/1lCmzCjgPVxv5Ggl_tuwjT4JsQXTvIpe1bk6676Q8q4k/edit?usp=sharing


# SUNDAY 3OTH OF MARCH 2025
Today I made some changes to the read me updated on the 0XD000 and header files implementation, will do more tomorrow

# FRIDAY 18TH OF APRIL 2025
Finally implemented the registers with an arrays and the IBM logo came out successfully

# TUESDAY 22ND OF APRIL 2025
USER PROMPT AND RESPONSE DOCUMENT BELOW:
https://docs.google.com/document/d/1TCS7zqr0TmhDWnZlrZGGhgr9EgrIPfQejwXHA3xC6tM/edit?usp=sharing
EDITS:
```c
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

```

I packaged it within the 0x0E00 because the opcodes start with that and there are two remaining values with the kk register.

```c
case 0x55:
    for (int i = 0; i <= X; i++) {
        chip8_mem_write(I + i, registers[i]);
    }
    break;


```
Did the same as it is encoded within 0xF000
# FRIDAY 25TH OF APRIL 2025
**CHIP-8 Debugging & Fixes (April 24–25, 2025)**

---

### 🧵 Session Summary

#### ✅ Prompt: “i cant edit sdlbasecode.c”  
**Response:** Use accessor functions to interact with `static` variables like `buttons` instead of directly referencing or redeclaring them. For `buttons`, use `chip8_register_read(i)` safely.

#### ✅ Prompt: “DT: 0” and font memory not loading  
**Response:** Verified that `chip8_mem_reset()` was clearing out font data due to `memset(mem, 0, 4096)` after loading the ROM. Fix: Re-inject font using `chip8_mem_write()` in `chip8_init()` like so:
```c
for (int i = 0; i < 80; i++) {
    chip8_mem_write(0x050 + i, font[i]);
}
```
Edit:
```c
for(int i = 0; i < 80; i++) {
        chip8_mem_write(0x050 + i, chip8_mem_read(i));
}
```
I tested it and it worked out perfectly so i decided to leave it threre.

#### ✅ Prompt: “so what will happen to the header file”  
**Response:** To access shared variables across files without static conflicts, declare them as `extern` in the header file:
```c
extern uint8_t dt, st;
extern uint8_t buttons[16];
```
EDIT:
Didnt write that again

#### ✅ Prompt: “case 0x29… FX29 called”  
**Response:** Confirmed that `FX29` loads font address using `I = 0x050 + Vx * 5`. Ensure font is placed at 0x050 or the characters won’t show properly on screen.

```c
case 0xF029:
    uint8_t myCharacter = registers[X];
    I = 0x050 + (myCharacter * 5);
    break;
```
Edits:
```c
case 0x29:
    uint8_t myCharacter = registers[X];
    I = 0x050 + (myCharacter * 5);
    break;
```
Packaged it within the 0x7000 again
#### ✅ Prompt: “i cant move… can shoot”  
**Response:** Issue was from conflicting `buttons[]` access. Fixed by using `chip8_register_read(i)` in `FX0A` input-wait opcode.
```c
case 0xF00A:
        bool key_pressed = false;
        for (int i = 0; i < 16; i++) {
            if(buttons[i]) {
                registers[X] = i;
                key_pressed = true;
                break;
            }
        }
        if(!key_pressed) {
            pc -= 2;
        }
        break;
```
Edits:
case 0x0A:
```c
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
```

I packaged it within the 0x7000 since the register would start with register kk which gives the last two values
#### ✅ Prompt: “high score not shown after death”  
**Response:** Likely that high score was using BCD output (`FX33`) and needed fonts to render. Since `chip8_mem_reset()` cleared memory, fonts disappeared. Temporary solution was re-writing font after ROM load:
```c
chip8_mem_reset();
for (int i = 0; i < 80; i++) {
    chip8_mem_write(0x050 + i, font[i]);
}
```
Edit:
``` c
    for(int i = 0; i < 80; i++) {
        chip8_mem_write(0x050 + i, chip8_mem_read(i));
    }
```
---

### Learning Outcomes

1. **Understanding of the CHIP8 Architecture:** Implemented fetch-decode-execute cycle, memory-mapped I/O, and opcode logic.
2. **Memory/Register Management:** Correct use of `mem`, `V0–VF`, `I`, `PC`, and `SP`.
3. **Opcode Instruction Handling:** Implemented `DXYN`, `FX29`, `FX33`, `FX0A`, `EX9E`, `EXA1`, etc.
4. **Debugging and Testing:** Used `printf()` to trace memory for visual bugs and input logic.
5. **SDL Integration:** Connected emulator to graphical output via `chip8_draw_sprite()` from  SDL basecode.
6. **Modular Design under Constraints:** Implemented the chip8 file without modifying the sdlbasecode.c ,I accessed it through calling its functions and linking it together during compilation
7. **Ethical Tool Usage:** Logged ChatGPT usage and maintained academic integrity with detailed prompt-to-edit tracking.

---

###  Opcode & Memory Debug Logs

## Opcode: `DXYN` (Draw Sprite)
```c
printf("DXYN: Drawing sprite from I=0x%03X at (%d,%d), height=%d\n", I, x, y, height);
for (int j = 0; j < height; j++) {
    printf("mem[0x%03X] = 0x%02X\n", I + j, chip8_mem_read(I + j));
}
```
Edits: Removed it after using it to debug
#### 🧮 Opcode: `FX33` (BCD Conversion)
```c
uint8_t val = registers[X];
chip8_mem_write(I, val / 100);
chip8_mem_write(I + 1, (val / 10) % 10);
chip8_mem_write(I + 2, val % 10);
```
```c
Edit:
uint8_t BCD = registers[X];
chip8_mem_write(I,BCD/100);
chip8_mem_write(I + 1, (BCD/10) % 10);
chip8_mem_write(I + 2, BCD % 10);
break;
```
#### 🧮 Font Dump
```c
printf("=== Font memory from 0x050 to 0x09F ===\n");
for (int i = 0x050; i < 0x050 + 80; i++) {
    printf("mem[%03X] = %02X\n", i, chip8_mem_read(i));
}
```
Edits: Removed that after debugging.
#### 🧮 Delay Timer Debug

printf("DT: %d\n", registers[CHIP8_REG_DT]);
```
Edits:
Removed that after testing for debug

# CONCLUSION
Finally I have finished this project and now it is time for me to write comments and to push this github code.

# Reources:
ChatGPT: Helped me with the logic, clarification and debugging. 
Lepuri: https://lepuri.net/making-a-chip-8-emulator-in-c/
Tobias Langoff: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
Cowgod Chip8 Documentation: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#keyboard


