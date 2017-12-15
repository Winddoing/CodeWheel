#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/*#include "openssl-rsakey.h"*/
/*#include "jz-rsakey.h"*/
#include "x2000-rsakey.h"

unsigned int dst[1024] = {0};

#if 0
/* spl hash md5 */
unsigned int src[] = {
	0x83c862d7,
	0x6d0304fc,
	0xaff642ea,
	0xf2a5ea7c,
};
#endif

#if 0
/* rsa enc nkr */
unsigned int src[] = {
		0x042c38de,
		0x1663bbe6,
		0x17bffb57,
		0x58883f97,
		0x0086b8b3,
		0x575dd308,
		0x308e21c4,
		0x9e75f07f,
		0xe0f7561d,
		0xb25c8f1e,
		0x0de4b431,
		0xce4df0a0,
		0x0cf07f2d,
		0x692712b3,
		0xb640e66e,
		0x503f67de,
		0xb8eaef6a,
		0xcea16e6a,
		0x4458eebc,
		0x05e308f0,
		0x20f39798,
		0x9c89292e,
		0xc83f357b,
		0x937ae80c,
		0xdd8621a6,
		0x05d52a27,
		0x8db80e4f,
		0x53853023,
		0xd4d16ae9,
		0x66783f82,
		0x00a428a2,
};
#endif

#if 0 // GET_ENCKEY
unsigned int src[] = {
		0xe8da50a3,
		0xd2692384,
		0x696ca64e,
		0xb79fc5bc,
		0x98dd66ac,
		0xb7f30515,
		0x1a396879,
		0x9632e46a,
		0x94bcba6e,
		0x628245cb,
		0xd34afdab,
		0x26f1fddf,
		0x39688476,
		0xd17f48ef,
		0xeee0e439,
		0xae271213,
		0x31272b31,
		0x73d47a65,
		0x71d0ebe2,
		0xfa8d145c,
		0xc6520722,
		0xe383aa1e,
		0x72b9b057,
		0x47f98cfc,
		0x099cd7e5,
		0x69b9dbaa,
		0xe5c2b79d,
		0x1b91696e,
		0xc84a612e,
		0x2c1c9e2f,
		0x002a3b9e,

};
#endif

#if  1
unsigned int src[] = {
		0xea5c02af,
		0x8b05446b,
		0x55b7f988,
		0xc0d99d03,
		0xa54d57b4,
		0x0c7dc97e,
		0x639c2c5f,
		0x6192489c,
		0xe64e8b17,
		0x8b745b61,
		0x0fb37ee0,
		0xd6f70a0b,
		0xd0d8f05e,
		0xb43f374a,
		0xf957d3bd,
		0x0c4abc62,
		0x49c66f4e,
		0x9bffce12,
		0xaa64930d,
		0x6ba4ef88,
		0xc319cd1d,
		0x250869c0,
		0xdd4ae982,
		0x1efa73ab,
		0x508f5f6e,
		0x8556ac44,
		0x93722174,
		0x1f38a738,
		0x9da31c08,
		0x5f621bf0,
		0x0027c763,

};
#endif

void rev_dump(unsigned int *d, unsigned int len)
{
	unsigned int i = 0;

	printf("\n\n");
	for(i = 0; i < len; i++) {
		if(i % 4 == 0){
			printf("\n\t");
		}
		if(i != len - 1)
			printf("0x%08x,",d[i]);
		else
			printf("0x%08x",d[i]);
	}

}

int main(int argc, char **argv)
{
	unsigned int i;
	unsigned int n_len, e_len, d_len, outlen;

	unsigned int n[128] = {0};
	unsigned int e[128] = {0};
	unsigned int d[128] = {0};

	n_len = sizeof(nn_key)/4;
	e_len = sizeof(pb_key)/4;
	d_len = sizeof(pr_key)/4;

	if(argc < 2) {
		printf("err arg ...\n");
		return -1;
	}

	if(strcmp(argv[1], "openssl") == 0) {
		printf("opessl rsakey ...\n");
		for(i = 0; i < n_len; i++)
			n[i] = nn_key[n_len - 1 - i];
		for(i = 0; i < e_len; i++)
			e[i] = pb_key[e_len - 1 - i];
		for(i = 0; i < d_len; i++)
			d[i] = pr_key[d_len - 1 - i];

		rev_dump(n, n_len);
		rev_dump(e, e_len);
		rev_dump(d, d_len);
	} else {
		printf("jz rsakey ...\n");
		for(i = 0; i < n_len; i++)
			n[i] = nn_key[i];
		for(i = 0; i < e_len; i++)
			e[i] = pb_key[i];
		for(i = 0; i < d_len; i++)
			d[i] = pr_key[i];
	}

	printf("\nrsa start ...\n");

	outlen = do_rsa(sizeof(src)/sizeof(int), src, e_len, e, n_len, n, dst);
	printf("++++++++nku++++++++  1 rsa length %d\n",outlen);
	for(i = 0; i < outlen; i++)
		printf("rsa 0x%08x\n",dst[i]);

/*	outlen = do_rsa(outlen, dst, d_len, d, n_len, n, dst);*/
	outlen = do_rsa(sizeof(src)/sizeof(int), src, d_len, d, n_len, n, dst);
	printf("++++++++nkr++++++++  2 rsa length %d\n",outlen);
	for(i = 0; i < outlen; i++)
		printf("rsa 0x%08x\n",dst[i]);

	return 0;
}
