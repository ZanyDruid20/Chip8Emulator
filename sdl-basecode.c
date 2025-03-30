#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <SDL/SDL.h>
//#include "C:/msys64/home/Furnom Dam/chip8Emulator/project-ZanyDruid20/chip8.h"


/* You must implement these functions in your code! */
void chip8_init(void);
void chip8_reset(void);
void chip8_shutdown(void);
void chip8_execute_instruction(void);

/* These functions are available to you to use in your code. */
uint8_t chip8_mem_read(uint16_t addr);
void chip8_mem_write(uint16_t addr, uint8_t val);
uint8_t chip8_register_read(uint8_t reg);
void chip8_register_write(uint8_t reg, uint8_t val);
void chip8_clear_frame(void);
void chip8_mem_clear(void);
int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height);
void chip8_mem_reset(void);

/* Key statuses are defined as registers 0x00-0x0F */
#define CHIP8_REG_DT        0x10
#define CHIP8_REG_ST        0x11

/* Sound parameters. Feel free to adjust these to your taste.
   BEEP_FREQUENCY - Sine wave frequency for the generated sound. This changes
                    the tone that is played.
   BEEP_AMPLITUDE - Sine wave amplitude. This changes the volume, basically.
                    Don't set above 32767, as sound samples are signed 16-bit
                    integers, and using amplitudes above this will cause
                    clipping artifacts. */
#define BEEP_FREQUENCY 400
#define BEEP_AMPLITUDE 28000

/* No user-serviceable parts below. Do not modify anything below this line. */
/* Traditional COSMAC VIP keymap */
static const uint8_t keymap_cosmac[16] = {
    0x01, 0x02, 0x03, 0x0c,
    0x04, 0x05, 0x06, 0x0d,
    0x07, 0x08, 0x09, 0x0e,
    0x0a, 0x00, 0x0b, 0x0f
};

/* DREAM 6800/ETI-660 keymap */
static const uint8_t keymap_dream[16] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f
};

#define PIXEL_SET 0xffffffff
#define MEMORY_SIZE 4096
#define MEMORY_MASK (MEMORY_SIZE - 1)

static uint8_t mem[MEMORY_SIZE];
static uint32_t framebuffer[64 * 32];
static uint8_t buttons[16];
static uint8_t dt, st;
static uint8_t keymap[16];
static char *romfn;

static SDL_Surface *screen, *fbsurf;

static const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

uint8_t chip8_mem_read(uint16_t addr) {
    return mem[addr & MEMORY_MASK];
}

void chip8_mem_write(uint16_t addr, uint8_t val) {
    mem[addr & MEMORY_MASK] = val;
}

uint8_t chip8_register_read(uint8_t reg) {
    if(reg < CHIP8_REG_DT)
        return buttons[reg];
    else if(reg == CHIP8_REG_DT)
        return dt;
    else if(reg == CHIP8_REG_ST)
        return st;

    return 0;
}

void chip8_register_write(uint8_t reg, uint8_t val) {
    if(reg == CHIP8_REG_DT)
        dt = val;
    else if(reg == CHIP8_REG_ST)
        st = val;
}

void chip8_clear_frame(void) {
    memset(framebuffer, 0, 64 * 32 * sizeof(uint32_t));
}

void chip8_mem_clear(void) {
    memset(mem, 0, MEMORY_SIZE);
}

int chip8_draw_sprite(uint16_t addr, uint8_t x, uint8_t y, uint8_t height) {
    uint8_t i, j, bp;
    int rv = 0;
    int ptr;

    y &= 0x1F;
    x &= 0x3F;
    ptr = (y << 6) + x;

    if(height + y > 32)
        height = 32 - y;

    for(i = 0; i < height; ++i, ptr += 64) {
        bp = chip8_mem_read(addr + i);

        for(j = 0; j < 8 && x + j < 64; ++j) {
            if((bp & (0x80 >> j))) {
                framebuffer[ptr + j] ^= PIXEL_SET;

                if(!framebuffer[ptr + j])
                    rv = 1;
            }
        }
    }

    return rv;
}

