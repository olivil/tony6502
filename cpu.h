#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>

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

int execute(FILE *program, uint8_t *ram);
void step(uint8_t opcode, FILE* program, uint8_t *ram);
void notImplemented(uint8_t opcode);
void illegalOpcode(uint8_t opcode);

#endif  /* CPU_H */
