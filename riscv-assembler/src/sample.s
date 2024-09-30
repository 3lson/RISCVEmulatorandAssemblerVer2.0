# Initialize registers
addi x1, x0, 10    # x1 = 10
addi x2, x0, 20    # x2 = 20

# Arithmetic operations
add x3, x1, x2     # x3 = x1 + x2 = 30
sub x4, x2, x1     # x4 = x2 - x1 = 10
mul x5, x1, x2     # x5 = x1 * x2 = 200

# Logical operations
and x6, x1, x2     # x6 = x1 & x2 = 0
or x7, x1, x2      # x7 = x1 | x2 = 30
xor x8, x1, x2     # x8 = x1 ^ x2 = 30

# Shifts
slli x9, x1, 2     # x9 = x1 << 2 = 40
srli x10, x2, 1    # x10 = x2 >> 1 = 10
srai x11, x2, 1    # x11 = x2 >> 1 (arithmetic) = 10

# Load and store
lui x12, 0x12345   # x12 = 0x12345000
sw x1, 0(x2)       # Store x1 at address in x2
lw x13, 0(x2)      # Load from address in x2 to x13

# Branches and jumps
beq x1, x2, 8      # Branch if x1 == x2 (not taken)
bne x1, x2, 8      # Branch if x1 != x2 (taken)
jal x14, 16        # Jump and link
jalr x15, 0(x14)   # Jump and link register

# Compare
slt x16, x1, x2    # Set if less than
sltu x17, x1, x2   # Set if less than (unsigned)

# Special instructions
fence
ecall
ebreak

# Additional arithmetic
addi x18, x0, -5   # Test negative immediate
auipc x19, 0x123   # Add upper immediate to PC

# Additional branches
blt x1, x2, 8      # Branch if less than
bge x2, x1, 8      # Branch if greater or equal
bltu x1, x2, 8     # Branch if less than (unsigned)
bgeu x2, x1, 8     # Branch if greater or equal (unsigned)

# Additional loads
lb x20, 1(x2)      # Load byte
lh x21, 2(x2)      # Load halfword
lbu x22, 1(x2)     # Load byte unsigned
lhu x23, 2(x2)     # Load halfword unsigned

# Additional stores
sb x1, 3(x2)       # Store byte
sh x1, 4(x2)       # Store halfword