#include <stdio.h>
#include <stdlib.h>

int main() {
    // Array of hexadecimal instructions
    const char *instructions[] = {
        "fff000b3", // Add instruction
        "fff00133", // Add immediate instruction
        "fff080b3", // Other instructions...
        "fff080b3",
        "fff080b3",
        "fff080b3",
        "fff10133",
        "fff00133",
        "fff080b3",
        "fff10133",
        "fff080b3",
        "fff10133",
        "fff081b3"
    };

    FILE *file = fopen("output.bin", "wb");
    if (!file) {
        perror("Failed to create output.bin");
        return 1;
    }

    // Loop through each instruction and write to the binary file
    for (int i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++) {
        unsigned int instruction;
        sscanf(instructions[i], "%x", &instruction); // Convert hex string to unsigned int

        // Write instruction in little-endian format
        unsigned char bytes[4];
        bytes[0] = (instruction & 0xFF);         // Least significant byte
        bytes[1] = (instruction >> 8) & 0xFF;
        bytes[2] = (instruction >> 16) & 0xFF;
        bytes[3] = (instruction >> 24) & 0xFF;  // Most significant byte

        fwrite(bytes, sizeof(unsigned char), 4, file); // Write 4 bytes
    }

    fclose(file);
    printf("output.bin created successfully.\n");
    return 0;
}
