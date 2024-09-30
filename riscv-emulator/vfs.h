#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_FILES 16
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 1024

typedef struct {
    char name[MAX_FILENAME];
    uint8_t data[MAX_FILE_SIZE];
    size_t size;
    bool is_open;
} VirtualFile;

typedef struct {
    VirtualFile files[MAX_FILES];
    int file_count;
} VirtualFileSystem;

void init_vfs();
int vfs_open(const char* filename, int flags);
int vfs_close(int fd);
ssize_t vfs_read(int fd, void* buf, size_t count);
ssize_t vfs_write(int fd, const void* buf, size_t count);

#endif // VFS_H