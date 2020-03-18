#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	long int offset=0;
	int size=0;
	char *mem=NULL;
	FILE *stream=NULL;

	if(argc != 3) {
		return -1;
	}

	stream=fopen("/dev/mem", "r");
	offset=strtol(*(argv+1), (char **)NULL, 16);
	size=strtol(*(argv+2), (char **)NULL, 16);
	mem=(char *) malloc(size);

	fseek(stream, offset, SEEK_SET);
	fread(mem, size, 1, stream);
	fwrite(mem, size, 1, stdout);
	return 1;
}
