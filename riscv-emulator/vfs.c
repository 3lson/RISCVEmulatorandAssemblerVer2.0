#include "vfs.h"
#include <string.h>

static VirtualFileSystem vfs;

void init_vfs() {
    vfs.file_count = 0;
    memset(vfs.files, 0, sizeof(vfs.files));
}

int vfs_open(const char* filename, int flags) {
    for (int i = 0; i < vfs.file_count; i++) {
        if (strcmp(vfs.files[i].name, filename) == 0) {
            if (!vfs.files[i].is_open) {
                vfs.files[i].is_open = true;
                return i; 
            }
            return -1; 
        }
    }
    
    if (vfs.file_count < MAX_FILES) {
        strncpy(vfs.files[vfs.file_count].name, filename, MAX_FILENAME - 1);
        vfs.files[vfs.file_count].name[MAX_FILENAME - 1] = '\0'; 
        vfs.files[vfs.file_count].size = 0;
        vfs.files[vfs.file_count].is_open = true;
        return vfs.file_count++;
    }
    
    return -1;  
}

int vfs_close(int fd) {
    if (fd >= 0 && fd < vfs.file_count && vfs.files[fd].is_open) {
        vfs.files[fd].is_open = false;
        return 0;
    }
    return -1;
}

ssize_t vfs_read(int fd, void* buf, size_t count) {
    if (fd >= 0 && fd < vfs.file_count && vfs.files[fd].is_open) {
        size_t to_read = count > vfs.files[fd].size ? vfs.files[fd].size : count;
        memcpy(buf, vfs.files[fd].data, to_read);
        return to_read;
    }
    return -1;
}

ssize_t vfs_write(int fd, const void* buf, size_t count) {
    if (fd >= 0 && fd < vfs.file_count && vfs.files[fd].is_open) {
        size_t to_write = count > MAX_FILE_SIZE ? MAX_FILE_SIZE : count;
        memcpy(vfs.files[fd].data, buf, to_write);
        vfs.files[fd].size = to_write;
        return to_write;
    }
    return -1;
}