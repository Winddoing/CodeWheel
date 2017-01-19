#include <stdio.h>

int main (int argc, char **argv)
{
	int i;

	printf("1111---: argc=%d\n", argc);
	printf("2222---: argv=%s\n", argv[0]);

	printf("v: ");
	for(i = 0; i < argc; i++) {
		printf("%s", argv[i]);
	}
	printf("\n");

	while (--argc > 0 && **++argv == '-') {
		printf("<1>: argc=%d, argv=%c\n", argc, **argv);
		while (*++*argv) {
			printf("<2>: argv=%c\n", **argv);
			switch (**argv) {
				case 'l':
					printf("-----------------lllllll\n");
					break;
				case 'A':
					printf("-----------------AAAAAAA\n");
					break;
				case 'c':
					printf("-----------------ccccccc\n");
					break;
			}
		}
	}

	return 0;
}
