/*#############################################################
 *     File Name	: bmp2rgb565.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月30日 星期三 16时33分18秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("bmp2rgb565 [BMP_SRC] [RGB565_DEST]\n");
        return 1;
    }

    FILE* src = NULL;
    FILE* dst = NULL;
    unsigned char buffer[5];
    int initOffsetBMP;

    src = fopen(argv[1], "r");
    dst = fopen(argv[2], "w");

    if(src == NULL)
    {
        printf("Unable to open the BMP file\n");
        return 2;
    }

    if(dst == NULL)
    {
        printf("Unable to open the RGB565 file\n");
        return 3;
    }

    printf("Reading BMP\n");

    fseek(src, 0x0A, SEEK_SET);
    buffer[0] = getc(src);
    buffer[1] = getc(src);
    buffer[2] = getc(src);
    buffer[3] = getc(src);
    printf("Pixel array offset %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);
    initOffsetBMP = buffer[0] + (buffer[1] >> 8) + (buffer[2] >> 16) + (buffer[3] >> 24);

    fseek(src, 0x12, SEEK_SET);
    buffer[0] = getc(src);
    buffer[1] = getc(src);
    buffer[2] = getc(src);
    buffer[3] = getc(src);
    printf("Width %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);

    //fseek(src, 0x12, SEEK_SET);
    buffer[0] = getc(src);
    buffer[1] = getc(src);
    buffer[2] = getc(src);
    buffer[3] = getc(src);
    printf("Height %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);

    fseek(src, 0x1C, SEEK_SET);
    buffer[0] = getc(src);
    buffer[1] = getc(src);
    printf("Bits Per Pixel %02x %02x\n", buffer[0], buffer[1]);

    if(buffer[1] != 0 || buffer[0] != 0x18) //biBitCount=24
    {
        printf("Could not read BMP\n");
        return 1;
    }

    fseek(src, initOffsetBMP, SEEK_SET);

    printf("Now reading BMP pixels and printing them to the RBG565 file\n");
    while(1)
    {
        buffer[0] = getc(src);
        buffer[1] = getc(src);
        buffer[2] = getc(src);
        if(feof(src))
            break;
        printf("Pixel BMP 24 bits R %02X G %02X B %02X ", buffer[2], buffer[1], buffer[0]);
        buffer[2] = buffer[2] >> 3; //R
        buffer[1] = buffer[1] >> 2; //G
        buffer[0] = buffer[0] >> 3; //B
        printf("-> RGB 565 R %02X G %02X B %02X ", buffer[2], buffer[1], buffer[0]);
        buffer[3] = ((buffer[0] & 0x1F) << 3) + ((buffer[1] & 0x38) >> 3);
        buffer[4] = ((buffer[1] & 0x07) << 5) + (buffer[2] & 0x1F);
        printf("-> RGB 565 M %02X%02X\n", buffer[3], buffer[4]);
        fprintf(dst, "%c%c", buffer[4], buffer[3]);
    }

    printf("Done !\n");

    fclose(src);
    fclose(dst);

    return 0;
}
