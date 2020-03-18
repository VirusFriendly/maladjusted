#include <elf.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void elfwrite(FILE *, Elf32_Ehdr);
void ferr(void);

/* elfstrip()
 *
 */
int elfstrip(char *path) {
	int x=0;
	Elf32_Ehdr ehdr;
	FILE *elf=NULL;
	char *blankeident="\177ELF\0\0\0\0\0\0\0\0\0\0";

	elf=fopen(path, "r+");

	if(elf == NULL) {
		perror("elfedit");
		exit(-1);
	}
	
	x=fread(&ehdr, 1, sizeof(Elf32_Ehdr), elf);
	
	if(x != sizeof(Elf32_Ehdr)) {
		ferr();
	} else if(strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
		fprintf(stderr, "%s isnt an ELF file\n", path);
		exit(-1);
	}
	
	(void)memcpy(ehdr.e_ident, blankeident, EI_NIDENT);
	ehdr.e_version=0;
	ehdr.e_shoff=0;
	ehdr.e_ehsize=0;
	ehdr.e_shentsize=0; 
	elfwrite(elf, ehdr);
	return 1;
}

int elfpatch(char *patch, char *path) {
	int x=0;
	Elf32_Ehdr ehdr;
	Elf32_Ehdr ehdr2;
	FILE *elf=NULL;

	elf=fopen(patch, "r");

	if(elf == NULL) {
		perror("elfedit");
		exit(-1);
	}
	
	x=fread(&ehdr, 1, sizeof(Elf32_Ehdr), elf);

	if(x != sizeof(Elf32_Ehdr)) {
		ferr();
	} else if(strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
		fprintf(stderr, "%s isnt an ELF file\n", patch);
		exit(-1);
	}
	
	(void) fclose(elf);
	elf=fopen(path, "r+");

	if(elf == NULL) {
		perror("elfedit");
		exit(-1);
	}
	
	x=fread(&ehdr2, 1, sizeof(Elf32_Ehdr), elf);

	if(x != sizeof(Elf32_Ehdr)) {
		ferr();
	} else if(strncmp(ehdr2.e_ident, ELFMAG, SELFMAG) != 0) {
		(void) fprintf(stderr, "%s isnt an ELF file\n", path);
		exit(-1);
	}
	
	(void)memcpy(ehdr2.e_ident, ehdr.e_ident, sizeof(ehdr.e_ident));
	(void)memcpy(ehdr2.e_ident+EI_ABIVERSION, "RealWarp", 8);
	ehdr2.e_type=ehdr.e_type;
	ehdr2.e_machine=ehdr.e_machine;
	ehdr2.e_version=ehdr.e_version;
	ehdr2.e_ehsize=sizeof(Elf32_Ehdr);
	ehdr2.e_phentsize=sizeof(Elf32_Phdr);
	ehdr2.e_shentsize=sizeof(Elf32_Shdr);
	elfwrite(elf, ehdr2);
	return 1;
}

void elfwrite(FILE *elf, Elf32_Ehdr ehdr) {
	int x=0;
	
	rewind(elf);
	x=fwrite(&ehdr, 1, sizeof(ehdr), elf);
	
	(void) fclose(elf);
}

void ferr(void) {
	(void) fprintf(stderr, "file I/O failure\n");
	exit(-1);
}
