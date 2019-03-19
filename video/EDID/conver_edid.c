#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#if 1
void main(int argc, char* argv[])
{
    int i, j, k;
    char* edid[8] = {
        "00ffffffffffff002de130000200000001170103807341780acf74a755469824",
        "10494b210800818095009040a9c0a940b30061407140023a801871382d40582c",
        "4500c48e2100001e662150b051001b3040703600f61a1100001e000000fc0054",
        "565f4d4f4e49544f520a2020000000fd00324b0e5b12000a2020202020200166",
        "02032ef24d010384050710129314161f2022260907071117508301000070030c",
        "003000b82d20a0060141000000008c0ad08a20e02d10103e9600c48e21000018",
        "023a801871382d40582c4500c48e2100001e011d00bc52d01e20b8285540c48e",
        "2100001e011d80d0721c1620102c2580c48e2100009e000000000000000000a1"
    };

    for(j = 0; j < 8; j++) {
        for(i = 0; i < 32; i++) {
            printf("0x");

            for(k = 0; k < 2; k++) {
                printf("%c", edid[j][i * 2 + k]);
            }

            printf(",");

            if((i + 1) % 16 == 0)
                printf("\n");
        }
    }
}
#else
void main(int argc, char* argv[])
{
    FILE *edid = fopen(argv[1], "rb");
    unsigned char buf[512];
    int reti, i;

    printf("%s\n", argv[1]);
    ret = fread(buf, sizeof(unsigned char), 256, edid);
    printf("read ret %d\n", ret);

    for(i = 0; i < 256; i++) {
        printf("0x%02x,", buf[i]);

        if((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n");

    fclose(edid)
}
#endif
