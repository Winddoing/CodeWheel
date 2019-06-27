/*#############################################################
 *     File Name	: string_len.cpp
 *     Author		: winddoing
 *     Created Time	: 2019年06月27日 星期四 10时17分38秒
 *     Description	:
 *############################################################*/

#include <iostream>
#include <string>

using namespace std;

static int set_cell(int sz, string & str)
{
	int i = 0;
	char s[50] = { 0 };

	for (i = 0; i < sz; i++) {
		sprintf(s, "%d", sz);
		str.append(s);
	}

	return (int)str.length();
}

int main(int argc, const char *argv[])
{
	string str;
	int i = 0, sz = 0, pos = 0;

	for (i = 0; i < 16; i++) {
		sz = set_cell(i, str);

		if (!((i + 1) % 4)) {
			string zz;
			zz.append("swq");

			str.insert(pos, zz);
			pos = str.length();
		}
	}

	printf("str: %s\n", str.c_str());
	printf("sz=%d, size=%ld, length=%ld\n", sz, str.size(), str.length());

	return 0;
}
