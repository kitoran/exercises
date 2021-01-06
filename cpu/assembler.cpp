#include <sys/mman.h>
#include "assembler.h"

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
