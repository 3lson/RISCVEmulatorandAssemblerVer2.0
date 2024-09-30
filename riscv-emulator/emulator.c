#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vfs.h"

#define MEM_SIZE 4096
#define NUM_REGISTERS 32

// RISC-V Opcodes
#define OPCODE_LUI    0x37
#define OPCODE_AUIPC  0x17
#define OPCODE_JAL    0x6F
#define OPCODE_JALR   0x67
#define OPCODE_BRANCH 0x63
#define OPCODE_LOAD   0x03
#define OPCODE_STORE  0x23
#define OPCODE_OP_IMM 0x13
#define OPCODE_OP     0x33
#define OPCODE_FENCE  0x0F
#define OPCODE_SYSTEM 0x73

// Function codes
#define FUNCT3_BEQ  0x0
#define FUNCT3_BNE  0x1
#define FUNCT3_BLT  0x4
#define FUNCT3_BGE  0x5
#define FUNCT3_BLTU 0x6
#define FUNCT3_BGEU 0x7

#define FUNCT3_LB  0x0
#define FUNCT3_LH  0x1
#define FUNCT3_LW  0x2
#define FUNCT3_LBU 0x4
#define FUNCT3_LHU 0x5

#define FUNCT3_SB 0x0
#define FUNCT3_SH 0x1
#define FUNCT3_SW 0x2

#define FUNCT3_ADD  0x0
#define FUNCT3_SLL  0x1
#define FUNCT3_SLT  0x2
#define FUNCT3_SLTU 0x3
#define FUNCT3_XOR  0x4
#define FUNCT3_SRL  0x5
#define FUNCT3_OR   0x6
#define FUNCT3_AND  0x7

#define FUNCT7_ADD  0x00
#define FUNCT7_SUB  0x20
#define FUNCT7_SRL  0x00
#define FUNCT7_SRA  0x20

typedef struct {
    uint32_t registers[NUM_REGISTERS];
    uint32_t pc;
    uint8_t memory[MEM_SIZE];
} CPU;

void initialize_cpu(CPU *cpu) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->registers[i] = 0;
    }
    cpu->pc = 0;
    init_vfs();
}

uint32_t fetch(CPU *cpu) {
    uint32_t instruction = *(uint32_t*)&cpu->memory[cpu->pc];
    cpu->pc += 4;
    return instruction;
}

void print_instruction(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    int32_t imm_i = (int32_t)instruction >> 20;
    int32_t imm_s = ((int32_t)(instruction & 0xFE000000) >> 20) | ((instruction >> 7) & 0x1F);
    int32_t imm_b = ((int32_t)(instruction & 0x80000000) >> 19) | ((instruction & 0x80) << 4) | ((instruction >> 20) & 0x7E0) | ((instruction >> 7) & 0x1E);
    uint32_t imm_u = instruction & 0xFFFFF000;
    int32_t imm_j = ((int32_t)(instruction & 0x80000000) >> 11) | (instruction & 0xFF000) | ((instruction >> 9) & 0x800) | ((instruction >> 20) & 0x7FE);

    printf("Instruction: 0x%08x - ", instruction);

    switch (opcode) {
        case OPCODE_LUI:    printf("LUI x%d, 0x%x\n", rd, imm_u >> 12); break;
        case OPCODE_AUIPC:  printf("AUIPC x%d, 0x%x\n", rd, imm_u >> 12); break;
        case OPCODE_JAL:    printf("JAL x%d, %d\n", rd, imm_j); break;
        case OPCODE_JALR:   printf("JALR x%d, x%d, %d\n", rd, rs1, imm_i); break;
        case OPCODE_BRANCH:
            switch (funct3) {
                case FUNCT3_BEQ:  printf("BEQ x%d, x%d, %d\n", rs1, rs2, imm_b); break;
                case FUNCT3_BNE:  printf("BNE x%d, x%d, %d\n", rs1, rs2, imm_b); break;
                case FUNCT3_BLT:  printf("BLT x%d, x%d, %d\n", rs1, rs2, imm_b); break;
                case FUNCT3_BGE:  printf("BGE x%d, x%d, %d\n", rs1, rs2, imm_b); break;
                case FUNCT3_BLTU: printf("BLTU x%d, x%d, %d\n", rs1, rs2, imm_b); break;
                case FUNCT3_BGEU: printf("BGEU x%d, x%d, %d\n", rs1, rs2, imm_b); break;
            }
            break;
        case OPCODE_LOAD:
            switch (funct3) {
                case FUNCT3_LB:  printf("LB x%d, %d(x%d)\n", rd, imm_i, rs1); break;
                case FUNCT3_LH:  printf("LH x%d, %d(x%d)\n", rd, imm_i, rs1); break;
                case FUNCT3_LW:  printf("LW x%d, %d(x%d)\n", rd, imm_i, rs1); break;
                case FUNCT3_LBU: printf("LBU x%d, %d(x%d)\n", rd, imm_i, rs1); break;
                case FUNCT3_LHU: printf("LHU x%d, %d(x%d)\n", rd, imm_i, rs1); break;
            }
            break;
        case OPCODE_STORE:
            switch (funct3) {
                case FUNCT3_SB: printf("SB x%d, %d(x%d)\n", rs2, imm_s, rs1); break;
                case FUNCT3_SH: printf("SH x%d, %d(x%d)\n", rs2, imm_s, rs1); break;
                case FUNCT3_SW: printf("SW x%d, %d(x%d)\n", rs2, imm_s, rs1); break;
            }
            break;
        case OPCODE_OP_IMM:
            switch (funct3) {
                case FUNCT3_ADD:  printf("ADDI x%d, x%d, %d\n", rd, rs1, imm_i); break;
                case FUNCT3_SLL:  printf("SLLI x%d, x%d, %d\n", rd, rs1, imm_i & 0x1F); break;
                case FUNCT3_SLT:  printf("SLTI x%d, x%d, %d\n", rd, rs1, imm_i); break;
                case FUNCT3_SLTU: printf("SLTIU x%d, x%d, %d\n", rd, rs1, imm_i); break;
                case FUNCT3_XOR:  printf("XORI x%d, x%d, %d\n", rd, rs1, imm_i); break;
                case FUNCT3_SRL:
                    if ((imm_i >> 10) & 1) printf("SRAI x%d, x%d, %d\n", rd, rs1, imm_i & 0x1F);
                    else printf("SRLI x%d, x%d, %d\n", rd, rs1, imm_i & 0x1F);
                    break;
                case FUNCT3_OR:   printf("ORI x%d, x%d, %d\n", rd, rs1, imm_i); break;
                case FUNCT3_AND:  printf("ANDI x%d, x%d, %d\n", rd, rs1, imm_i); break;
            }
            break;
        case OPCODE_OP:
            switch (funct3) {
                case FUNCT3_ADD:
                    if (((instruction >> 25) & 0x7F) == FUNCT7_SUB) printf("SUB x%d, x%d, x%d\n", rd, rs1, rs2);
                    else printf("ADD x%d, x%d, x%d\n", rd, rs1, rs2);
                    break;
                case FUNCT3_SLL:  printf("SLL x%d, x%d, x%d\n", rd, rs1, rs2); break;
                case FUNCT3_SLT:  printf("SLT x%d, x%d, x%d\n", rd, rs1, rs2); break;
                case FUNCT3_SLTU: printf("SLTU x%d, x%d, x%d\n", rd, rs1, rs2); break;
                case FUNCT3_XOR:  printf("XOR x%d, x%d, x%d\n", rd, rs1, rs2); break;
                case FUNCT3_SRL:
                    if (((instruction >> 25) & 0x7F) == FUNCT7_SRA) printf("SRA x%d, x%d, x%d\n", rd, rs1, rs2);
                    else printf("SRL x%d, x%d, x%d\n", rd, rs1, rs2);
                    break;
                case FUNCT3_OR:   printf("OR x%d, x%d, x%d\n", rd, rs1, rs2); break;
                case FUNCT3_AND:  printf("AND x%d, x%d, x%d\n", rd, rs1, rs2); break;
            }
            break;
        case OPCODE_FENCE: printf("FENCE\n"); break;
        case OPCODE_SYSTEM:
            if (instruction == 0x00000073) printf("ECALL\n");
            else if (instruction == 0x00100073) printf("EBREAK\n");
            else printf("SYSTEM (not implemented)\n");
            break;
        default: printf("Unknown instruction\n");
    }
}

