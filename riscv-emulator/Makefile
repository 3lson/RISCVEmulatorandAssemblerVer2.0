CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=riscv-emulator

all: $(TARGET)

$(TARGET): emulator.c
	$(CC) $(CFLAGS) -o $(TARGET) emulator.c

clean:
	rm -f $(TARGET)