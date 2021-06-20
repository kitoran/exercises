#include "storearray.h"
#include <linux/limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <libexplain/open.h>
char hexchar(int d) {
    if(d < 10) {
        return '0' + d;
    } else {
        return 'a'+d - 10;
    }
}

char* hexhash(const unsigned char * hash) {
    static char res[16*2 + 1];
    for(int i = 0; i < 16; i++) {
        res[i*2] = hexchar(hash[i] >> 4);
        res[i*2+1] = hexchar(hash[i] & 0b1111);
    }
    res[sizeof(res)-1] = 0;
    return res;
}


void save(void *data, int size, const unsigned char *hash, char *type, int version) {
    char path[PATH_MAX];

    snprintf(path, PATH_MAX, "%s-%s-%d", hexhash(hash), type, version);
    fprintf(stderr, "saving %s", path);
    int d = explain_open_or_die(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    write(d, data, size);
    close(d);
}

bool load(const unsigned char *hash, char *type, int version, void **data, int *size_)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s-%s-%d", hexhash(hash), type, version);

    int d = open(path, O_RDONLY);
    if(d < 0) {
        return false;
    }

    struct stat st;
    fstat(d, &st);
    __off_t size = st.st_size;

    *data = malloc(size);
    read(d, *data, size);
    if(size_) {
        *size_ = size;
    }
    close (d);
  return true;
}

bool loadOne(const unsigned char *hash, char *type, int version, void *data)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s-%s-%d", hexhash(hash), type, version);

    int d = open(path, O_RDONLY);
    if(d < 0) {
        return false;
    }

    struct stat st;
    fstat(d, &st);
    __off_t size = st.st_size;

//    *data = malloc(size);
    read(d, data, size);

    close (d);
    return true;
}