void execute(CPU *cpu, uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    int32_t imm_i = (int32_t)instruction >> 20;
    int32_t imm_s = ((int32_t)(instruction & 0xFE000000) >> 20) | ((instruction >> 7) & 0x1F);
    int32_t imm_b = ((int32_t)(instruction & 0x80000000) >> 19) | ((instruction & 0x80) << 4) | ((instruction >> 20) & 0x7E0) | ((instruction >> 7) & 0x1E);
    uint32_t imm_u = instruction & 0xFFFFF000;
    int32_t imm_j = ((int32_t)(instruction & 0x80000000) >> 11) | (instruction & 0xFF000) | ((instruction >> 9) & 0x800) | ((instruction >> 20) & 0x7FE);

    print_instruction(instruction);

    switch (opcode) {
        // ... [All existing cases remain the same] ...

        case OPCODE_SYSTEM:
            if (instruction == 0x00000073) {
                // ECALL
                uint32_t syscall_number = cpu->registers[17];  // a7 in RISC-V
                switch(syscall_number) {
                    case 56:  // openat in Linux
                        cpu->registers[10] = vfs_open((char*)&cpu->memory[cpu->registers[11]], cpu->registers[12]);
                        break;
                    case 57:  // close
                        cpu->registers[10] = vfs_close(cpu->registers[10]);
                        break;
                    case 63:  // read
                        cpu->registers[10] = vfs_read(cpu->registers[10], &cpu->memory[cpu->registers[11]], cpu->registers[12]);
                        break;
                    case 64:  // write
                        cpu->registers[10] = vfs_write(cpu->registers[10], &cpu->memory[cpu->registers[11]], cpu->registers[12]);
                        break;
                    default:
                        printf("Unhandled syscall: %d\n", syscall_number);
                        break;
                }
            } else if (instruction == 0x00100073) {
                // EBREAK
                printf("EBREAK instruction executed. Stopping emulation.\n");
                exit(0);
            }
            break;
    }

    // Ensure x0 is always 0
    cpu->registers[0] = 0;

    printf("After execution:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("x%d: %d\t", i, cpu->registers[i]);
        if ((i + 1) % 4 == 0) printf("\n");
    }
    printf("\n--------------------\n");
}

int main() {
    CPU cpu;
    initialize_cpu(&cpu);

    // Read the binary file
    FILE *file = fopen("output.bin", "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    fread(cpu.memory, 1, MEM_SIZE, file);
    fclose(file);

    // Execute instructions
    while (cpu.pc < MEM_SIZE) {
        uint32_t instruction = fetch(&cpu);
        if (instruction == 0) break;  // Stop if we reach a null instruction
        execute(&cpu, instruction);
    }

    return 0;
}