static int chip8_load_rom(const char *fn) {
    FILE *fp;
    int size;

    chip8_mem_clear();

    if(!(fp = fopen(fn, "rb"))) {
        fprintf(stderr, "Could not open ROM: %s\n", fn);
        return -1;
    }

    /* Determine the size of the file */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if(size > MEMORY_SIZE - 0x200) {
        fprintf(stderr, "ROM size too large, bailing out!\n");
        return -1;
    }

    if(fread(mem + 0x200, 1, size, fp) != size) {
        fprintf(stderr, "Could not read ROM!\n");
        return -1;
    }

    fclose(fp);

    memcpy(mem, font, 80);
    memset(buttons, 0, 16);
    chip8_clear_frame();

    if(!romfn)
        romfn = strdup(fn);

    return 0;
}

void chip8_mem_reset(void) {
    memset(mem, 0, 4096);
    chip8_load_rom(romfn);
    dt = 0;
    st = 0;
}

static void draw_framebuffer(void) {
    int i, j, ii;
    uint32_t *px = (uint32_t *)screen->pixels;

    for(i = 0; i < 320; ++i) {
        ii = i / 10;

        for(j = 0; j < 640; ++j) {
            px[i * 640 + j] = framebuffer[(ii * 64) + (j / 10)] | 0xff000000;
        }
    }
}

static void chip8_frame(void) {
    int i;
    SDL_Rect rect;

    for(i = 0; i < 12; ++i) {
        chip8_execute_instruction();
    }

    if(dt)
        --dt;

    if(st) {
        --st;
    }

    SDL_LockSurface(screen);
    draw_framebuffer();
    SDL_UnlockSurface(screen);
    SDL_Flip(screen);
    SDL_Delay(15);
}

/* Sound stuff... yay. */
#define SAMPLE_RATE 44100

/* This is good enough to play a simple beep while the sound timer says to do
   so. Thankfully, that's all we have to do for chip8. This makes a nice simple
   sine wave for a pleasant sounding beep, anyway. Could this be more efficient?
   sure. Does it need to be? No. */
void fill_audio(void *udata, Uint8 *stream, int len) {
    static double phase = 0.0;
    double increment = 2.0 * M_PI * BEEP_FREQUENCY / SAMPLE_RATE;
    Sint16 *buffer = (Sint16 *)stream;
    int length = len >> 1, i;

    for(i = 0; i < length; ++i) {
        if(st) {
            buffer[i] = (Sint16)(BEEP_AMPLITUDE * sin(phase));
            phase += increment;

            if(phase >= 2.0 * M_PI)
                phase -= 2.0 * M_PI;
        }
        else {
            buffer[i] = 0;
            phase = 0;
        }
    }
}

#ifdef DEBUG_BASECODE
#include "chip8cpu.h"

struct Chip8CPU_struct cpu;

void chip8_execute_instruction(void) {
    Chip8CPU_execute(&cpu);
}

void chip8_reset(void) {
    Chip8CPU_reset(&cpu);
    chip8_mem_reset();
}

static uint8_t _c8_mr(void *cxt, uint16_t addr) {
    (void)cxt;
    return chip8_mem_read(addr);
}

static uint16_t _c8_mr16(void *cxt, uint16_t addr) {
    (void)cxt;
    return (mem[addr & 0xFFF] << 8) | mem[(addr + 1) & 0xFFF];
}

static void _c8_mw(void *cxt, uint16_t addr, uint8_t val) {
    (void)cxt;
    chip8_mem_write(addr, val);
}

static uint8_t _c8_rr(void *cxt, uint8_t reg) {
    (void)cxt;
    return chip8_register_read(reg);
}

static void _c8_rw(void *cxt, uint8_t reg, uint8_t val) {
    (void)cxt;
    chip8_register_write(reg, val);
}

static void _c8_cfb(void *cxt) {
    (void)cxt;
    chip8_clear_frame();
}

static int _c8_spr(void *cxt, uint16_t addr, uint8_t x, uint8_t y, uint8_t h) {
    (void)cxt;
    return chip8_draw_sprite(addr, x, y, h);
}

void chip8_init(void) {
    Chip8CPU_init(&cpu);
    Chip8CPU_set_memread(&cpu, &_c8_mr);
    Chip8CPU_set_memread16(&cpu, &_c8_mr16);
    Chip8CPU_set_memwrite(&cpu, &_c8_mw);
    Chip8CPU_set_regread(&cpu, &_c8_rr);
    Chip8CPU_set_regwrite(&cpu, &_c8_rw);
    Chip8CPU_set_clearfb(&cpu, &_c8_cfb);
    Chip8CPU_set_drawspr(&cpu, &_c8_spr);
}

