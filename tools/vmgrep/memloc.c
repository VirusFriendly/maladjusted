#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	char s1[1024];
	char s2[1024];
	int i=0;
	int c=0;
	int n=1024;
	long int offset=0;
	FILE *stream=NULL;

	stream=fopen("/dev/mem", "r");
	fread(s1, n, 1, stdin);
	printf("%8x", 0);
	
	c=getc(stream);
	while(c != EOF) {
		if(c == s1[0]) {
			offset=ftell(stream);
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
			printf("%8x");
			fseek(stream, offset-1, SEEK_SET);
			fread(s2, n, 1, stream);
			
			if(memcmp(s1, s2, n) == 0) {
				printf("\n%d length signature found at %x\n\a", n, offset-1);
			}

			for(i=0; i < 1025; i++) {
				s2[i]=0;
			}
			
			fseek(stream, offset, SEEK_SET);
		}
		c=getc(stream);
	}

	for(i=0; i < 1025; i++) {
		s1[i]=0;
	}
	
	return 0;
}
