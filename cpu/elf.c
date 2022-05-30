#include <elf.h>


//typedef struct {
//0               unsigned char e_ident[EI_NIDENT];
//16               uint16_t      e_type;
//18               uint16_t      e_machine;
//20               uint32_t      e_version;
//24               ElfN_Addr     e_entry;
//32               ElfN_Off      e_phoff;
//40               ElfN_Off      e_shoff;
//48               uint32_t      e_flags;
//52               uint16_t      e_ehsize;
//54               uint16_t      e_phentsize;
//56               uint16_t      e_phnum;
//58               uint16_t      e_shentsize;
//60               uint16_t      e_shnum;
//62               uint16_t      e_shstrndx;
//           } ElfN_Ehdr;

#define TO_FILL 0
Elf64_Ehdr elfHeader = { {EI_MAG0,EI_MAG1,EI_MAG2,EI_MAG3,
                       ELFCLASS64, ELFDATA2LSB, EV_CURRENT, /*ELFOSABI_LINUX*/ELFOSABI_NONE,
                       0},
                      ET_EXEC/*ET_REL*/, EM_X86_64, EV_CURRENT, .e_entry = TO_FILL,
                        .e_phoff = 0x00000040, .e_shoff = 0, .e_flags = 0, .e_ehsize = 0x0040,
                      .e_phentsize = 0x3000, .e_phnum =  0x0001,
                      .e_shentsize = 0, .e_shnum = 0, .e_shstrndx = SHN_UNDEF
                    };
Elf64_Phdr programHeader = {.p_type = PT_LOAD, .p_offset = sizeof(Elf64_Ehdr)+sizeof(Elf64_Phdr),
                           .p_vaddr = TO_FILL, .p_paddr = 0, .p_filesz = TO_FILL, .p_memsz = TO_FILL,
                           .p_flags = PF_X, .p_align = 0};
