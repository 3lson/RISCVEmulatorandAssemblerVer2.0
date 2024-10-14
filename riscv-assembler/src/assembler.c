#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define MAX_INSTRUCTIONS 1000

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

// Function to convert register name to number
int reg_to_num(char *reg) {
    if (reg[0] == 'x') {
        return atoi(reg + 1);
    }
    return -1;
}

// Function to generate R-type instruction
unsigned int generate_r_type(int opcode, int rd, int funct3, int rs1, int rs2, int funct7) {
    return (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

// Function to generate I-type instruction
unsigned int generate_i_type(int opcode, int rd, int funct3, int rs1, int imm) {
    return (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

// Function to generate S-type instruction
unsigned int generate_s_type(int opcode, int funct3, int rs1, int rs2, int imm) {
    return ((imm & 0xFE0) << 20) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | ((imm & 0x1F) << 7) | opcode;
}

// Function to generate B-type instruction
unsigned int generate_b_type(int opcode, int funct3, int rs1, int rs2, int imm) {
    return ((imm & 0x1000) << 19) | ((imm & 0x7E0) << 20) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | ((imm & 0x1E) << 7) | ((imm & 0x800) >> 4) | opcode;
}

// Function to generate U-type instruction
unsigned int generate_u_type(int opcode, int rd, int imm) {
    return (imm << 12) | (rd << 7) | opcode;
}

// Function to generate J-type instruction
unsigned int generate_j_type(int opcode, int rd, int imm) {
    return ((imm & 0x100000) << 11) | ((imm & 0xFF000)) | ((imm & 0x800) << 9) | ((imm & 0x7FE) << 20) | (rd << 7) | opcode;
}

int main() {
    FILE *input_file = fopen("sample.s", "r");
    if (input_file == NULL) {
        printf("Error opening input file\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    unsigned int instructions[MAX_INSTRUCTIONS];
    int instruction_count = 0;

    while (fgets(line, sizeof(line), input_file)) {
        char *token;
        char *op, *rd, *rs1, *rs2;
        int imm;

        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;

        // Tokenize the line
        token = strtok(line, " ,\t\n");
        if (token == NULL) continue;

        op = token;
        rd = strtok(NULL, " ,\t\n");
        rs1 = strtok(NULL, " ,\t\n");
        rs2 = strtok(NULL, " ,\t\n");

        // R-type instructions
        if (strcmp(op, "add") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_ADD, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "sub") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_ADD, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_SUB);
        } else if (strcmp(op, "sll") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_SLL, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "slt") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_SLT, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "sltu") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_SLTU, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "xor") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_XOR, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "srl") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_SRL, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_SRL);
        } else if (strcmp(op, "sra") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_SRL, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_SRA);
        } else if (strcmp(op, "or") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_OR, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        } else if (strcmp(op, "and") == 0) {
            instructions[instruction_count++] = generate_r_type(OPCODE_OP, reg_to_num(rd), FUNCT3_AND, reg_to_num(rs1), reg_to_num(rs2), FUNCT7_ADD);
        }
        // I-type instructions
        else if (strcmp(op, "addi") == 0 || strcmp(op, "slti") == 0 || strcmp(op, "sltiu") == 0 ||
                 strcmp(op, "xori") == 0 || strcmp(op, "ori") == 0 || strcmp(op, "andi") == 0) {
            imm = atoi(rs2);
            int funct3 = (op[0] == 'a') ? FUNCT3_ADD : 
                         (op[0] == 's' && op[3] == 'i') ? FUNCT3_SLT :
                         (op[0] == 's' && op[3] == 'u') ? FUNCT3_SLTU :
                         (op[0] == 'x') ? FUNCT3_XOR :
                         (op[0] == 'o') ? FUNCT3_OR : FUNCT3_AND;
            instructions[instruction_count++] = generate_i_type(OPCODE_OP_IMM, reg_to_num(rd), funct3, reg_to_num(rs1), imm);
        } else if (strcmp(op, "slli") == 0 || strcmp(op, "srli") == 0 || strcmp(op, "srai") == 0) {
            imm = atoi(rs2);
            int funct3 = (op[1] == 'l') ? FUNCT3_SLL : FUNCT3_SRL;
            int funct7 = (op[2] == 'a') ? FUNCT7_SRA : FUNCT7_SRL;
            instructions[instruction_count++] = generate_r_type(OPCODE_OP_IMM, reg_to_num(rd), funct3, reg_to_num(rs1), imm & 0x1F, funct7);
        } else if (strcmp(op, "lb") == 0 || strcmp(op, "lh") == 0 || strcmp(op, "lw") == 0 ||
                   strcmp(op, "lbu") == 0 || strcmp(op, "lhu") == 0) {
            imm = atoi(rs1);
            rs1 = rs2;
            int funct3 = (op[1] == 'b' && op[2] == '\0') ? FUNCT3_LB :
                         (op[1] == 'h' && op[2] == '\0') ? FUNCT3_LH :
                         (op[1] == 'w') ? FUNCT3_LW :
                         (op[1] == 'b' && op[2] == 'u') ? FUNCT3_LBU : FUNCT3_LHU;
            instructions[instruction_count++] = generate_i_type(OPCODE_LOAD, reg_to_num(rd), funct3, reg_to_num(rs1), imm);
        } else if (strcmp(op, "jalr") == 0) {
            imm = atoi(rs2);
            instructions[instruction_count++] = generate_i_type(OPCODE_JALR, reg_to_num(rd), 0, reg_to_num(rs1), imm);
        }
        // S-type instructions
        else if (strcmp(op, "sb") == 0 || strcmp(op, "sh") == 0 || strcmp(op, "sw") == 0) {
            imm = atoi(rs2); 
            int funct3 = (op[1] == 'b') ? FUNCT3_SB : (op[1] == 'h') ? FUNCT3_SH : FUNCT3_SW;
            instructions[instruction_count++] = generate_s_type(OPCODE_STORE, funct3, reg_to_num(rs1), reg_to_num(rd), imm);
        }
        // B-type instructions
        else if (strcmp(op, "beq") == 0 || strcmp(op, "bne") == 0 || strcmp(op, "blt") == 0 ||
                 strcmp(op, "bge") == 0 || strcmp(op, "bltu") == 0 || strcmp(op, "bgeu") == 0) {
            imm = atoi(rs2);
            rs2 = rs1;
            rs1 = rd;
            int funct3 = (op[1] == 'e') ? FUNCT3_BEQ :
                         (op[1] == 'n') ? FUNCT3_BNE :
                         (op[1] == 'l' && op[3] == '\0') ? FUNCT3_BLT :
                         (op[1] == 'g' && op[3] == '\0') ? FUNCT3_BGE :
                         (op[1] == 'l' && op[3] == 'u') ? FUNCT3_BLTU : FUNCT3_BGEU;
            instructions[instruction_count++] = generate_b_type(OPCODE_BRANCH, funct3, reg_to_num(rs1), reg_to_num(rs2), imm);
        }
        // U-type instructions
        else if (strcmp(op, "lui") == 0) {
            imm = atoi(rs1);
            instructions[instruction_count++] = generate_u_type(OPCODE_LUI, reg_to_num(rd), imm);
        } else if (strcmp(op, "auipc") == 0) {
            imm = atoi(rs1);
            instructions[instruction_count++] = generate_u_type(OPCODE_AUIPC, reg_to_num(rd), imm);
        }
        // J-type instruction
        else if (strcmp(op, "jal") == 0) {
            imm = atoi(rs1);
            instructions[instruction_count++] = generate_j_type(OPCODE_JAL, reg_to_num(rd), imm);
        }
        // Special instructions
        else if (strcmp(op, "fence") == 0) {
            instructions[instruction_count++] = 0x0000000F; // Simple FENCE instruction
        } else if (strcmp(op, "ecall") == 0) {
            instructions[instruction_count++] = 0x00000073; // ECALL instruction
        } else if (strcmp(op, "ecall") == 0) {
            instructions[instruction_count++] = 0x00000073; // ECALL instruction
        } else if (strcmp(op, "ebreak") == 0) {
            instructions[instruction_count++] = 0x00100073; // EBREAK instruction
        } else {
            printf("Unsupported instruction: %s\n", op);
        }
    }

    fclose(input_file);

    // Write machine code to output file
    FILE *output_file = fopen("output.bin", "wb");
    if (output_file == NULL) {
        printf("Error opening output file\n");
        return 1;
    }

    fwrite(instructions, sizeof(unsigned int), instruction_count, output_file);
    fclose(output_file);

    printf("Assembly converted to machine code. Output written to output.bin\n");
    printf("Total instructions: %d\n", instruction_count);

    return 0;
}