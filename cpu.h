#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>

/* Macros to easily test for value of specific flag */
#define N(registers) (registers->p & 0b10000000)
#define V(registers) (registers->p & 0b01000000)
#define B(registers) (registers->p & 0b00010000)
#define D(registers) (registers->p & 0b00001000)
#define I(registers) (registers->p & 0b00000100)
#define Z(registers) (registers->p & 0b00000010)
#define C(registers) (registers->p & 0b00000001)
/* Macros to easily set specific flags */
#define SET_N(registers) (registers->p |= 0b10000000)
#define SET_V(registers) (registers->p |= 0b01000000)
#define SET_B(registers) (registers->p |= 0b00010000)
#define SET_D(registers) (registers->p |= 0b00001000)
#define SET_I(registers) (registers->p |= 0b00000100)
#define SET_Z(registers) (registers->p |= 0b00000010)
#define SET_C(registers) (registers->p |= 0b00000001)
/* Macros to easily clear specific flags */
#define CLEAR_N(registers) (registers->p &= 0b01111111)
#define CLEAR_V(registers) (registers->p &= 0b10111111)
#define CLEAR_B(registers) (registers->p &= 0b11101111)
#define CLEAR_D(registers) (registers->p &= 0b11110111)
#define CLEAR_I(registers) (registers->p &= 0b11111011)
#define CLEAR_Z(registers) (registers->p &= 0b11111101)
#define CLEAR_C(registers) (registers->p &= 0b11111110)
/* Convert from unsigned to signed, used in relative adressing */
#define SIGNED(byte) ((int8_t) byte)

typedef struct {
        /* Accumulator */
        uint8_t a;
        /* X index */
        uint8_t x;
        /* Y index */
        uint8_t y;
        /* Stack pointer */
        uint8_t sp;
        /* Program counter */
        uint16_t pc;
        /* Processor flags
           Each flag uses one bit, ordered as follows:
           NV-BDIZC
           Where:
           N: negative
           V: overflow
           -: unused bit
           B: break caused interrupt
           D: decimal mode
           I: IRQ disable
           Z: zero
           C: carry
        */
        uint8_t p;
} Registers;

/* Main loop functions */
int execute(FILE *program, uint8_t *ram);
void step(uint8_t opcode, FILE *program, uint8_t *ram, Registers *registers);
/* Adressing modes and memory access */
size_t fpread(void *ptr, size_t size, size_t nmemb, size_t offset,
              FILE *stream); /* Read from file stream at given offset */
uint8_t fetchImmediate(FILE *program, Registers *registers);
uint8_t fetchAbsolute(FILE *program, Registers *registers, uint8_t *ram);
uint8_t fetchZeroPage(FILE *program, Registers *registers, uint8_t *ram);
/* Flag manipulation functions */
void updateNegFlag(uint8_t result, Registers *registers);
void updateZeroFlag(uint8_t result, Registers *registers);
/* Opcode implementations and helpers */
void ADC(uint8_t operand, Registers *registers);
void notImplemented(uint8_t opcode);
void illegalOpcode(uint8_t opcode);
uint8_t binToBCD(uint8_t value);
uint8_t BCDToBin(uint8_t value);

#endif  /* CPU_H */
