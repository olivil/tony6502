#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "cpu.h"

#define RAM_SIZE 0xFFFF

int main(int argc, char** argv) {
        uint8_t ram[RAM_SIZE];
        FILE *program;

        if(argc != 2) {
                printf("Usage: tony6502 <path/to/program>\n");
                return -1;
        }

        program = fopen(argv[1], "rb");

        if(program == NULL) {
                printf("No such file\n");
                return -ENOENT;
        }

        execute(program, ram, RAM_SIZE);

        return 0;
}
