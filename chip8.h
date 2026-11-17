#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef struct {
    uint8_t memoria[4096];
    uint8_t v[16];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint16_t stack[16];
    uint16_t teclado;
    uint8_t pantalla[32][8];
}Chip8;

typedef struct {
    uint8_t familia;
    uint8_t registros[2];
    uint8_t variante;
    uint16_t direccion;
    uint8_t byte;
}Instruccion;

void inicializar(Chip8 *cpu);

void insertProg(uint8_t programa[],int size,Chip8 *cpu);

uint16_t fetch(Chip8 *cpu);

Instruccion decode(uint16_t opcode);

void execute(Instruccion instruccion, Chip8 *cpu);

#endif