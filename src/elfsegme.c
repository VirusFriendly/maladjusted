#include <elf.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findseg(char *path);
int main(int argc, char **argv) {
	findseg(*(argv+1));
	return 1;
}

int findseg(char *path) {
	int x=0;
	Elf32_Ehdr ehdr;
	Elf32_Ehdr ehdr2;
	FILE *elf=NULL;
	int pos=0;
	Elf32_Shdr shdr;

	elf=fopen(path, "r+");

	if(elf == NULL) {
		perror("elfedit");
		exit(-1);
	}
	
	x=fread(&ehdr2, 1, sizeof(Elf32_Ehdr), elf);

	if(x != sizeof(Elf32_Ehdr)) {
		exit(-1);
	} else if(strncmp(ehdr2.e_ident, ELFMAG, SELFMAG) != 0) {
		(void) fprintf(stderr, "%s isnt an ELF file\n", path);
		exit(-1);
	}

	fseek(elf, 0, SEEK_END);
	pos=ftell(elf);
	fseek(elf, (pos-sizeof(Elf32_Shdr)), SEEK_SET);

	read(&shdr, 1, sizeof(Elf32_Shdr), elf);
	printf("name %x\n", shdr.sh_name);
	printf("type %x\n", shdr.sh_type);
	printf("addr %x\n", shdr.sh_addr);
	
	return 1;
}
