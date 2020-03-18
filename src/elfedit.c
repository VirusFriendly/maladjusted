#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int elfstrip(char *);
int elfpatch(char *, char *);
int elfread(char *);
void userinfo(void);

/* main()
 * visibility: public
 * arguments: argc [int], argv [string pointer]
 * calls:
 * 	<unistd.h> exit(), getopt()
 *
 * 	elfpatch(), elfread(), elfstrip(), userinfo()
 * returns: always returns 0
 */
int main(int argc, char **argv) {
	int opt=0;
	
	opt=getopt(argc, argv, "sp");
	switch(opt) {
		case 's':
			if(argc-optind == 1) {
				elfstrip(*(argv+optind));
			} else {
				userinfo();
			}
			break;
		case 'p':
			if(argc-optind == 2) {
				elfpatch(*(argv+optind), *(argv+optind+1));
			} else {
				userinfo();
			}
			break;
		case '?':
			userinfo();
			break;
		default:
			if(argc-optind == 1) {
				elfread(*(argv+optind));
			} else {
				userinfo();
			}
	}
	exit(1);
}

/* userinfo()
 * visibility: private
 * arguments: [void]
 * calls:
 * 	<stdio.h> fprintf()
 * return: [void]
 *
 * notes:
 * 	just a help program to spit out directions
 */
void userinfo(void) {
	(void) fprintf(stderr, " elfedit (c)2001 footclan\n");
	(void) fprintf(stderr, " by Maetrics <maetrics@realwarp.net>\n");
	(void) fprintf(stderr, " http://footclan.realwarp.net/\n\n");
	(void) fprintf(stderr, "elfedit filename ");
	(void) fprintf(stderr, "- to view the elf header.\n");
	(void) fprintf(stderr, "elfedit -s filename ");
	(void) fprintf(stderr, "- to strip the elf header.\n");
	(void) fprintf(stderr, "elfedit -p filename1 filename2 ");
	(void) fprintf(stderr, "- to patch filename2 with filename1's elf header\n");
}
