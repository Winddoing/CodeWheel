#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct YUVBuffer{
    uint8_t *buffer[2];
    int index;
};


void usage() {
   fprintf(stderr, "usage: yuvcmp <INPUT FILE>  <OUTPUT FILE> <WIDTH> <HEIGHT> <FORMAT>\n");
   fprintf(stderr, "       current only support YUV444/YUV420/yuv422 planar compare\n");
   fprintf(stderr, "       FORMAT 0:YUV444 1:YUV420 2:yuv422\n");
   exit(1);
}


void yuvcompare(uint8_t *buf0, uint8_t *buf1, uint8_t *out, int size){
    int a;
    int b0, b1;
    while(size-- > 0){
        //*out++ = *buf1++ - *buf0++;
        b0 = *buf0++;
        b1 = *buf1++;
        
        a = abs(b1 - b0);
        if (a > 3){
            *out = 0;
            //printf("%d, b0:%d b1:%d  ", a, b0, b1);
        }else 
            *out = 128;
        out++;
    }
}

int main(int argc, char *argv[]){
    char *from, *to;
    int width, height;
    size_t size;
    uint8_t *outFrame;
    int type;
    FILE *fOut, *fIn;
    YUVBuffer yuvBuffer;
    int first;
    
    if (argc < 6)
        usage();
    
    from = argv[1];
    to = argv[2];
    fOut = fopen(to, "wb");
    fIn = fopen(from, "rb");
    
    width = atoi(argv[3]);
    height = atoi(argv[4]);
    type = atoi(argv[5]);
    size = width * height * 3;
    if (type == 1){
        size /= 2;
    }else if (type == 2){
        size =  width * height * 2;
    }

    outFrame = (uint8_t *)malloc(size);
    yuvBuffer.index = 0;
    yuvBuffer.buffer[0] = (uint8_t *)malloc(size);
    yuvBuffer.buffer[1] = (uint8_t *)malloc(size);   
    
    fread(yuvBuffer.buffer[0], size, 1, fIn); //read the first Frame
    yuvBuffer.index ^= 1;
    while (fread(yuvBuffer.buffer[yuvBuffer.index], size, 1, fIn)){
        first = yuvBuffer.index ^ 1;
        yuvcompare(yuvBuffer.buffer[first], yuvBuffer.buffer[yuvBuffer.index], outFrame, size);
        yuvBuffer.index ^= 1;
        fwrite(outFrame, size, 1, fOut);
    }
    fclose(fIn);
    fclose(fOut);

}