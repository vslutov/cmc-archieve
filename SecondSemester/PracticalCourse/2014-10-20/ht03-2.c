// This code just not work!

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf.h>
#include <stab.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
sread(void *pbuffer, int size, int file)
{
    char *buffer = pbuffer;
    int r = 0;
    while ((size > 0) && (r = read(file, buffer, size)) > 0) {
        size -= r;
        buffer += r;
    }
    return size;
}

struct Stab
{
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uintptr_t n_value;
};

struct Function
{
    char *f_name;
    ssize_t f_index;
    ssize_t f_start;
    ssize_t f_end;
    char *f_source;
};

int
compar(const void *pa, const void *pb) {
    struct Function a = *(const struct Function *)pa;
    struct Function b = *(const struct Function *)pb;

    int result = strcmp(a.f_name, b.f_name);
    if (result != 0) {
        return result;
    } else {
        return b.f_start - a.f_start;
    }
}

int
main(int argc, char **argv)
{
    char *path = argv[1];

    int elf_file = open(path, O_RDONLY, 0);
    Elf32_Ehdr elf_header;
    sread(&elf_header, sizeof(elf_header), elf_file);

    Elf32_Shdr *elf_sections = calloc(
        elf_header.e_shnum,
        elf_header.e_shentsize);

    lseek(elf_file, elf_header.e_shoff, SEEK_SET);
    sread(elf_sections, elf_header.e_shentsize * elf_header.e_shnum, elf_file);

    Elf32_Shdr elf_shstr_header = elf_sections[elf_header.e_shstrndx];
    char *shnames = calloc(elf_shstr_header.sh_size, 1);
    lseek(elf_file, elf_shstr_header.sh_offset, SEEK_SET);
    sread(shnames, elf_shstr_header.sh_size, elf_file);

    struct Stab *stab = NULL;
    ssize_t stab_count = 0;
    char *stabstr = NULL;

    for (uint16_t i = 1; i < elf_header.e_shnum; ++i) {
        if (strcmp(shnames + elf_sections[i].sh_name, ".stab") == 0) {
            Elf32_Shdr stab_section = elf_sections[i];
            stab = calloc(stab_section.sh_size, 1);
            lseek(elf_file, stab_section.sh_offset, elf_file);
            sread(stab, stab_section.sh_size, elf_file);
            stab_count = stab_section.sh_size / sizeof(*stab);
        }

        if (strcmp(shnames + elf_sections[i].sh_name, ".stabstr") == 0) {
            Elf32_Shdr stabstr_section = elf_sections[i];
            stabstr = calloc(stabstr_section.sh_size, 1);
            lseek(elf_file, stabstr_section.sh_offset, SEEK_SET);
            sread(stabstr, stabstr_section.sh_size, elf_file);
        }
    }

    if (stab == NULL || stabstr == NULL) {
        printf("No debug info\n");
        return 0;
    }

    char *filename = NULL;
    struct Function *functions = calloc(stab_count, sizeof(*functions));
    ssize_t function_count = 0;
    bool set_source = true;
    bool set_end = true;

    for (ssize_t i = 0; i < stab_count; ++i) {
        if (stab[i].n_type == N_SO || stab[i].n_type == N_SOL) {
            if (
                stab[i].n_type == N_SO &&
                function_count > 0 &&
                set_end == false) {

                functions[function_count - 1].f_end = stab[i].n_value;
                set_end = true;
            }

            filename = stabstr + stab[i].n_strx;
        } else if (stab[i].n_type == N_FUN) {
            char *name = stabstr + stab[i].n_strx;
            *strchr(name, ':') = 0;
            functions[function_count].f_name = name;
            functions[function_count].f_index = i;
            functions[function_count].f_start = stab[i].n_value;

            if (function_count > 0) {
                functions[function_count - 1].f_end = stab[i].n_value;
            }

            set_source = false;
            set_end = false;
            ++function_count;
        } else if (stab[i].n_type == N_SLINE) {
            if (function_count > 0 && set_source == false) {
                functions[function_count - 1].f_source = filename;
                set_source = true;
            }
        }
    }

    qsort(functions, function_count, sizeof(*functions), compar);

    for (ssize_t i = 0; i < function_count; ++i) {
        printf(
            "%s %d 0x%08x 0x%08x %s\n",
            functions[i].f_name,
            functions[i].f_index,
            functions[i].f_start,
            functions[i].f_end,
            functions[i].f_source);
    }

    return 0;
}

