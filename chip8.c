#include "chip8.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



void inicializar(Chip8 *cpu) {
    printf("Inicializando CPU...\n");
    memset(cpu,0,sizeof(Chip8));
    cpu->PC = 0x200;
    for (int i = 0; i<80; i++){
        cpu->memoria[i] = fontset[i];
    }
}

uint16_t fetch(Chip8 *cpu){
    uint16_t op = ((uint16_t)cpu->memoria[cpu->PC] << 8) | cpu->memoria[cpu->PC + 1];
    cpu->PC += 2;
    return op;
}

Instruccion decode(uint16_t opcode){
    Instruccion instruccion;
    instruccion.familia = (opcode & 0xF000) >> 12;
    switch (instruccion.familia)
    {
    case 0:
    case 1:
    case 2:
    case 0xA:
    case 0xB:
        instruccion.direccion = (opcode & 0x0FFF);
        break;

    case 3:
    case 4:
    case 6:
    case 7:
    case 0xC:
    case 0xE:
    case 0xF:
        instruccion.registros[0] = (opcode & 0x0F00) >> 8;
        instruccion.byte = (opcode & 0x00FF);
        break;

    case 5:
    case 8:
    case 9:
    case 0xD:
        instruccion.registros[0] = (opcode & 0x0F00) >> 8;
        instruccion.registros[1] = (opcode & 0x00F0) >> 4;
        instruccion.variante = (opcode & 0x000F);
        break;
    
    default:
        printf("OPERATION NOT ACCEPTED");
        break;
    }

    return instruccion; 
}

void execute(Instruccion instruccion, Chip8 *cpu) {
    switch(instruccion.familia)
    {
        case 0:
            switch(instruccion.direccion)
            {
                case 0x0E0://LIMPIAR PANTALLA
                    
                    break;
                
                case 0x0EE://RETURN
                    cpu->SP--;
                    cpu->PC = cpu->stack[cpu->SP];
                    break;
                
                default:
                    printf("INVALID 0x0 INSTRUCTION");

            }
            break;

        case 1://JUMP
            cpu->PC = instruccion.direccion;
            break;
        
        case 2://CALL
            cpu->stack[cpu->SP] = cpu->PC;
            cpu->SP++;
            cpu->PC = instruccion.direccion;
            break;

        case 3://SKIP SI IGUAL reg y var
            if(instruccion.byte == cpu->v[instruccion.registros[0]]) {
                cpu->PC += 2;
            }        
            break;

        case 4://SKIP SI NO IGUAL reg y var
            if(instruccion.byte != cpu->v[instruccion.registros[0]]) {
                cpu->PC += 2;
            } 
            break;

        case 5://SKIP SI IGUAL reg y reg
            if(cpu->v[instruccion.registros[0]] == cpu->v[instruccion.registros[1]]) {
                cpu->PC += 2;
            }        
            break;

        case 6://LOAD reg y var
            cpu->v[instruccion.registros[0]] = instruccion.byte;
            break;

        case 7://ADD reg y var
            cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] + instruccion.byte;
            break;

        case 8://OPS DE REGISTROS reg y reg
            switch(instruccion.variante)
            {
                case 0://LOAD
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[1]];
                    break;

                case 1: //OR
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] | cpu->v[instruccion.registros[1]];
                    break;

                case 2://AND
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] & cpu->v[instruccion.registros[1]];
                    break;

                case 3://XOR
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] ^ cpu->v[instruccion.registros[1]];
                    break;

                case 4://ADD
                        cpu->v[0xF] = (cpu->v[instruccion.registros[0]] + cpu->v[instruccion.registros[1]]) > 255;
                        cpu->v[instruccion.registros[0]] = (cpu->v[instruccion.registros[0]] + cpu->v[instruccion.registros[1]]) & 0xFF;
                    break;

                case 5://SUB
                    if(cpu->v[instruccion.registros[0]] >= cpu->v[instruccion.registros[1]]){
                        cpu->v[0xF] = 1;
                        cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] - cpu->v[instruccion.registros[1]];
                    }
                    else {
                        cpu->v[0xF] = 0;
                        cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] - cpu->v[instruccion.registros[1]];
                    }
                    break;

                case 6://DIV ENTRE 2
                    cpu->v[0xF] = cpu->v[instruccion.registros[0]] & 1;
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] >> 1;
                    break;

                case 7://SUB INVERSO
                    if(cpu->v[instruccion.registros[1]] >= cpu->v[instruccion.registros[0]]){
                        cpu->v[0xF] = 1;
                        cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[1]] - cpu->v[instruccion.registros[0]];
                    }
                    else {
                        cpu->v[0xF] = 0;
                        cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[1]] - cpu->v[instruccion.registros[0]];
                    }
                    break;

                case 0xE://MUL POR 2
                    cpu->v[0xF] = (cpu->v[instruccion.registros[0]] & 0b10000000) >> 7;
                    cpu->v[instruccion.registros[0]] = cpu->v[instruccion.registros[0]] << 1;
                    break;
                
                default:
                    printf("INVALID 0x8 INSTRUCTION");
            }
            
            break;

        case 9://SKIP SI NO IGUAL reg y reg
            if(cpu->v[instruccion.registros[0]] != cpu->v[instruccion.registros[1]]) {
                cpu->PC += 2;
            }    
            break;

        case 0xA:
            cpu->I = instruccion.direccion;
            break;

        case 0xB://JUMP DIR + V0
            cpu->PC = instruccion.direccion + cpu->v[0];
            break;

        case 0xC://RAND rand and var
            cpu->v[instruccion.registros[0]] = ((uint8_t)(rand() % 256)) & instruccion.byte;            
            break;

        case 0xD://DRAW
            
            break;

        case 0xE://SKIPS TECLADO
            switch(instruccion.byte)
            {
                case 0x9E://ESTA PULSADO
                    if(cpu->teclado & (1 << cpu->v[instruccion.registros[0]])){
                        cpu->PC += 2;
                    }
                    break;

                case 0xA1://NO ESTA PULSADO
                    if(!(cpu->teclado & (1 << cpu->v[instruccion.registros[0]]))){
                        cpu->PC += 2;
                    }
                    break;
            }
            break;
            
        case 0xF://TIMERS
            
        
            break;
    }
}