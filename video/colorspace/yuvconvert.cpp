#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void usage() {
   fprintf(stderr, "usage: yuvconvert <INPUT FILE>  <OUTPUT FILE> <WIDTH> <HEIGHT> <Count> <Func>\n");
   fprintf(stderr,"Func: 0--yuv444 sample down to NV12\n");
   fprintf(stderr,"      1--yuv444 average to NV12\n");
   fprintf(stderr,"      2--yuv444 sample down to yuv422\n");
   fprintf(stderr,"      3--yuv444 average to yuv422\n");
   exit(1);
}

void yuvconvert(uint8_t *dst, uint8_t *src, int width, int height){
    uint8_t *y, *u, *v;
    y = src;
    u = src + width * height;
    v = src + width * height *2;
        
    //Copy Y
    memcpy(dst, y, width * height);
    
    //Convert to NV12
    dst += width * height;
    for (int h = 0; h < (height + 1)/2; h++){
        for (int w = 0; w < (width + 1)/2; w++){
            dst[2*w]        = u[2*w]; 
            dst[2*w + 1]    = v[2*w];
        }
        u += 2 * width;
        v += 2 * width;
        dst += width;
    }
}


void yuvaverageconvert(uint8_t *dst, uint8_t *src, int width, int height){
    uint8_t *y, *u, *v;
    y = src;
    u = src + width * height;
    v = src + width * height *2;
        
    //Copy Y
    memcpy(dst, y, width * height);
    
    //Convert to NV12
    dst += width * height;
    for (int h = 0; h < (height + 1)/2; h++){
        for (int w = 0; w < (width + 1)/2; w++){
            dst[2*w]        = ((uint32_t)u[2*w] + (uint32_t)u[2*w + 1] + (uint32_t)u[2*w + width] + (uint32_t)u[2*w + 1 + width]) >> 2; 
            dst[2*w + 1]    = ((uint32_t)v[2*w] + (uint32_t)v[2*w + 1] + (uint32_t)v[2*w + width] + (uint32_t)v[2*w + 1 + width]) >> 2;
        }
        u += 2 * width;
        v += 2 * width;
        dst += width;
    }
}

void yuvto422(uint8_t *dst, uint8_t *src, int width, int height){
    uint8_t *y, *u, *v;
    uint8_t *dst_u, *dst_v;
    y = src;
    u = src + width * height;
    v = src + width * height *2;
    dst_u = dst + width * height;
    dst_v = dst_u + width * height / 2;
        
    //Copy Y
    memcpy(dst, y, width * height);

    for (int h = 0; h < (height + 1) / 2; h++){
        memcpy(dst_u + h * width, u + (2*h + 1) * width, width);
        memcpy(dst_v + h * width, v + (2*h + 1) * width, width);
    }   
    return;
}

void yuvaverageto422(uint8_t *dst, uint8_t *src, int width, int height){
    uint8_t *y, *u, *v;
    uint8_t *dst_u, *dst_v;
    y = src;
    u = src + width * height;
    v = src + width * height *2;
    dst_u = dst + width * height;
    dst_v = dst_u + width * height / 2;
        
    //Copy Y
    memcpy(dst, y, width * height);
    for (int h = 0; h < (height + 1) / 2; h++){
        for (int w = 0; w < width; w++){
            dst_u[w] = (u[w] + u[w + width]) / 2;
            dst_v[w] = (v[w] + v[w + width]) / 2;
        }
        dst_u += width;
        dst_v += width;
        u += width * 2;
        v += width * 2;
    }   
    return;
    
}


int main(int argc, char *argv[]){
    char *from, *to;
    int width, height;
    size_t size, csize;
    uint8_t *frame;
    uint8_t *cframe;
    FILE *fOut, *fIn;
	int count;
	int func;
    if (argc < 5)
        usage();
    
    from = argv[1];
    to = argv[2];
    width = atoi(argv[3]);
    height = atoi(argv[4]);
	count = atoi(argv[5]);
    func = atoi(argv[6]);

    size = width * height * 3;
    switch (func){
        case 0:
        case 1:
            csize = size / 2;
        break;
        case 2:
        case 3:
            csize = size * 2 / 3;
        break;
    }
    
    
    frame = (uint8_t *)malloc(size);
    cframe = (uint8_t *)malloc(csize);
    fOut = fopen(to, "wb");
    fIn = fopen(from, "rb");
    while (fread(frame, size, 1, fIn) && count-- >0){
		switch (func){
		case 0:
        	yuvconvert(cframe, frame, width, height);
			break;
		case 1:
        	yuvaverageconvert(cframe, frame, width, height);
			break;
        case 2:
            yuvto422(cframe, frame, width, height);
            break;
        case 3:
            yuvaverageto422(cframe, frame, width, height);
            break;
		}
        fwrite(cframe, csize, 1, fOut);
    }    
    

    
    fclose(fIn);
    fclose(fOut);

}
