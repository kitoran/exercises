#include <elf.h>

Elf64_Ehdr header = { {EI_MAG0,EI_MAG1,EI_MAG2,EI_MAG3,
                       ELFCLASS64, ELFDATA2LSB, EV_CURRENT, /*ELFOSABI_LINUX*/ELFOSABI_NONE,
                       0},
                      ET_EXEC/*ET_REL*/, EM_X86_64, EV_CURRENT, .e_entry = 0,

                    };