void chip8_shutdown(void) {
}
#endif

static int handle_keypress(SDLKey key, int pressed) {
    static int in_reset;

    switch(key) {
        case SDLK_ESCAPE:
            if(pressed && !in_reset) {
                chip8_reset();
                in_reset = 1;
            }
            else if(!pressed) {
                in_reset = 0;
            }
            break;

        case SDLK_1:
            buttons[keymap[0]] = pressed;
            break;

        case SDLK_2:
            buttons[keymap[1]] = pressed;
            break;

        case SDLK_3:
            buttons[keymap[2]] = pressed;
            break;

        case SDLK_4:
            buttons[keymap[3]] = pressed;
            break;

        case SDLK_q:
            buttons[keymap[4]] = pressed;
            break;

        case SDLK_w:
            buttons[keymap[5]] = pressed;
            break;

        case SDLK_e:
            buttons[keymap[6]] = pressed;
            break;

        case SDLK_r:
            buttons[keymap[7]] = pressed;
            break;

        case SDLK_a:
            buttons[keymap[8]] = pressed;
            break;

        case SDLK_s:
            buttons[keymap[9]] = pressed;
            break;

        case SDLK_d:
            buttons[keymap[10]] = pressed;
            break;

        case SDLK_f:
            buttons[keymap[11]] = pressed;
            break;

        case SDLK_z:
            buttons[keymap[12]] = pressed;
            break;

        case SDLK_x:
            buttons[keymap[13]] = pressed;
            break;

        case SDLK_c:
            buttons[keymap[14]] = pressed;
            break;

        case SDLK_v:
            buttons[keymap[15]] = pressed;
            break;
    }
}

int main(int argc, char *argv[]) {
    SDL_Event ev;
    int running = 1, i, mute = 0;
    SDL_AudioSpec audio_desired;
    SDL_AudioSpec audio_obtained;

    if(argc < 2) {
        printf("Usage: %s [options] romfilename\n", argv[0]);
        printf("Options:\n");
        printf("  -k    - Use alternate keymap\n");
        printf("  -m    - Mute sounds\n");

        return 1;
    }

    memcpy(keymap, keymap_cosmac, 16);

    if(argc > 2) {
        for(i = 1; i < argc - 1; ++i) {
            if(!strcmp(argv[i], "-k")) {
                printf("Using alternate keymap\n");
                memcpy(keymap, keymap_dream, 16);
            }
            else if(!strcmp(argv[i], "-m")) {
                printf("Muting sound\n");
                mute = 1;
            }
            else {
                fprintf(stderr, "Unknown option %s, ignoring\n", argv[i]);
            }
        }
    }

    /* Set up the chip8 emulation */
    if(chip8_load_rom(argv[argc - 1]))
        return 1;

    chip8_init();
    chip8_reset();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    /* Set up video */
    if(!(screen = SDL_SetVideoMode(640, 320, 32, SDL_SWSURFACE))) {
        fprintf(stderr, "Failed to set up SDL video mode, exiting\n");
        return 1;
    }

    SDL_WM_SetCaption("CMSC 411 CHIP-8", NULL);

    if(!mute) {
        /* Set up sound */
        audio_desired.freq = SAMPLE_RATE;
        audio_desired.format = AUDIO_S16SYS;
        audio_desired.channels = 1;
        audio_desired.samples = 2048;
        audio_desired.callback = &fill_audio;

        if(SDL_OpenAudio(&audio_desired, &audio_obtained) < 0) {
            fprintf(stderr, "Failed to set up audio: %s\n", SDL_GetError());
            return -1;
        }

        SDL_PauseAudio(0);
    }

    /* Go! */
    while(running) {
        chip8_frame();

        /* Handle any events we've gotten */
        while(SDL_PollEvent(&ev)) {
            if(ev.type == SDL_KEYDOWN) {
                handle_keypress(ev.key.keysym.sym, 1);
            }
            else if(ev.type == SDL_KEYUP) {
                handle_keypress(ev.key.keysym.sym, 0);
            }
            else if(ev.type == SDL_QUIT) {
                printf("Got quit signal, exiting.\n");
                running = 0;
                break;
            }
        }
    }

    chip8_shutdown();

    if(!mute)
        SDL_CloseAudio();

    SDL_Quit();
    return 0;
}