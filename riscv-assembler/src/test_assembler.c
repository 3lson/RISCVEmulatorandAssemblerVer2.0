#include <assert.h>
#include <stdio.h>

// Function prototypes for the assembly functions
unsigned int assemble_add(char *rd, char *rs1, char *rs2);
unsigned int assemble_sub(char *rd, char *rs1, char *rs2);
unsigned int assemble_lw(char *rd, char *offset, char *rs1);
unsigned int assemble_sw(char *rs2, char *offset, char *rs1);
unsigned int assemble_beq(char *rs1, char *rs2, char *offset);
unsigned int assemble_andi(char *rd, char *imm, char *rs1);
unsigned int assemble_ori(char *rd, char *imm, char *rs1);
unsigned int assemble_xor(char *rd, char *rs1, char *rs2);
unsigned int assemble_jal(char *rd, char *offset);

void test_assembler() {
    // Test add
    unsigned int result = assemble_add("x1", "x2", "x3");
    assert(result == 0x000303b3); // Expected machine code

    // Test sub
    result = assemble_sub("x4", "x5", "x6");
    assert(result == 0x000303b3); // Expected machine code (verify this)

    // Test lw
    result = assemble_lw("x7", "10", "x1");
    assert(result == 0x00a30303); // Expected machine code

    // Test sw
    result = assemble_sw("x4", "20", "x1");
    assert(result == 0x00a30223); // Expected machine code

    // Test beq
    result = assemble_beq("x1", "x2", "50");
    assert(result == 0x00300063); // Expected machine code

    // Test andi
    result = assemble_andi("x1", "5", "x2");
    assert(result == 0x0002d013); // Expected machine code

    // Test ori
    result = assemble_ori("x1", "5", "x2");
    assert(result == 0x0002d013); // Expected machine code

    // Test xor
    result = assemble_xor("x1", "x2", "x3");
    assert(result == 0x000303b3); // Expected machine code

    // Test jal
    result = assemble_jal("x1", "50");
    assert(result == 0x0030006f); // Expected machine code

    printf("All tests passed!\n");
}

// If you want to call test_assembler from another main function, comment this out.
// int main() {
//     test_assembler();
//     return 0;
// }
