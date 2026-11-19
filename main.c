#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "chip8.h"
#include <time.h>

void imprimirPantalla(Chip8 *cpu){
    for(int i = 0; i < 32; i++){
        for (int j = 0; j < 8; j++){
            for (int bit = 7; bit >= 0; bit--){
                uint8_t pixel = (cpu->pantalla[i][j] >> bit) & 1;
                putchar(pixel ? '.' : ' ');
            }
        }
        putchar('\n');
    }
}

void dibujarFrame(SDL_Renderer *ren,Chip8 *cpu,SDL_Rect rect){
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren,0,255,0,255);

    for(int i = 0; i < 32; i++){
        for (int j = 0; j < 8; j++){
            for (int bit = 7; bit >= 0; bit--){
                if((cpu->pantalla[i][j] >> bit) & 1){
                    rect.x = (j*8+(7-bit))*10;
                    rect.y = i*10;
                    rect.w = 10;
                    rect.h = 10;
                    SDL_RenderFillRect(ren,&rect);
                }
            }
        }
    }
    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[]) {
    uint8_t programa1[] = {
    0x00, 0xe0, 0xa2, 0x2a, 0x60, 0x0c, 0x61, 0x08, 0xd0, 0x1f, 0x70, 0x09, 0xa2, 0x39, 0xd0, 0x1f,
    0xa2, 0x48, 0x70, 0x08, 0xd0, 0x1f, 0x70, 0x04, 0xa2, 0x57, 0xd0, 0x1f, 0x70, 0x08, 0xa2, 0x66,
    0xd0, 0x1f, 0x70, 0x08, 0xa2, 0x75, 0xd0, 0x1f, 0x12, 0x28, 0xff, 0x00, 0xff, 0x00, 0x3c, 0x00,
    0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00, 0x38, 0x00, 0x3f,
    0x00, 0x3f, 0x00, 0x38, 0x00, 0xff, 0x00, 0xff, 0x80, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x80, 0x00,
    0x80, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x80, 0xf8, 0x00, 0xfc, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x3b,
    0x00, 0x39, 0x00, 0xf8, 0x00, 0xf8, 0x03, 0x00, 0x07, 0x00, 0x0f, 0x00, 0xbf, 0x00, 0xfb, 0x00,
    0xf3, 0x00, 0xe3, 0x00, 0x43, 0xe0, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
    0x00,0xe0, 0x00, 0xe0, 0x00
};
uint8_t programa2[] = {
0x12, 0x4e, 0x08, 0x19, 0x01, 0x01, 0x08, 0x01, 0x0f, 0x01, 0x01, 0x09, 0x08, 0x09, 0x0f, 0x09, 0x01, 0x11, 0x08, 0x11, 0x0f, 0x11, 0x01, 0x19, 0x0f, 0x19, 0x16, 0x01, 0x16, 0x09, 0x16, 0x11, 0x16, 0x19, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0xa2, 0x02, 0x82, 0x0e, 0xf2, 0x1e, 0x82, 0x06, 0xf1, 0x65, 0x00, 0xee, 0xa2, 0x02, 0x82, 0x0e, 0xf2, 0x1e, 0x82, 0x06, 0xf1, 0x55, 0x00, 0xee, 0x6f, 0x10, 0xff, 0x15, 0xff, 0x07, 0x3f, 0x00, 0x12, 0x46, 0x00, 0xee, 0x00, 0xe0, 0x62, 0x00, 0x22, 0x2a, 0xf2, 0x29, 0xd0, 0x15, 0x70, 0xff, 0x71, 0xff, 0x22, 0x36, 0x72, 0x01, 0x32, 0x10, 0x12, 0x52, 0xf2, 0x0a, 0x22, 0x2a, 0xa2, 0x22, 0xd0, 0x17, 0x22, 0x42, 0xd0, 0x17, 0x12, 0x64
};
    int size = sizeof(programa2);

    srand((unsigned int)time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("TEST",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);
    Chip8 cpu;
    inicializar(&cpu);
    insertProg(programa2, size, &cpu);
    uint16_t op;
    Instruccion inst;
    SDL_Rect rect;
    SDL_Event evento;


    while(1) {

        // printf("PRESIONA ESPACIO PA SEGUIR: \n");
        // getchar();

        while(SDL_PollEvent(&evento)) {
            switch(evento.type) {
                case SDL_QUIT:
                    imprimirPantalla(&cpu);
                    SDL_DestroyWindow(win);
                    SDL_Quit();
                    exit(0);
                    break;

                case SDL_KEYDOWN:
                    switch(evento.key.keysym.sym){
                        case SDLK_1:
                            logKey(0x1,1,&cpu);
                            break;
                        case SDLK_2:
                            logKey(0x2,1,&cpu);
                            break;
                        case SDLK_3:
                            logKey(0x3,1,&cpu);
                            break;
                        case SDLK_4:
                            logKey(0xC,1,&cpu);
                            break;
                        case SDLK_q:
                            logKey(0x4,1,&cpu);       
                            break;
                        case SDLK_w:
                            logKey(0x5,1,&cpu);
                            break;
                        case SDLK_e:
                            logKey(0x6,1,&cpu);
                            break;
                        case SDLK_r:
                            logKey(0xD,1,&cpu);
                            break;
                        case SDLK_a:
                            logKey(0x7,1,&cpu);
                            break;
                        case SDLK_s:
                            logKey(0x8,1,&cpu);
                            break;
                        case SDLK_d:
                            logKey(0x9,1,&cpu);
                            break;
                        case SDLK_f:
                            logKey(0xE,1,&cpu);
                            break;
                        case SDLK_z:
                            logKey(0xA,1,&cpu);
                            break;
                        case SDLK_x:
                            logKey(0x0,1,&cpu);
                            break;      
                        case SDLK_c:
                            logKey(0xB,1,&cpu);
                            break;
                        case SDLK_v:
                            logKey(0xF,1,&cpu);
                            break;
                    }
                    break;
                
                case SDL_KEYUP:
                    printf("KEYUP DETECTADO: %d\n", evento.key.keysym.sym);
                    switch(evento.key.keysym.sym){
                        case SDLK_1:
                            logKey(0x1,0,&cpu);
                            break;
                        case SDLK_2:
                            logKey(0x2,0,&cpu);
                            break;
                        case SDLK_3:
                            logKey(0x3,0,&cpu);
                            break;
                        case SDLK_4:
                            logKey(0xC,0,&cpu);
                            break;
                        case SDLK_q:
                            logKey(0x4,0,&cpu);       
                            break;
                        case SDLK_w:
                            logKey(0x5,0,&cpu);
                            break;
                        case SDLK_e:
                            logKey(0x6,0,&cpu);
                            break;
                        case SDLK_r:
                            logKey(0xD,0,&cpu);
                            break;
                        case SDLK_a:
                            logKey(0x7,0,&cpu);
                            break;
                        case SDLK_s:
                            logKey(0x8,0,&cpu);
                            break;
                        case SDLK_d:
                            logKey(0x9,0,&cpu);
                            break;
                        case SDLK_f:
                            logKey(0xE,0,&cpu);
                            break;
                        case SDLK_z:
                            logKey(0xA,0,&cpu);
                            break;
                        case SDLK_x:
                            logKey(0x0,0,&cpu);
                            break;      
                        case SDLK_c:
                            logKey(0xB,0,&cpu);
                            break;
                        case SDLK_v:
                            logKey(0xF,0,&cpu);
                            break;
                    }
                    break;
                    
            }
        }

        op = fetch(&cpu);
        inst = decode(op);
        execute(inst, &cpu);
        dibujarFrame(ren,&cpu,rect);   
         
    }
    imprimirPantalla(&cpu);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

