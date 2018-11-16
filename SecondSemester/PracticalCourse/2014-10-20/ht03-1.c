#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned char e_ident[16];

    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf32_Ehdr;

enum {
    ELF_TYPE_COUNT = 5
};

const char *elf_types[ELF_TYPE_COUNT] = {
    "NONE",
    "REL",
    "EXEC",
    "DYN",
    "CORE"
};

typedef struct
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Shdr;

typedef struct
{
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} Elf32_Phdr;

int
main(int argc, char **argv)
{
    char *path = argv[1];

    FILE *elf_file = fopen(path, "rb");
    if (elf_file == NULL) {
        fprintf(stderr, "Couldn't open input file\n");
        return 1;
    }

    Elf32_Ehdr elf_header;
    size_t count = fread(&elf_header, sizeof(elf_header), 1, elf_file);
    if (count != 1) {
        fprintf(stderr, "Error while reading ELF Header\n");
        return 1;
    }

    if (elf_header.e_ident[0] != '\x7f' ||
        elf_header.e_ident[1] != 'E' ||
        elf_header.e_ident[2] != 'L' ||
        elf_header.e_ident[3] != 'F') {

        fprintf(stderr, "Not an ELF file\n");
        return 1;
    }


    if (elf_header.e_ident[4] != 1 ||
        elf_header.e_ident[5] != 1 ||
        elf_header.e_ident[6] != 1 ||
        elf_header.e_ident[7] != 0 ||
        elf_header.e_ident[8] != 0 ||
        elf_header.e_type > ELF_TYPE_COUNT ||
        elf_header.e_machine != 3 ||
        elf_header.e_version != 1 ||
        elf_header.e_flags != 0 ||
        elf_header.e_ehsize != sizeof(Elf32_Ehdr) ||

        (elf_header.e_phentsize != sizeof(Elf32_Phdr) &&
        elf_header.e_phentsize != 0) ||

        (elf_header.e_shentsize != sizeof(Elf32_Shdr) &&
        elf_header.e_shentsize != 0)
        ) {

        fprintf(stderr, "Not supported ELF file\n");
        return 1;
    }

    printf("TYPE: %s\n", elf_types[elf_header.e_type]);
    printf("                NAME       ADDR     OFFSET       SIZE   ALGN\n");

    Elf32_Shdr *elf_sections = calloc(
        elf_header.e_shnum,
        elf_header.e_shentsize);

    fseek(elf_file, elf_header.e_shoff, SEEK_SET);
    fread(elf_sections, elf_header.e_shentsize, elf_header.e_shnum, elf_file);

    Elf32_Shdr elf_shstr_header = elf_sections[elf_header.e_shstrndx];
    char *shnames = calloc(elf_shstr_header.sh_size, 1);
    fseek(elf_file, elf_shstr_header.sh_offset, SEEK_SET);
    fread(shnames, 1, elf_shstr_header.sh_size, elf_file);

    for (uint16_t i = 1; i < elf_header.e_shnum; ++i) {
        printf(
            "%20s 0x%08x %10d %10d 0x%04x\n",
            shnames + elf_sections[i].sh_name,
            elf_sections[i].sh_addr,
            elf_sections[i].sh_offset,
            elf_sections[i].sh_size,
            elf_sections[i].sh_addralign);
    }

    return 0;
}

