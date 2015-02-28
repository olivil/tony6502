#include "cpu.h"

int execute(FILE *program, uint8_t *ram) {
        Registers registers;
        uint8_t opcode;

        /* Set initial state of registers.
           In a real 6502, all but the bits 1 to 5 of the p register
           are software defined, but we have to give them some value
           here. */
        registers.a = 0x00;
        registers.x = 0x00;
        registers.y = 0x00;
        registers.sp = 0xFF;
        registers.pc = 0x0000;
        registers.p = 0b00110100;

        while(fread(&opcode, 1, 1, program)) {
                step(opcode, program, ram, &registers);
        }

        return 0;
}

void step(uint8_t opcode, FILE* program, uint8_t *ram, Registers *registers) {
    uint8_t operand;

        switch(opcode) {
        case 0x00:
                notImplemented(opcode);
                break;
        case 0x01:
                notImplemented(opcode);
                break;
        case 0x04:
                notImplemented(opcode);
                break;
        case 0x05:
                notImplemented(opcode);
                break;
        case 0x06:
                notImplemented(opcode);
                break;
        case 0x08:
                notImplemented(opcode);
                break;
        case 0x09:                                                  /* ORA # */
                /* we read operand */
                operand = readByte(program, registers->pc);
                /* bitwise OR between RAM and A */
                registers->a = (registers->a & ram[operand]);
                /* set/clear the negative flag */
                setNegFlag(registers->a, registers);
                /* set/clear the zero flag */
				setZeroFlag(registers->a, registers);
                break;
        case 0x0A:
                notImplemented(opcode);
                break;
        case 0x0C:
                notImplemented(opcode);
                break;
        case 0x0D:
                notImplemented(opcode);
                break;
        case 0x0E:
                notImplemented(opcode);
                break;
        case 0x10:                                                    /* BPL */
                /* Check if negative flag is clear */
                if(!(registers->p & 0b10000000)) {
                    /* we read operand */
                    operand = readByte(program, registers->pc);
                    /* if negative we decrease PC */
                    if(operand & 0b10000000) {
                        registers->pc -= (~operand + 1);
                    }
                    /* if positive we increment PC */
                    else {
                        registers->pc += operand;
                    }
                }
                /* else we skip operand */
                else {
                    registers->pc += 1;
                }
                break;
        case 0x11:
                notImplemented(opcode);
                break;
        case 0x12:
                notImplemented(opcode);
                break;
        case 0x14:
                notImplemented(opcode);
                break;
        case 0x15:
                notImplemented(opcode);
                break;
        case 0x16:
                notImplemented(opcode);
                break;
        case 0x18:
                notImplemented(opcode);
                break;
        case 0x19:
                notImplemented(opcode);
                break;
        case 0x1A:
                notImplemented(opcode);
                break;
        case 0x1C:
                notImplemented(opcode);
                break;
        case 0x1D:
                notImplemented(opcode);
                break;
        case 0x1E:
                notImplemented(opcode);
                break;
        case 0x20:
                notImplemented(opcode);
                break;
        case 0x21:
                notImplemented(opcode);
                break;
        case 0x24:
                notImplemented(opcode);
                break;
        case 0x25:
                notImplemented(opcode);
                break;
        case 0x26:
                notImplemented(opcode);
                break;
        case 0x28:
                notImplemented(opcode);
                break;
        case 0x29:
                notImplemented(opcode);
                break;
        case 0x2A:
                notImplemented(opcode);
                break;
        case 0x2C:
                notImplemented(opcode);
                break;
        case 0x2D:
                notImplemented(opcode);
                break;
        case 0x2E:
                notImplemented(opcode);
                break;
        case 0x30:                                                    /* BMI */
                /* Check if negative flag is set */
                if(registers->p & 0b10000000) {
                    /* we read operand */
                    operand = readByte(program, registers->pc);
                    /* if negative we decrease PC */
                    if(operand & 0b10000000) {
                        registers->pc -= (~operand + 1);
                    }
                    /* if positive we increment PC */
                    else {
                        registers->pc += operand;
                    }
                }
                /* else we skip operand */
                else {
                    registers->pc += 1;
                }
                break;
        case 0x31:
                notImplemented(opcode);
                break;
        case 0x32:
                notImplemented(opcode);
                break;
        case 0x34:
                notImplemented(opcode);
                break;
        case 0x35:
                notImplemented(opcode);
                break;
        case 0x36:
                notImplemented(opcode);
                break;
        case 0x38:
                notImplemented(opcode);
                break;
        case 0x39:
                notImplemented(opcode);
                break;
        case 0x3A:
                notImplemented(opcode);
                break;
        case 0x3C:
                notImplemented(opcode);
                break;
        case 0x3D:
                notImplemented(opcode);
                break;
        case 0x3E:
                notImplemented(opcode);
                break;
        case 0x40:
                notImplemented(opcode);
                break;
        case 0x41:
                notImplemented(opcode);
                break;
        case 0x45:
                notImplemented(opcode);
                break;
        case 0x46:
                notImplemented(opcode);
                break;
        case 0x48:
                notImplemented(opcode);
                break;
        case 0x49:
                notImplemented(opcode);
                break;
        case 0x4A:
                notImplemented(opcode);
                break;
        case 0x4C:
                notImplemented(opcode);
                break;
        case 0x4D:
                notImplemented(opcode);
                break;
        case 0x4E:
                notImplemented(opcode);
                break;
        case 0x50:
                notImplemented(opcode);
                break;
        case 0x51:
                notImplemented(opcode);
                break;
        case 0x52:
                notImplemented(opcode);
                break;
        case 0x55:
                notImplemented(opcode);
                break;
        case 0x56:
                notImplemented(opcode);
                break;
        case 0x58:
                notImplemented(opcode);
                break;
        case 0x59:
                notImplemented(opcode);
                break;
        case 0x5A:
                notImplemented(opcode);
                break;
        case 0x5D:
                notImplemented(opcode);
                break;
        case 0x5E:
                notImplemented(opcode);
                break;
        case 0x60:
                notImplemented(opcode);
                break;
        case 0x61:
                notImplemented(opcode);
                break;
        case 0x64:
                notImplemented(opcode);
                break;
        case 0x65:
                notImplemented(opcode);
                break;
        case 0x66:
                notImplemented(opcode);
                break;
        case 0x68:
                notImplemented(opcode);
                break;
        case 0x69:
                notImplemented(opcode);
                break;
        case 0x6A:
                notImplemented(opcode);
                break;
        case 0x6C:
                notImplemented(opcode);
                break;
        case 0x6D:
                notImplemented(opcode);
                break;
        case 0x6E:
                notImplemented(opcode);
                break;
        case 0x70:
                notImplemented(opcode);
                break;
        case 0x71:
                notImplemented(opcode);
                break;
        case 0x72:
                notImplemented(opcode);
                break;
        case 0x74:
                notImplemented(opcode);
                break;
        case 0x75:
                notImplemented(opcode);
                break;
        case 0x76:
                notImplemented(opcode);
                break;
        case 0x78:
                notImplemented(opcode);
                break;
        case 0x79:
                notImplemented(opcode);
                break;
        case 0x7A:
                notImplemented(opcode);
                break;
        case 0x7C:
                notImplemented(opcode);
                break;
        case 0x7D:
                notImplemented(opcode);
                break;
        case 0x7E:
                notImplemented(opcode);
                break;
        case 0x80:
                notImplemented(opcode);
                break;
        case 0x81:
                notImplemented(opcode);
                break;
        case 0x84:
                notImplemented(opcode);
                break;
        case 0x85:
                notImplemented(opcode);
                break;
        case 0x86:
                notImplemented(opcode);
                break;
        case 0x88:
                notImplemented(opcode);
                break;
        case 0x89:
                notImplemented(opcode);
                break;
        case 0x8A:
                notImplemented(opcode);
                break;
        case 0x8C:
                notImplemented(opcode);
                break;
        case 0x8D:
                notImplemented(opcode);
                break;
        case 0x8E:
                notImplemented(opcode);
                break;
        case 0x90:
                notImplemented(opcode);
                break;
        case 0x91:
                notImplemented(opcode);
                break;
        case 0x92:
                notImplemented(opcode);
                break;
        case 0x94:
                notImplemented(opcode);
                break;
        case 0x95:
                notImplemented(opcode);
                break;
        case 0x96:
                notImplemented(opcode);
                break;
        case 0x98:
                notImplemented(opcode);
                break;
        case 0x99:
                notImplemented(opcode);
                break;
        case 0x9A:
                notImplemented(opcode);
                break;
        case 0x9C:
                notImplemented(opcode);
                break;
        case 0x9D:
                notImplemented(opcode);
                break;
        case 0x9E:
                notImplemented(opcode);
                break;
        case 0xA0:
                notImplemented(opcode);
                break;
        case 0xA1:
                notImplemented(opcode);
                break;
        case 0xA2:
                notImplemented(opcode);
                break;
        case 0xA4:
                notImplemented(opcode);
                break;
        case 0xA5:
                notImplemented(opcode);
                break;
        case 0xA6:
                notImplemented(opcode);
                break;
        case 0xA8:
                notImplemented(opcode);
                break;
        case 0xA9:
                notImplemented(opcode);
                break;
        case 0xAA:
                notImplemented(opcode);
                break;
        case 0xAC:
                notImplemented(opcode);
                break;
        case 0xAD:
                notImplemented(opcode);
                break;
        case 0xAE:
                notImplemented(opcode);
                break;
        case 0xB0:
                notImplemented(opcode);
                break;
        case 0xB1:
                notImplemented(opcode);
                break;
        case 0xB2:
                notImplemented(opcode);
                break;
        case 0xB4:
                notImplemented(opcode);
                break;
        case 0xB5:
                notImplemented(opcode);
                break;
        case 0xB6:
                notImplemented(opcode);
                break;
        case 0xB8:
                notImplemented(opcode);
                break;
        case 0xB9:
                notImplemented(opcode);
                break;
        case 0xBA:
                notImplemented(opcode);
                break;
        case 0xBC:
                notImplemented(opcode);
                break;
        case 0xBD:
                notImplemented(opcode);
                break;
        case 0xBE:
                notImplemented(opcode);
                break;
        case 0xC0:
                notImplemented(opcode);
                break;
        case 0xC1:
                notImplemented(opcode);
                break;
        case 0xC4:
                notImplemented(opcode);
                break;
        case 0xC5:
                notImplemented(opcode);
                break;
        case 0xC6:
                notImplemented(opcode);
                break;
        case 0xC8:
                notImplemented(opcode);
                break;
        case 0xC9:
                notImplemented(opcode);
                break;
        case 0xCA:
                notImplemented(opcode);
                break;
        case 0xCB:
                notImplemented(opcode);
                break;
        case 0xCC:
                notImplemented(opcode);
                break;
        case 0xCD:
                notImplemented(opcode);
                break;
        case 0xCE:
                notImplemented(opcode);
                break;
        case 0xD0:
                notImplemented(opcode);
                break;
        case 0xD1:
                notImplemented(opcode);
                break;
        case 0xD2:
                notImplemented(opcode);
                break;
        case 0xD5:
                notImplemented(opcode);
                break;
        case 0xD6:
                notImplemented(opcode);
                break;
        case 0xD8:
                notImplemented(opcode);
                break;
        case 0xD9:
                notImplemented(opcode);
                break;
        case 0xDA:
                notImplemented(opcode);
                break;
        case 0xDB:
                notImplemented(opcode);
                break;
        case 0xDD:
                notImplemented(opcode);
                break;
        case 0xDE:
                notImplemented(opcode);
                break;
        case 0xE0:
                notImplemented(opcode);
                break;
        case 0xE1:
                notImplemented(opcode);
                break;
        case 0xE4:
                notImplemented(opcode);
                break;
        case 0xE5:
                notImplemented(opcode);
                break;
        case 0xE6:
                notImplemented(opcode);
                break;
        case 0xE8:
                notImplemented(opcode);
                break;
        case 0xE9:
                notImplemented(opcode);
                break;
        case 0xEA:
                notImplemented(opcode);
                break;
        case 0xEC:
                notImplemented(opcode);
                break;
        case 0xED:
                notImplemented(opcode);
                break;
        case 0xEE:
                notImplemented(opcode);
                break;
        case 0xF0:
                notImplemented(opcode);
                break;
        case 0xF1:
                notImplemented(opcode);
                break;
        case 0xF2:
                notImplemented(opcode);
                break;
        case 0xF5:
                notImplemented(opcode);
                break;
        case 0xF6:
                notImplemented(opcode);
                break;
        case 0xF8:
                notImplemented(opcode);
                break;
        case 0xF9:
                notImplemented(opcode);
                break;
        case 0xFA:
                notImplemented(opcode);
                break;
        case 0xFD:
                notImplemented(opcode);
                break;
        case 0xFE:
                notImplemented(opcode);
                break;
        default:
                illegalOpcode(opcode);
                break;
        }
}

uint8_t readByte(FILE* program, uint8_t pc) {
        uint8_t byte;
        
        pc += 1;
        fread(&byte, 1, 1, program);

        return byte;
}

void setNegFlag(uint8_t result, Registers *registers) {
        if(result & 0b1000000) {
            registers->p = (registers->p | 0b10000000);
        }
        else {
            registers->p = (registers->p & 0b01111111);
        }
}

void setZeroFlag(uint8_t result, Registers *registers) {
        if(result == 0) {
            registers->p = (registers->p | 0b00000010);
        }
        else {
            registers->p = (registers->p & 0b11111101);
        }
}

void notImplemented(uint8_t opcode) {
        printf("opcode %02x not yet implemented\n", opcode);
}

void illegalOpcode(uint8_t opcode) {
        printf("illegal opcode: %02x\n", opcode);
}
