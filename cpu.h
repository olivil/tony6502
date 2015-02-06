#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>

int execute(FILE *program, uint8_t *ram, int ramSize);


#endif  /* CPU_H */
