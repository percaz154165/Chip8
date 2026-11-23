# CHIP-8 Emulator

Emulador de CHIP-8 escrito en C utilizando SDL2 para la salida gráfica, el audio y la gestión de entrada. Soporta carga de ROMs desde archivo binario e introducción manual de bytes hexadecimales por consola.

## Características

- **CPU y memoria:** Decodificación de instrucciones, memoria de 4096 bytes con carga desde `0x200` y temporizadores (`delay` y `sound`) sincronizados a 60 Hz.
- **Gráficos:** *Framebuffer* monocromo de `64x32` píxeles con soporte para sprites mediante `DXYN`, operaciones `XOR` y detección de colisión en `VF`, escalado y renderizado con SDL2.
- **Audio:** Generación de onda cuadrada de 480 Hz mediante SDL2 mientras el temporizador de sonido está activo.
- **Entrada:** Mapeo de teclado para los controles del CHIP-8 y carga de programas tanto desde fichero como por entrada estándar (`scanf`) con redimensión dinámica (`realloc`).

## Requisitos

- Compilador de C compatible con C11 (`gcc` o `clang`).
- Librerías de desarrollo de **SDL2** (`libsdl2-dev` en sistemas basados en Debian/Ubuntu).

## Compilación

En Linux/macOS con `gcc` y `pkg-config`:

```bash
gcc main.c chip8.c -o chip8_emu $(sdl2-config --cflags --libs)
```


## Uso

### Cargar una ROM desde archivo

```bash
./chip8 <ruta a la rom .ch8>
```

### Introducir bytes hexadecimales por consola

Si ejecutas el programa en el modo de entrada manual, introduce los bytes en hexadecimal sin el prefijo `0x` (por ejemplo, separados por espacios o saltos de línea):

```text
00 e0 a2 2a 60 0c 61 08 d0 1f 70 09 a2 39 d0 1f a2 48 70 08
```

Finaliza la entrada enviando `EOF` (`Ctrl+D` en Linux/macOS o `Ctrl+Z` seguido de `Enter` en Windows) para cargar los bytes en memoria y ejecutar el programa.

## Estado del proyecto

Proyecto completado y funcional. Cuenta con ciclo de CPU independiente, redibujado sincronizado a 60Hz, audio por SDL2, carga desde archivo y entrada manual, verificado ejecutando ROMs clásicas como *Space Invaders*, *Pong*...