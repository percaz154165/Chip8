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

void logTeclado(SDL_Window *win, SDL_Event evento, Chip8 *cpu){
    switch(evento.type) {
                case SDL_QUIT:
                    imprimirPantalla(cpu);
                    SDL_DestroyWindow(win);
                    SDL_Quit();
                    exit(0);
                    break;

                case SDL_KEYDOWN:
                    switch(evento.key.keysym.sym){
                        case SDLK_1:
                            logKey(0x1,1,cpu);
                            break;
                        case SDLK_2:
                            logKey(0x2,1,cpu);
                            break;
                        case SDLK_3:
                            logKey(0x3,1,cpu);
                            break;
                        case SDLK_4:
                            logKey(0xC,1,cpu);
                            break;
                        case SDLK_q:
                            logKey(0x4,1,cpu);       
                            break;
                        case SDLK_w:
                            logKey(0x5,1,cpu);
                            break;
                        case SDLK_e:
                            logKey(0x6,1,cpu);
                            break;
                        case SDLK_r:
                            logKey(0xD,1,cpu);
                            break;
                        case SDLK_a:
                            logKey(0x7,1,cpu);
                            break;
                        case SDLK_s:
                            logKey(0x8,1,cpu);
                            break;
                        case SDLK_d:
                            logKey(0x9,1,cpu);
                            break;
                        case SDLK_f:
                            logKey(0xE,1,cpu);
                            break;
                        case SDLK_z:
                            logKey(0xA,1,cpu);
                            break;
                        case SDLK_x:
                            logKey(0x0,1,cpu);
                            break;      
                        case SDLK_c:
                            logKey(0xB,1,cpu);
                            break;
                        case SDLK_v:
                            logKey(0xF,1,cpu);
                            break;
                    }
                    break;
                
                case SDL_KEYUP:
                    switch(evento.key.keysym.sym){
                        case SDLK_1:
                            logKey(0x1,0,cpu);
                            break;
                        case SDLK_2:
                            logKey(0x2,0,cpu);
                            break;
                        case SDLK_3:
                            logKey(0x3,0,cpu);
                            break;
                        case SDLK_4:
                            logKey(0xC,0,cpu);
                            break;
                        case SDLK_q:
                            logKey(0x4,0,cpu);       
                            break;
                        case SDLK_w:
                            logKey(0x5,0,cpu);
                            break;
                        case SDLK_e:
                            logKey(0x6,0,cpu);
                            break;
                        case SDLK_r:
                            logKey(0xD,0,cpu);
                            break;
                        case SDLK_a:
                            logKey(0x7,0,cpu);
                            break;
                        case SDLK_s:
                            logKey(0x8,0,cpu);
                            break;
                        case SDLK_d:
                            logKey(0x9,0,cpu);
                            break;
                        case SDLK_f:
                            logKey(0xE,0,cpu);
                            break;
                        case SDLK_z:
                            logKey(0xA,0,cpu);
                            break;
                        case SDLK_x:
                            logKey(0x0,0,cpu);
                            break;      
                        case SDLK_c:
                            logKey(0xB,0,cpu);
                            break;
                        case SDLK_v:
                            logKey(0xF,0,cpu);
                            break;
                    }
                    break;
                    
            }
}

void beep_callback(void *userdata, Uint8 *stream, int len) {
    (void)userdata;
    const int sample_rate = 44100;
    const int freq = 480; // Tono clásico de onda cuadrada
    const int half_period = sample_rate / (2 * freq);
    const Sint16 amplitude = 5000; // Volumen moderado para evitar clipping

    static int phase = 0;
    Sint16 *out = (Sint16 *)stream;
    int samples = len / (int)sizeof(Sint16);

    for (int i = 0; i < samples; i++) {
        out[i] = ((phase / half_period) % 2 == 0) ? amplitude : -amplitude;
        phase = (phase + 1) % (half_period * 2);
    }
}

int main(int argc, char* argv[]) {
    int size = 0;
    uint8_t *buffer = NULL;
    if (argc == 2) {
        FILE *f = fopen(argv[1],"rb");
        fseek(f,0L,SEEK_END);
        size = ftell(f);
        rewind(f);

        if(size > (4096 - 0x200)){
            printf("ARCHIVO DEMASIADO GRANDE PARA LA MEMORIA !!!");
            return 1;
        }

        buffer = malloc(size);
        if(buffer == NULL){
            printf("ERROR LEYENDO EL PROGRAMA (malloc error) !!!");
        }

        fread(buffer, sizeof(uint8_t),size/sizeof(uint8_t),f);
        fclose(f);
    }

    else if(argc < 2){
        unsigned int val;
        printf("INGRESA EL PROGRAMA EN HEXADECIMAL, SEPARANDO CADA BYTE POR ESPACIOS (Ctrl+D PARA TERMINAR): \n");
        while(size < (4096 - 0x200) && scanf("%x",&val) == 1){
            buffer = realloc(buffer,size+1);
            buffer[size] = (uint8_t)(val & 0xFF);
            size++;
        }
    }
    else{
        printf("USO: %s <ruta programa>",argv[0]);
        return 1;
    }


    srand((unsigned int)time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("TEST",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_AudioSpec spec = {
        .freq = 44100,
        .format = AUDIO_S16SYS,
        .channels = 1,
        .samples = 1024,
        .callback = beep_callback,
        .userdata = NULL
    };
    SDL_AudioDeviceID audio_dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    SDL_PauseAudioDevice(audio_dev, 1);

    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);
    Chip8 cpu;
    inicializar(&cpu);
    insertProg(buffer, size, &cpu);
    uint16_t op;
    Instruccion inst;
    SDL_Rect rect;
    SDL_Event evento;
    uint32_t ultTTick = SDL_GetTicks(); //ultimo Timer Tick
    uint32_t ultTick = SDL_GetTicks(); //ultimo Tick
    uint32_t ultFTick = SDL_GetTicks(); //ultimo Frame Tick



    while(1) {
        // printf("PRESIONA ESPACIO PA SEGUIR: \n");
        // getchar();
        if(SDL_GetTicks() - ultTick >= 1) { //CICLO DE CPU (1/xHz)
            while(SDL_PollEvent(&evento)) {
                logTeclado(win,evento,&cpu);
            }

            op = fetch(&cpu);
            inst = decode(op);
            execute(inst, &cpu);  

            ultTick = SDL_GetTicks();
        }
        if(SDL_GetTicks() - ultFTick >= 16){ //FRAMERATE (1/xHz)

            dibujarFrame(ren,&cpu,rect); 
            ultFTick = SDL_GetTicks();
        }
        if(SDL_GetTicks() - ultTTick >= 16){ //TIMER (1/60Hz)    

        
            if(cpu.DT > 0){
                cpu.DT -= 1;
            }
            if (cpu.ST > 0){
                SDL_PauseAudioDevice(audio_dev, 0);
                cpu.ST -= 1;
            }
            else {
                SDL_PauseAudioDevice(audio_dev, 1);
            }
            ultTTick = SDL_GetTicks();
        }
        
    }
    imprimirPantalla(&cpu);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

