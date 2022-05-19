#include <stdio.h>

#define TEST 1
#if TEST
#include <string.h>
#endif

#define PRINT printf
#define HEXDUMP_LINE_LEN 16

static int chk_patt(const char *buf, unsigned int len, char chr)
{
	while (len--) {
		if (buf[len] != chr)
			return 0;
	}
	return 1;
}

static void pr_line(const char *buf, unsigned int len, unsigned int addr)
{
	int i;
	unsigned char chr;

	PRINT("0x%.8X: ", addr);

	for (i = 0; i < HEXDUMP_LINE_LEN; i++) {
		chr = (unsigned char)buf[i];

		if (i < len)
			PRINT("%.2x ", chr);
		else
			PRINT("   ");
	}

	PRINT("|");
	for (i = 0; i < HEXDUMP_LINE_LEN; i++) {
		if (i < len) {
			if (buf[i] >= 0x20 && buf[i] <= 0x7E)
				PRINT("%c", buf[i]);
			else
				PRINT(".");
		} else {
			PRINT(" ");
		}
	}
	PRINT("|\n");
}

void hexdump(const char *buf, unsigned int length)
{
	unsigned int addr = 0, len;
	char chr, ommit = 0, same = 0;

	if (!length)
		return;

	for (; length; addr += len, buf += len, length -= len) {
		len = length < HEXDUMP_LINE_LEN ? length: HEXDUMP_LINE_LEN;

		if (same && chk_patt(buf, len, chr)) {
			if (ommit)
				continue;
			PRINT("0x%.8X: ", addr);
			PRINT("*\n");
			ommit = 1;
			continue;
		}
		
		pr_line(buf, len, addr);
		if (chk_patt(buf, len, buf[0])) {
			ommit = 0;
			same = 1;
			chr = buf[0];
		} else {
			same = ommit = 0;
		}
	}
	PRINT("0x%.8X:\n", addr);
}

#if TEST
int main(int argc, char **argv)
{
	char buf[1024];

	PRINT("--- case 0 ---\n");
	memset(buf, 0xDD, 1024);
	hexdump(buf, 1024);

	PRINT("--- case 1 ---\n");
	memset(buf, 0xAA, 1024);
	buf[512] = 0;
	buf[12] = 'a';
	buf[312] = 'b';
	memcpy(buf + 793, "hexdump", sizeof("hexdump"));
	hexdump(buf, 1024);
	return 0;
}
#endif
