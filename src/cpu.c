#include "cpu.h"

int execute(FILE *program, uint8_t *ram)
{
        Registers registers;
        uint8_t opcode;

        /*
         * Set initial state of registers. In a real 6502, all but
         * the bits 1 to 5 of the p register are software defined, but
         * we have to give them some value here.
         */
        registers.a = 0x00;
        registers.x = 0x00;
        registers.y = 0x00;
        registers.sp = 0xFF;
        registers.pc = 0x0000;
        registers.p = 0b00110100;

        while ((opcode = fetchImmediate(program, &registers))) {
                step(opcode, program, ram, &registers);
        }

        return 0;
}

void step(uint8_t opcode, FILE *program, uint8_t *ram, Registers *registers)
{
        uint8_t operand, res, highbyte, lowbyte;
        uint16_t address;

        switch (opcode) {
        case 0x00: /* BRK */
                /* Skip the signature byte after the BRK opcode. */
                registers->pc++;
                /* Push the high byte of the return address to the stack. */
                ram[registers->sp] = registers->pc & 0xFF;
                registers->sp--;
                /* Push the low byte of the return address to the stack. */
                ram[registers->sp] = registers->pc << 8;
                registers->sp--;
                /* Push the P register with B flag set to the stack. */
                ram[registers->sp] = registers->p | 0b00010000;
                registers->sp--;
                /* Set I and clear D (the latter is 65C02 specific). */
                SET_I(registers);
                CLEAR_D(registers);
                /* Set PC to the address at the reset vector. */
                lowbyte = ram[0xFFFE];
                highbyte = ram[0xFFFF];
                registers->pc = highbyte << 8 | lowbyte;
                break;
        case 0x01: /* ORA (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x04: /* TSB zp */
                operand = fetchZeroPage(program, registers, ram);
                res = TSB(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x05: /* ORA zp */
                operand = fetchZeroPage(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x06: /* ASL zp */
                operand = fetchZeroPage(program, registers, ram);
                res = ASL(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x08: /* PHP */
                ram[0x0100 | registers->sp] = registers->p;
                registers->sp--;
                break;
        case 0x09: /* ORA # */
                operand = fetchImmediate(program, registers);
                ORA(operand, registers);
                break;
        case 0x0A: /* ASL A */
                registers->a = ASL(registers->a, registers);
                break;
        case 0x0C: /* TSB a */
                operand = fetchAbsolute(program, registers, ram);
                res = TSB(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x0D: /* ORA a */
                operand = fetchAbsolute(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x0E: /* ASL a */
                operand = fetchAbsolute(program, registers, ram);
                res = ASL(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x10: /* BPL */
                operand = fetchImmediate(program, registers);
                /* Branch if negative flag is clear. */
                if (!N(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0x11: /* ORA (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x12: /* ORA (zp) */
                operand = fetchIndirect(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x14: /* TRB zp */
                operand = fetchZeroPage(program, registers, ram);
                res = TRB(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x15: /* ORA zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x16: /* ASL zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                res = ASL(operand, registers);
                registers->pc--;
                storeZeroPageX(program, registers, ram, res);
                break;
        case 0x18: /* CLC */
                CLEAR_C(registers);
                break;
        case 0x19: /* ORA a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x1A: /* INC A */
                registers->a++;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x1C: /* TRB a */
                operand = fetchAbsolute(program, registers, ram);
                res = TRB(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x1D: /* ORA a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                ORA(operand, registers);
                break;
        case 0x1E: /* ASL a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                res = ASL(operand, registers);
                registers->pc -= 2;
                storeAbsoluteX(program, registers, ram, res);
                break;
        case 0x20: /* JSR */
                fpread(&lowbyte, 1, 1, registers->pc, program);
                registers->pc++;
                fpread(&highbyte, 1, 1, registers->pc, program);
                /*
                 * Push the address of the last byte before the next
                 * instruction into the stack; this is our return
                 * address.
                 */
                ram[registers->sp] = registers->pc << 8;
                registers->sp--;
                ram[registers->sp] = registers->pc & 0xFF;
                registers->sp--;
                address = highbyte << 8 | lowbyte;
                registers->pc = address;
                break;
        case 0x21: /* AND (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x24: /* BIT zp */
                operand = fetchZeroPage(program, registers, ram);
                BIT(operand, registers);
                break;
        case 0x25: /* AND zp */
                operand = fetchZeroPage(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x26: /* ROL zp */
                operand = fetchZeroPage(program, registers, ram);
                res = ROL(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x28: /* PLP */
                registers->sp++;
                registers->p = ram[0x0100 | registers->sp];
                break;
        case 0x29: /* AND # */
                operand = fetchImmediate(program, registers);
                AND(operand, registers);
                break;
        case 0x2A: /* ROL A */
                registers->a = ROL(registers->a, registers);
                break;
        case 0x2C: /* BIT a */
                operand = fetchAbsolute(program, registers, ram);
                BIT(operand, registers);
                break;
        case 0x2D: /* AND a */
                operand = fetchAbsolute(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x2E: /* ROL a */
                operand = fetchAbsolute(program, registers, ram);
                res = ROL(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x30: /* BMI */
                operand = fetchImmediate(program, registers);
                /* Branch if negative flag is set. */
                if (N(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0x31: /* AND (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x32: /* AND (zp) */
                operand = fetchIndirect(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x34: /* BIT zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                BIT(operand, registers);
                break;
        case 0x35: /* AND zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x36: /* ROL zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                res = ROL(operand, registers);
                registers->pc--;
                storeZeroPageX(program, registers, ram, res);
                break;
        case 0x38: /* SEC */
                SET_C(registers);
                break;
        case 0x39: /* AND a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x3A: /* DEC A */
                registers->a--;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x3C: /* BIT a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                BIT(operand, registers);
                break;
        case 0x3D: /* AND a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                AND(operand, registers);
                break;
        case 0x3E: /* ROL a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                res = ROL(operand, registers);
                registers->pc -= 2;
                storeAbsoluteX(program, registers, ram, res);
                break;
        case 0x40: /* RTI */
                registers->sp--;
                registers->p = ram[0x0100 | registers->sp];
                registers->sp--;
                lowbyte = ram[0x0100 | registers->sp];
                registers->sp--;
                highbyte = ram[0x0100 | registers->sp];
                registers->pc = highbyte << 8 | lowbyte;
                break;
        case 0x41: /* EOR (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x45: /* EOR zp */
                operand = fetchZeroPage(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x46: /* LSR zp */
                operand = fetchZeroPage(program, registers, ram);
                res = LSR(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x48: /* PHA */
                ram[0x0100 | registers->sp] = registers->a;
                registers->sp--;
                break;
        case 0x49: /* EOR # */
                operand = fetchImmediate(program, registers);
                EOR(operand, registers);
                break;
        case 0x4A: /* LSR A */
                registers->a = LSR(registers->a, registers);
                break;
        case 0x4C: /* JMP a */
                fpread(&lowbyte, 1, 1, registers->pc, program);
                registers->pc++;
                fpread(&highbyte, 1, 1, registers->pc, program);
                registers->pc++;
                address = highbyte << 8 | lowbyte;
                registers->pc = address;
                break;
        case 0x4D: /* EOR a */
                operand = fetchAbsolute(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x4E: /* LSR a */
                operand = fetchAbsolute(program, registers, ram);
                res = LSR(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x50: /* BVC */
                operand = fetchImmediate(program, registers);
                /* Branch if overflow flag is clear. */
                if (!V(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0x51: /* EOR (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x52: /* EOR (zp) */
                operand = fetchIndirect(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x55: /* EOR zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x56: /* LSR zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                res = LSR(operand, registers);
                registers->pc--;
                storeZeroPageX(program, registers, ram, res);
                break;
        case 0x58: /* CLI */
                CLEAR_I(registers);
                break;
        case 0x59: /* EOR a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x5A: /* PHY */
                ram[0x0100 | registers->sp] = registers->y;
                registers->sp--;
                break;
        case 0x5D: /* EOR a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                EOR(operand, registers);
                break;
        case 0x5E: /* LSR a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                res = LSR(operand, registers);
                registers->pc -= 2;
                storeAbsoluteX(program, registers, ram, res);
                break;
        case 0x60: /* RTS */
                registers->sp++;
                lowbyte = ram[0x0100 | registers->sp];
                registers->sp++;
                highbyte = ram[0x0100 | registers->sp];
                registers->pc = (highbyte << 8 | lowbyte) + 1;
                break;
        case 0x61: /* ADC (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x64: /* STZ zp */
                storeZeroPage(program, registers, ram, 0);
                break;
        case 0x65: /* ADC zp */
                operand = fetchZeroPage(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x66: /* ROR zp */
                operand = fetchZeroPage(program, registers, ram);
                res = ROR(operand, registers);
                registers->pc--;
                storeZeroPage(program, registers, ram, res);
                break;
        case 0x68: /* PLA */
                registers->sp++;
                registers->a = ram[0x0100 | registers->sp];
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x69: /* ADC # */
                operand = fetchImmediate(program, registers);
                ADC(operand, registers);
                break;
        case 0x6A: /* ROR A */
                registers->a = ROR(registers->a, registers);
                break;
        case 0x6C: /* JMP (a) */
                fpread(&lowbyte, 1, 1, registers->pc, program);
                registers->pc++;
                fpread(&highbyte, 1, 1, registers->pc, program);
                registers->pc++;
                address = highbyte << 8 | lowbyte;
                /*
                 * Get a 16 bit value from the low byte located in the
                 * ram at the address specified, and the high byte
                 * which is the next byte in memory.
                 */
                registers->pc = (ram[address] << 8) | ram[address + 1];
                break;
        case 0x6D: /* ADC a */
                operand = fetchAbsolute(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x6E: /* ROR a */
                operand = fetchAbsolute(program, registers, ram);
                res = ROR(operand, registers);
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, res);
                break;
        case 0x70: /* BVS */
                operand = fetchImmediate(program, registers);
                /* Branch if overflow flag is set */
                if (V(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0x71: /* ADX (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x72: /* ADC (zp) */
                operand = fetchIndirect(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x74: /* STZ zp, x */
                storeZeroPageX(program, registers, ram, 0);
                break;
        case 0x75: /* ADC zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x76: /* ROR dp,x */
                operand = fetchZeroPageX(program, registers, ram);
                res = ROR(operand, registers);
                registers->pc --;
                storeZeroPageX(program, registers, ram, res);
                break;
        case 0x78: /* SEI */
                SET_I(registers);
                break;
        case 0x79: /* ADC a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x7A: /* PLY */
                registers->sp++;
                registers->y = ram[0x0100 | registers->sp];
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
                break;
        case 0x7C: /* JMP (a,x) */
                fpread(&lowbyte, 1, 1, registers->pc, program);
                registers->pc++;
                fpread(&highbyte, 1, 1, registers->pc, program);
                registers->pc++;
                address = highbyte << 8 | lowbyte;
                address += registers->x;
                /*
                 * Get a 16 bit value from the low byte located in the
                 * ram at the address specified, and the high byte
                 * which is the next byte in memory.
                 */
                registers->pc = (ram[address] << 8) | ram[address + 1];
                break;
        case 0x7D: /* ADC a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                ADC(operand, registers);
                break;
        case 0x7E: /* ROR a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                res = ROR(operand, registers);
                registers->pc -= 2;
                storeAbsoluteX(program, registers, ram, res);
                break;
        case 0x80: /* BRA */
                operand = fetchImmediate(program, registers);
                registers->pc += SIGNED(operand);
                break;
        case 0x81: /* STA (zp,x) */
                storeIndirectX(program, registers, ram, registers->a);;
                break;
        case 0x84: /* STY zp */
                storeZeroPage(program, registers, ram, registers->y);
                break;
        case 0x85: /* STA zp */
                storeZeroPage(program, registers, ram, registers->a);
                break;
        case 0x86: /* STX zp */
                storeZeroPage(program, registers, ram, registers->x);
                break;
        case 0x88: /* DEY */
                registers->y--;
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
                break;
        case 0x89: /* BIT # */
                operand = fetchImmediate(program, registers);
                updateZeroFlag((registers->a & operand), registers);
                break;
        case 0x8A: /* TXA */
                registers->a = registers->x;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x8C: /* STY a */
                storeAbsolute(program, registers, ram, registers->y);
                break;
        case 0x8D: /* STA a */
                storeAbsolute(program, registers, ram, registers->a);
                break;
        case 0x8E: /* STX a */
                storeAbsolute(program, registers, ram, registers->x);
                break;
        case 0x90: /* BCC */
                operand = fetchImmediate(program, registers);
                if (!C(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0x91: /* STA (zp),y */
                storeIndirectY(program, registers, ram, registers->a);
                break;
        case 0x92: /* STA (zp) */
                storeIndirect(program, registers, ram, registers->a);
                break;
        case 0x94: /* STY zp,x */
                storeZeroPageX(program, registers, ram, registers->y);
                break;
        case 0x95: /* STA zp,x */
                storeZeroPageX(program, registers, ram, registers->a);
                break;
        case 0x96: /* STX zp,y */
                storeZeroPageY(program, registers, ram, registers->x);
                break;
        case 0x98: /* TYA */
                registers->a = registers->y;
                updateNegFlag(registers->a, registers);
                updateZeroFlag(registers->a, registers);
                break;
        case 0x99: /* STA a,y */
                storeAbsoluteY(program, registers, ram, registers->a);
                break;
        case 0x9A: /* TXS */
                registers->sp = registers->x;
                break;
        case 0x9C: /* STZ a */
                storeAbsolute(program, registers, ram, 0);
                break;
        case 0x9D: /* STA a,x */
                storeAbsoluteX(program, registers, ram, registers->a);
                break;
        case 0x9E: /* STZ a,x */
                storeAbsoluteX(program, registers, ram, 0);
                break;
        case 0xA0: /* LDY # */
                operand = fetchImmediate(program, registers);
                LDY(operand, registers);
                break;
        case 0xA1: /* LDA (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xA2: /* LDX # */
                operand = fetchImmediate(program, registers);
                LDX(operand, registers);
                break;
        case 0xA4: /* LDY zp */
                operand = fetchZeroPage(program, registers, ram);
                LDY(operand, registers);
                break;
        case 0xA5: /* LDA zp */
                operand = fetchZeroPage(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xA6: /* LDX zp */
                operand = fetchZeroPage(program, registers, ram);
                LDX(operand, registers);
                break;
        case 0xA8: /* TAY */
                registers->y = registers->a;
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
                break;
        case 0xA9: /* LDA # */
                operand = fetchImmediate(program, registers);
                LDA(operand, registers);
                break;
        case 0xAA: /* TAX */
                registers->x = registers->a;
                updateNegFlag(registers->x, registers);
                updateZeroFlag(registers->x, registers);
                break;
        case 0xAC: /* LDY a */
                operand = fetchAbsolute(program, registers, ram);
                LDY(operand, registers);
                break;
        case 0xAD: /* LDA a */
                operand = fetchAbsolute(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xAE: /* LDX a */
                operand = fetchAbsolute(program, registers, ram);
                LDX(operand, registers);
                break;
        case 0xB0: /* BCS */
                operand = fetchImmediate(program, registers);
                if (C(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0xB1: /* LDA (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xB2: /* LDA (zp) */
                operand = fetchIndirect(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xB4: /* LDY zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                LDY(operand, registers);
                break;
        case 0xB5: /* LDA zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xB6: /* LDX zp,y */
                operand = fetchZeroPageY(program, registers, ram);
                LDX(operand, registers);
                break;
        case 0xB8: /* CLV */
                CLEAR_V(registers);
                break;
        case 0xB9: /* LDA a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xBA: /* TSX */
                registers->x = registers->sp;
                updateNegFlag(registers->x, registers);
                updateZeroFlag(registers->x, registers);
                break;
        case 0xBC: /* LDY a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                LDY(operand, registers);
                break;
        case 0xBD: /* LDA a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                LDA(operand, registers);
                break;
        case 0xBE: /* LDX a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                LDX(operand, registers);
                break;
        case 0xC0: /* CPY # */
                operand = fetchImmediate(program, registers);
                CPY(operand, registers);
                break;
        case 0xC1: /* CMP (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xC4: /* CPY zp */
                operand = fetchZeroPage(program, registers, ram);
                CPY(operand, registers);
                break;
        case 0xC5: /* CMP zp */
                operand = fetchZeroPage(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xC6: /* DEC zp */
                operand = fetchZeroPage(program, registers, ram);
                operand--;
                registers->pc--;
                storeZeroPage(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xC8: /* INY */
                registers->y++;
                updateNegFlag(registers->y, registers);
                updateZeroFlag(registers->y, registers);
                break;
        case 0xC9: /* CMP # */
                operand = fetchImmediate(program, registers);
                CMP(operand, registers);
                break;
        case 0xCA: /* DEX */
                registers->x--;
                updateNegFlag(registers->x, registers);
                updateZeroFlag(registers->x, registers);
                break;
        case 0xCC: /* CPY a */
                operand = fetchAbsolute(program, registers, ram);
                CPY(operand, registers);
                break;
        case 0xCD: /* CMP a */
                operand = fetchAbsolute(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xCE: /* DEC a */
                operand = fetchAbsolute(program, registers, ram);
                operand--;
                registers->pc -= 2;
                storeAbsolute(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xD0: /* BNE */
                operand = fetchImmediate(program, registers);
                /* Branch if zero flag is clear. */
                if (!Z(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0xD1: /* CMP (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xD2: /* CMP (zp) */
                operand = fetchIndirect(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xD5: /* CMP zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xD6: /* DEC zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                operand--;
                registers->pc--;
                storeZeroPageX(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xD8: /* CLD */
                CLEAR_D(registers);
                break;
        case 0xD9: /* CMP a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xDA: /* PHX */
                ram[0x0100 | registers->sp] = registers->x;
                registers->sp--;
                break;
        case 0xDD: /* CMP a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                CMP(operand, registers);
                break;
        case 0xDE: /* DEC a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                operand--;
                registers->pc -= 2;
                storeAbsoluteX(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xE0: /* CPX # */
                operand = fetchImmediate(program, registers);
                CPX(operand, registers);
                break;
        case 0xE1: /* SBC (zp,x) */
                operand = fetchIndirectX(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xE4: /* CPX zp */
                operand = fetchZeroPage(program, registers, ram);
                CPX(operand, registers);
                break;
        case 0xE5: /* SBC zp */
                operand = fetchZeroPage(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xE6: /* INC zp */
                operand = fetchZeroPage(program, registers, ram);
                operand++;
                registers --;
                storeZeroPage(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xE8: /* INX */
                registers->x++;
                updateNegFlag(registers->x, registers);
                updateZeroFlag(registers->x, registers);
                break;
        case 0xE9: /* SBC # */
                operand = fetchImmediate(program, registers);
                SBC(operand, registers);
                break;
        case 0xEA: /* NOP */
                break;
        case 0xEC: /* CPX a */
                operand = fetchAbsolute(program, registers, ram);
                CPX(operand, registers);
                break;
        case 0xED: /* SBC a */
                operand = fetchAbsolute(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xEE: /* INC a */
                operand = fetchAbsolute(program, registers, ram);
                operand++;
                registers -= 2;
                storeAbsolute(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xF0: /* BEQ */
                operand = fetchImmediate(program, registers);
                if (Z(registers)) {
                        registers->pc += SIGNED(operand);
                }
                break;
        case 0xF1: /* SBC (zp),y */
                operand = fetchIndirectY(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xF2: /* SBC (zp) */
                operand = fetchIndirect(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xF5: /* SBC zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xF6: /* INC zp,x */
                operand = fetchZeroPageX(program, registers, ram);
                operand++;
                registers --;
                storeZeroPageX(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        case 0xF8: /* SED */
                SET_D(registers);
                break;
        case 0xF9: /* SBC a,y */
                operand = fetchAbsoluteY(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xFA: /* PLX */
                registers->sp++;
                registers->x = ram[0x0100 | registers->sp];
                updateNegFlag(registers->x, registers);
                updateZeroFlag(registers->x, registers);
                break;
        case 0xFD: /* SBC a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                SBC(operand, registers);
                break;
        case 0xFE: /* INC a,x */
                operand = fetchAbsoluteX(program, registers, ram);
                operand++;
                registers -= 2;
                storeAbsoluteX(program, registers, ram, operand);
                updateNegFlag(operand, registers);
                updateZeroFlag(operand, registers);
                break;
        default:
                illegalOpcode(opcode);
                break;
        }
}

size_t fpread(void *ptr, size_t size, size_t nmemb, size_t offset,
              FILE *stream)
{
        if (fseek(stream, offset, SEEK_SET) != 0) {
                return 0;
        }

        return fread(ptr, size, nmemb, stream);
}

/* Immediate addressing | # */
uint8_t fetchImmediate(FILE *program, Registers *registers)
{
        uint8_t byte;

        fpread(&byte, 1, 1, registers->pc, program);
        registers->pc++;

        return byte;
}

/* Absolute addressing | a */
uint8_t fetchAbsolute(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;
        address = highbyte << 8 | lowbyte;

        return ram[address];
}

/* Absolute indexed, x addressing | a,x */
uint8_t fetchAbsoluteX(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;

        address = highbyte << 8 | lowbyte;
        address += registers->x;

        return ram[address];
}

/* Absolute indexed, y addressing | a,y */
uint8_t fetchAbsoluteY(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;

        address = highbyte << 8 | lowbyte;
        address += registers->y;

        return ram[address];
}

/* Zero page addressing (aka Direct page addressing) | zp */
uint8_t fetchZeroPage(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        return ram[address];
}

/* Zero page indexed, x addressing | zp,x */
uint8_t fetchZeroPageX(FILE *program, Registers *registers, uint8_t *ram)
{
        /* Note that the address wraps around if greater than 0xFF */
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->x;

        return ram[address];
}

/* Zero page indexed, y addressing | zp,y */
uint8_t fetchZeroPageY(FILE *program, Registers *registers, uint8_t *ram)
{
        /* Note that the address wraps around if greater than 0xFF */
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->y;

        return ram[address];
}

/* Zero page indirect addressing | (zp) */
uint8_t fetchIndirect(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        effectiveAddress = ram[address + 1] << 8 | ram[address];

        return ram[effectiveAddress];
}

/* Zero page indexed indirect, x addressing | (zp,x) */
uint8_t fetchIndirectX(FILE *program, Registers *registers, uint8_t *ram)
{
        /* Note that the address wraps around if greater than 0xFF. */
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->x;

        /* Dereference and get address stored at address in ram. */
        effectiveAddress = ram[address + 1] << 8 | ram[address];

        return ram[effectiveAddress];
}

/* Zero page indirected indexed, y addressing | (zp),y */
uint8_t fetchIndirectY(FILE *program, Registers *registers, uint8_t *ram)
{
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        /* Dereference and get address stored at address in ram. */
        effectiveAddress = ram[address + 1] << 8 | ram[address];
        effectiveAddress += registers->y;

        return ram[effectiveAddress];
}

void storeAbsolute(FILE *program, Registers *registers, uint8_t *ram,
                   uint8_t value)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;
        address = highbyte << 8 | lowbyte;

        ram[address] = value;
}

void storeAbsoluteX(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;

        address = highbyte << 8 | lowbyte;
        address += registers->x;

        ram[address] = value;
}

void storeAbsoluteY(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        uint8_t lowbyte, highbyte;
        uint16_t address;

        fpread(&lowbyte, 1, 1, registers->pc, program);
        registers->pc++;
        fpread(&highbyte, 1, 1, registers->pc, program);
        registers->pc++;

        address = highbyte << 8 | lowbyte;
        address += registers->y;

        ram[address] = value;
}

void storeZeroPage(FILE *program, Registers *registers, uint8_t *ram,
                   uint8_t value)
{
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        ram[address] = value;
}

void storeZeroPageX(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->x;

        ram[address] = value;
}

void storeZeroPageY(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        uint8_t address;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->y;

        ram[address] = value;
}

void storeIndirect(FILE *program, Registers *registers, uint8_t *ram,
                   uint8_t value)
{
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        effectiveAddress = ram[address + 1] << 8 | ram[address];

        ram[effectiveAddress] = value;
}

void storeIndirectX(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        /* Note that the address wraps around if greater than 0xFF. */
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        address += registers->x;

        /* Dereference and get address stored at address in ram. */
        effectiveAddress = ram[address + 1] << 8 | ram[address];

        ram[effectiveAddress] = value;
}

void storeIndirectY(FILE *program, Registers *registers, uint8_t *ram,
                    uint8_t value)
{
        uint8_t address;
        uint16_t effectiveAddress;

        fpread(&address, 1, 1, registers->pc, program);
        registers->pc++;

        /* Dereference and get address stored at address in ram. */
        effectiveAddress = ram[address + 1] << 8 | ram[address];
        effectiveAddress += registers->y;

        ram[effectiveAddress] = value;
}

void updateNegFlag(uint8_t result, Registers *registers)
{
        /* if bit 7 is set, value is negative. */
        result & 0b1000000 ? SET_N(registers) : CLEAR_N(registers);
}

void updateZeroFlag(uint8_t result, Registers *registers)
{
        result == 0 ? SET_Z(registers) : CLEAR_Z(registers);
}

void ADC(uint8_t operand, Registers *registers)
{
        uint8_t res = registers->a + operand + C(registers);

        /* Set (signed) overflow flag if MSB of A and res differ. */
        ((registers->a & 0b10000000) != (res & 0b10000000)) ?
                SET_V(registers) : CLEAR_V(registers);
        updateNegFlag(registers->a, registers);
        updateZeroFlag(res, registers);

        /* Handle decimal mode. */
        if (D(registers)) {
                res = BCDToBin(registers->a) + BCDToBin(operand) +
                        C(registers);
                /*
                 * Set carry if res is greater than 99, the largest
                 * acceptable decimal value.
                 */
                res > 99 ? SET_C(registers) : CLEAR_C(registers);
                /* Wrap around if overflowing. */
                res %= 100;
                res = binToBCD(res);
        } else {
                /*
                 * If res is smaller than operand, there was an
                 * unsigned overflow, so set C.
                 */
                (res < operand) ?
                        SET_C(registers) : CLEAR_C(registers);
        }

        registers->a = res;
}

void AND(uint8_t operand, Registers *registers)
{
        registers->a &= operand;
        updateNegFlag(registers->a, registers);
        updateZeroFlag(registers->a, registers);
}

uint8_t ASL(uint8_t operand, Registers *registers)
{
        operand & 0b10000000 ?
                SET_C(registers): CLEAR_C(registers);
        operand <<= 1;
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);

        return operand;
}

void BIT(uint8_t operand, Registers *registers)
{
        operand | 0b10000000 ?
                SET_N(registers): CLEAR_N(registers);
        operand | 0b01000000 ?
                SET_V(registers): CLEAR_V(registers);
        updateZeroFlag((registers->a & operand), registers);
}

void CMP(uint8_t operand, Registers *registers)
{
        uint8_t res = registers->a - operand;
        registers->a >= operand ?
                SET_C(registers) : CLEAR_C(registers);
        updateNegFlag(res, registers);
        updateZeroFlag(res, registers);
}

void CPX(uint8_t operand, Registers *registers)
{
        uint8_t res = registers->x - operand;
        registers->x >= operand ?
                SET_C(registers) : CLEAR_C(registers);
        updateNegFlag(res, registers);
        updateZeroFlag(res, registers);
}

void CPY(uint8_t operand, Registers *registers)
{
        uint8_t res = registers->x - operand;
        registers->y >= operand ?
                SET_C(registers) : CLEAR_C(registers);
        updateNegFlag(res, registers);
        updateZeroFlag(res, registers);
}

void EOR(uint8_t operand, Registers *registers)
{
        registers->a ^= operand;
        updateNegFlag(registers->a, registers);
        updateZeroFlag(registers->a, registers);
}

void LDA(uint8_t operand, Registers *registers)
{
        registers->a = operand;
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);
}

void LDX(uint8_t operand, Registers *registers)
{
        registers->x = operand;
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);
}

void LDY(uint8_t operand, Registers *registers)
{
        registers->y = operand;
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);
}

uint8_t LSR(uint8_t operand, Registers *registers)
{
        operand & 0b00000001 ?
                SET_C(registers): CLEAR_C(registers);
        operand >>= 1;
        CLEAR_N(registers);
        updateZeroFlag(operand, registers);

        return operand;
}

void ORA(uint8_t operand, Registers *registers)
{
        registers->a |= operand;
        updateNegFlag(registers->a, registers);
        updateZeroFlag(registers->a, registers);
}

uint8_t ROL(uint8_t operand, Registers *registers)
{
        uint8_t res;

        res = C(registers) ? 1 : 0;
        operand & 0b10000000 ?
                SET_C(registers): CLEAR_C(registers);
        operand <<= 1;
        if (res) {
                operand |= 0b00000001;
        }
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);

        return operand;
}

uint8_t ROR(uint8_t operand, Registers *registers)
{
        uint8_t res;

        res = C(registers) ? 1 : 0;
        operand & 0b00000001 ?
                SET_C(registers): CLEAR_C(registers);
        operand >>= 1;
        if (res) {
                operand |= 0b10000000;
        }
        updateNegFlag(operand, registers);
        updateZeroFlag(operand, registers);

        return operand;
}

void SBC(uint8_t operand, Registers *registers)
{
        uint8_t res;

        /* Handle decimal mode.*/
        if (D(registers)) {
                res = BCDToBin(registers->a) - BCDToBin(operand) -
                        !C(registers);
                /* FIXME: handle underflow? */
                res > 99 ? SET_V(registers) : CLEAR_V(registers);
                /* Wrap around if overflowing. */
                res %= 100;
                res = binToBCD(res);
        } else {
                res = registers->a - operand - !C(registers);
                /* If the register and operand signs differ. */
                if (!(0x80 & registers->a) & (0x80 & operand)) {
                        /*
                         * And if the result has the same sign as the
                         * operand, then there is overflow.
                         */
                        if (!((0x80 & operand) & (0x80 & res))) {
                                SET_V(registers);
                        } else {
                                CLEAR_V(registers);
                        }
                } else {
                        CLEAR_V(registers);
                }
        }

        SIGNED(res) >= 0 ? SET_C(registers) : CLEAR_C(registers);
        updateNegFlag(res, registers);
        updateZeroFlag(res, registers);
        registers->a = res;
}

uint8_t TRB(uint8_t operand, Registers *registers)
{
        updateZeroFlag((registers->a & operand), registers);
        operand &= ~registers->a;

        return operand;
}

uint8_t TSB(uint8_t operand, Registers *registers)
{
        updateZeroFlag((registers->a & operand), registers);
        operand |= registers->a;

        return operand;
}

uint8_t binToBCD(uint8_t value)
{
        uint8_t result = 0, i = 1;

        while (value != 0) {
                result |= (value % 10) * i;
                value /= 10;
                i *= 16;
        }

        return result;
}

uint8_t BCDToBin(uint8_t value)
{
        uint8_t result = 0, i = 1;

        while (value != 0) {
                result |= (value % 2) * i;
                value >>= 4;
                i *= 10;
        }

        return result;
}

void notImplemented(uint8_t opcode)
{
        printf("opcode %02x not yet implemented\n", opcode);
}

void illegalOpcode(uint8_t opcode)
{
        printf("illegal opcode: %02x\n", opcode);
}
