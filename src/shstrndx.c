#include <elf.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ferr(void);
int elfstrip(char *);

int main(int argc, char **argv) {
	printf("This is a Proof-of-Concept for one of many GDB bugs that prevents it from reading binaries\n");
	elfstrip(*(argv+1));
	return 1;
}

/* elfstrip()
 *
 */
int elfstrip(char *path) {
	int x=0;
	Elf32_Ehdr ehdr;
	FILE *elf=NULL;

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
	
	ehdr.e_shstrndx=ehdr.e_shnum+1;
	rewind(elf);
	fwrite(&ehdr, 1, sizeof(ehdr), elf);
	(void) fclose(elf);
	return 1;
}

void ferr(void) {
	(void) fprintf(stderr, "file I/O failure\n");
	exit(-1);
}
