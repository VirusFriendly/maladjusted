#include <elf.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_class(char *);
void print_err(Elf32_Ehdr);
void print_etype(Elf32_Half);
void print_osabi(char *);

/* elfread()
 * visiblity: public
 * arguments: path [string]
 * calls:
 * 	<errno.h> perror()
 * 	<stdio.h> fopen(), fprintf(), fread(), printf()
 * 	<stdlib.h> exit()
 * 	<strings.h> strncmp()
 *
 * 	print_class(), print_etype(), print_osabi()
 * returns: always 0
 *
 * notes:
 * 	open file PATH, read only
 * 	read the elf header
 * 	close file PATH
 * 	print out the elf header
 */
int elfread(char *path) {
	int x=0;
	Elf32_Ehdr ehdr;
	FILE *elf=NULL;

	elf=fopen(path, "r");

	if(elf == NULL) {
		(void) fprintf(stderr, "fopen() error with %s", path);
		perror("elfedit");
		exit(-1);
	}
	
	x=fread(&ehdr, 1, sizeof(Elf32_Ehdr), elf);
	(void) fclose(elf);

	if(x != sizeof(Elf32_Ehdr)) {
		(void) fprintf(stderr, "fread() error with  %s", path);
		exit(-1);
	}

	if(strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
		return -1;
	}

	(void) printf("ELF ");
	(void) print_class(ehdr.e_ident+EI_CLASS);
	(void) printf("Version %x\n", *(ehdr.e_ident+EI_VERSION));
	(void) print_osabi(ehdr.e_ident+EI_OSABI);
	/* (void) printf("ABI Version %s\n", *(ehdr.e_ident+EI_ABIVERSION)); */
	(void) printf("Object file type: ");
	(void) print_etype(ehdr.e_type);
	(void) printf("Machine code: %d\n", ehdr.e_machine);
	(void) printf("Object File Version: %d\n", ehdr.e_version);
	(void) printf("Start Address: %x\n", ehdr.e_entry);
	(void) printf("Program header: %x\n", ehdr.e_phoff);
	(void) printf("Section header: %x\n", ehdr.e_shoff);
	(void) printf("Flags: %x\n", ehdr.e_flags);
	(void) printf("Program header table entry count: %d\n", ehdr.e_phnum);
	(void) printf("Section header table entry count: %d\n", ehdr.e_shnum);
	(void) printf("String table index: %d\n", ehdr.e_shstrndx);
	(void) print_err(ehdr);
	return 0;
}

/* print_class()
 * visibility: private
 * arguments: e_ident [string]
 * calls:
 * 	<stdio.h> printf()
 * returns: <void>
 *
 * notes:
 * 	just a helper function to clean up elfread()
 */
void print_class(char *e_ident) {
	switch(*e_ident) {
		case ELFCLASSNONE:
			(void) printf("invalid type\n");
		break;
		case ELFCLASS32:
			(void) printf("32-bit object\n");
		break;
		case ELFCLASS64:
			(void) printf("64-bit object\n");
		break;
		default:
			(void) printf("undefined type %x\n", *e_ident);
		break;
	}

	e_ident++;
	
	switch(*e_ident) {
		case ELFDATANONE:
			(void) printf("invalid data encoding\n");
		break;
		case ELFDATA2LSB:
			(void) printf("2's complement, little endian\n");
		break;
		case ELFDATA2MSB:
			(void) printf("2's complement, big endian\n");
		break;
		default:
			(void) printf("undefined type %x\n", *e_ident);
		break;
	}
}

void print_err(Elf32_Ehdr ehdr) {

	if(ehdr.e_ehsize != sizeof(Elf32_Ehdr)) {
		(void) printf("Elf header size doesnt match\n");
	}

	if(ehdr.e_phentsize != sizeof(Elf32_Phdr)) {
		(void) printf("Program header size doesnt match\n");
	}

	if(ehdr.e_shentsize != sizeof(Elf32_Shdr)) {
		(void) printf("Section header size doesnt match\n");
	}
}

/* print_osabi()
 * visibility: private
 * arguments: e_ident [string]
 * calls:
 * 	<stdio.h> printf()
 * returns: <void> 
 * 
 * notes:
 * 	just a helper function to clean up elfread()
 */ 
void print_osabi(char *e_ident) {
	switch((unsigned char) *e_ident) {
		case ELFOSABI_SYSV:
			(void) printf("UNIX System V ABI\n");
		break;
		case ELFOSABI_HPUX:
			(void) printf("HP-UX\n");
		break;
		case ELFOSABI_ARM:
			(void) printf("ARM\n");
		break;
		case ELFOSABI_STANDALONE:
			(void) printf("Standalone (embedded) application\n");
		break;
		default:
			(void) printf("undefined type %x\n", *e_ident);
		break;
	}
}
	
/* print_etype()
 * visibility: private
 * arguments: e_ident [string]
 * calls:
 * 	<stdio.h> printf()
 *	returns: <void> 
 * 
 * notes:
 * 	just a helper function to clean up elfread()
 */ 
void print_etype(Elf32_Half e_type) {
	switch(e_type) {
		case ET_NONE:
			(void) printf("No file type\n");
		break;
		case ET_REL:
			(void) printf("Relocatable file\n");
		break;
		case ET_EXEC:
			(void) printf("Executable file\n");
		break;
		case ET_DYN:
			(void) printf("Shared object file\n");
		break;
		case ET_CORE:
			(void) printf("Core file\n");
		break;
		case ET_NUM:
			(void) printf("Number of defined types\n");
		break;
		default:
			(void) printf("undefined type %d\n", e_type);
		break;
	}
}
