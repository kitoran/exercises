#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "assembler.h"
#include <libexplain/open.h>

Assembler allocateSomeExecutablePages()
{
    const int initialSize = 4096*10;
    char* buf = (char*)mmap(0, initialSize, PROT_EXEC | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    Assembler res;
    res.mem = buf+4096*5;
    res.size = 4096*5;
    return res;
}

void Assembler::dump() {
    static int file = 0;
    if(file == 0) {
        file = explain_open_or_die("/home/n/dump", O_WRONLY | O_CREAT, 0666);
    }
    lseek(file, 0, SEEK_SET);
    ftruncate(file, 0);
    write(file, mem, position);
}
