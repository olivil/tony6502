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

        while((opcode = fetchImmediate(program, &registers))) {
                step(opcode, program, ram, &registers);
        }

        return 0;
}

void step(uint8_t opcode, FILE* program, uint8_t *ram, Registers *registers) {
        uint8_t operand, temp;

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
        case 0x09: /* ORA # */
                operand = fetchImmediate(program, registers);
                registers->a = (registers->a | operand);
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x0A: /* ASL A */
                registers->a & 0b10000000 ?
                        SET_C(registers): CLEAR_C(registers);
                registers->a <<= 1;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
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
        case 0x10: /* BPL */
                operand = fetchImmediate(program, registers);
                /* Branch if negative flag is clear */
                if(!N(registers)) {
                        registers->pc += SIGNED(operand);
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
        case 0x18: /* CLC */
                CLEAR_C(registers);
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
        case 0x29: /* AND # */
                operand = fetchImmediate(program, registers);
                registers->a = (registers->a & operand);
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x2A: /* ROL A */
                temp = C(registers) ? 1 : 0;
                registers->a & 0b10000000 ?
                        SET_C(registers): CLEAR_C(registers);
                registers->a <<= 1;
                if(temp) {
                        registers->a |= 0b00000001;
                }
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x2C: /* BIT a */
                operand = fetchAbsolute(program, registers, ram);
                operand | 0b10000000 ?
                        SET_N(registers): CLEAR_N(registers);
                operand | 0b01000000 ?
                        SET_V(registers): CLEAR_V(registers);
                updateZeroFlag((registers->a & operand), registers);
                break;
        case 0x2D:
                notImplemented(opcode);
                break;
        case 0x2E:
                notImplemented(opcode);
                break;
        case 0x30: /* BMI */
                operand = fetchImmediate(program, registers);
                /* Branch if negative flag is set */
                if(N(registers)) {
                        registers->pc += SIGNED(operand);
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
        case 0x3A: /* DEC A */
                registers->a--;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
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
        case 0x49: /* EOR # */
                operand = fetchImmediate(program, registers);
                registers->a = (registers->a ^ operand);
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x4A: /* LSR A */
                registers->a & 0b00000001 ?
                        SET_C(registers): CLEAR_C(registers);
                registers->a >>= 1;
                CLEAR_N(registers);
                updateZeroFlag(registers->a, registers);
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
        case 0x50: /* BVC */
                operand = fetchImmediate(program, registers);
                /* Branch if overflow flag is clear */
                if(!V(registers)) {
                        registers->pc += SIGNED(operand);
                }
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
        case 0x58: /* CLI */
                CLEAR_I(registers);
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
        case 0x69: /* ADC # */
                operand = fetchImmediate(program, registers);

                temp = registers->a + operand + C(registers);

                /* Set (signed) overflow flag if MSB of A and temp differ */
                ((registers->a & 0b10000000) != (temp & 0b10000000)) ?
                        SET_V(registers) : CLEAR_V(registers);
                updateNegFlag(registers->a, registers);
                updateZeroFlag(temp, registers);

                /* Handle decimal mode */
                if(D(registers)) {
                        temp = BCDToBin(registers->a) + BCDToBin(operand) +
                                C(registers);
                        /* Set carry if temp is greater than 99, the largest
                           acceptable decimal value */
                        temp > 99 ? SET_C(registers) : CLEAR_C(registers);
                        temp %= 100; /* Wrap around if overflowing */
                        temp = binToBCD(temp);
                } else {
                        /* If temp is smaller than operand,
                           there was an unsigned overflow, so set C */
                        (temp < operand) ?
                                SET_C(registers) : CLEAR_C(registers);
                }

                registers->a = temp;
                break;
        case 0x6A: /* ROR A */
                temp = C(registers) ? 1 : 0;
                registers->a & 0b00000001 ?
                        SET_C(registers): CLEAR_C(registers);
                registers->a >>= 1;
                if(temp) {
                        registers->a |= 0b10000000;
                }
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
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
        case 0x70: /* BVS */
                operand = fetchImmediate(program, registers);
                /* Branch if overflow flag is set */
                if(V(registers)) {
                        registers->pc += SIGNED(operand);
                }
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
        case 0x80: /* BRA */
                operand = fetchImmediate(program, registers);
                registers->pc += SIGNED(operand);
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
        case 0x89: /* BIT # */
                operand = fetchImmediate(program, registers);
                updateZeroFlag((registers->a & operand), registers);
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
        case 0xA0: /* LDY # */
                operand = fetchImmediate(program, registers);
                registers->y = operand;
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
                break;
        case 0xA1:
                notImplemented(opcode);
                break;
        case 0xA2: /* LDX # */
                operand = fetchImmediate(program, registers);
                registers->x = operand;
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
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
        case 0xA9: /* LDA # */
                operand = fetchImmediate(program, registers);
                registers->a = operand;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
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
        case 0xB8: /* CLV */
                CLEAR_V(registers);
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
        case 0xC0: /* CPY # */
                operand = fetchImmediate(program, registers);
                temp = registers->y - operand;
                registers->y >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
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
        case 0xC9: /* CMP # */
                operand = fetchImmediate(program, registers);
                temp = registers->a - operand;
                registers->a >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
                break;
        case 0xCA:
                notImplemented(opcode);
                break;
        case 0xCB:
                notImplemented(opcode);
                break;
        case 0xCC: /* CPY a */
                operand = fetchAbsolute(program, registers, ram);
                temp = registers->y - operand;
                registers->y >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
                break;
        case 0xCD: /* CMP a */
                operand = fetchAbsolute(program, registers, ram);
                temp = registers->a - operand;
                registers->a >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
                break;
        case 0xCE:
                notImplemented(opcode);
                break;
        case 0xD0: /* BNE */
                operand = fetchImmediate(program, registers);
                /* Branch if zero flag is clear */
                if(!Z(registers)) {
                        registers->pc += SIGNED(operand);
                }
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
        case 0xD8: /* CLD */
                CLEAR_D(registers);
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
        case 0xE0: /* CPX # */
                operand = fetchImmediate(program, registers);
                temp = registers->x - operand;
                registers->x >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
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
        case 0xEA: /* NOP */
                break;
        case 0xEC: /* CPX a */
                operand = fetchAbsolute(program, registers, ram);
                temp = registers->x - operand;
                registers->x >= operand ?
                        SET_C(registers) : CLEAR_C(registers);
                updateNegFlag(temp, registers);
                updateZeroFlag(temp, registers);
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

size_t fpread(void *ptr, size_t size, size_t nmemb, size_t offset,
              FILE *stream) {
        if(fseek(stream, offset, SEEK_SET) != 0) {
                return 0;
        }

        return fread(ptr, size, nmemb, stream);
}

uint8_t fetchImmediate(FILE* program, Registers *registers) {
        uint8_t byte;

        fpread(&byte, 1, 1, registers->pc, program);
        registers->pc++;

        return byte;
}

uint8_t fetchAbsolute(FILE* program, Registers *registers, uint8_t *ram) {
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;
        address = highbyte << 8 | lowbyte;

        return ram[address];
}

uint8_t fetchZeroPage(FILE* program, Registers *registers, uint8_t *ram) {
        uint8_t byte;

        fpread(&byte, 1, 1, registers->pc, program);
        registers->pc++;

        return ram[byte];
}

void updateNegFlag(uint8_t result, Registers *registers) {
        /* if bit 7 is set, value is negative */
        result & 0b1000000 ? SET_N(registers) : CLEAR_N(registers);
}

void updateZeroFlag(uint8_t result, Registers *registers) {
        result == 0 ? SET_Z(registers) : CLEAR_Z(registers);
}

uint8_t binToBCD(uint8_t value) {
        uint8_t result = 0, i = 1;

        while(value != 0) {
                result |= (value % 10) * i;
                value /= 10;
                i *= 16;
        }

        return result;
}

uint8_t BCDToBin(uint8_t value) {
        uint8_t result = 0, i = 1;

        while(value != 0) {
                result |= (value % 2) * i;
                value >>= 4;
                i *= 10;
        }

        return result;
}

void notImplemented(uint8_t opcode) {
        printf("opcode %02x not yet implemented\n", opcode);
}

void illegalOpcode(uint8_t opcode) {
        printf("illegal opcode: %02x\n", opcode);
}
