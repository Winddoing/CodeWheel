/*#############################################################
 *     File Name    : rebuild_edid.c
 *     Author       : winddoing
 *     Created Time : Thu Mar 21 09:29:10 2019
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define adv_msg printf

typedef struct edid_info_ {
    unsigned char BasicInfo[54];
    unsigned char PreferredTiming[18];
    unsigned char DetailLen;
    unsigned char DetailedTiming[3][18];
    unsigned char HasLimit;
    unsigned char MonitorRangeLimit[18];
    unsigned char HasName;
    unsigned char MonitorName[18];
    unsigned char HasExtend;

    unsigned char ExtensionHeader[3];
    unsigned char DisplaySupport;
    unsigned char VideoDataSupport[32];
    unsigned char AudioDataSupport[4];
    unsigned char ExtensionDetailLen;
    unsigned char ExtensionDetailTiming[7][18];

} edid_info_t;


//16:9
unsigned char dtd_1920x1080[18] = {
    0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
    0x58, 0x2C, 0x45, 0x00, 0x76, 0xf2, 0x31, 0x00,
    0x00, 0x1e
};

//16:9
unsigned char dtd_1280x720[18] = {
    0x01, 0x1d, 0x00, 0x72, 0x51, 0xd0, 0x1e, 0x20,
    0x6e, 0x28, 0x55, 0x00, 0x76, 0xf2, 0x31, 0x00,
    0x00, 0x1e
};

//4:3
unsigned char dtd_1024x768[18] = {
    0x64, 0x19, 0x00, 0x40, 0x41, 0x00, 0x26, 0x30,
    0x18, 0x88, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18
};

unsigned char dtd_ignore_extend[128] = {
    0x02, 0x03, 0x1b, 0x61, 0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0c, 0x00,
    0x20, 0x00, 0x80, 0x2d, 0x43, 0x90, 0x84, 0x02, 0xe2, 0x00, 0x0f, 0x8c, 0x0a, 0xd0, 0x8a, 0x20,
    0xe0, 0x2d, 0x10, 0x10, 0x3e, 0x96, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29
};

//16:10
unsigned char dtd_1280x800[18] = {
    0x9e, 0x20, 0x00, 0x90, 0x51, 0x20, 0x1f, 0x30,
    0x48, 0x80, 0x36, 0x00, 0x40, 0x84, 0x63, 0x00,
    0x00, 0x1c
};

unsigned char extend_start_data[4] = { 
    0x02, 0x03, 0x00, 0xf1
};

unsigned char extend_audio_data[4] = { 
    0x23, 0x09, 0x07, 0x01
};

unsigned char extend_speaker_data[4] = { 
    0x83, 0x01, 0x00, 0x00
};

unsigned char extend_vendor_data[17] = {
    0x70, 0x03, 0x0c, 0x00, 0x10, 0x00, 0xb8, 0x2d, 0x20,
    0xa0, 0x06, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00
};

unsigned char range_limit[18] = {
    0x00, 0x00, 0x00, 0xfd, 0x00, 0x17, 0x55, 0x0f, 0x5c,
    0x11, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};


static unsigned char calculate_checksum(unsigned char *block_beg)
{
    unsigned char sum = 0;
    unsigned char i;

    for(i = 0; i < 0x7F; i++)
        sum += block_beg[i];

    return ((~sum) + 1); // Check this?!
}

static int is_ingore_extend(unsigned char *data)
{
    int i;

    for(i = 0; i < 128; i++) {
        if(data[i] != dtd_ignore_extend[i])
            return 1;
    }

    return 0;
}

static unsigned char edid_constrcut_video_data(unsigned char *dstedid, unsigned char *buff)
{
    unsigned char AddrTag, D, Addr, Data;
    unsigned char len;
    int i;
    int temp_addr;

    D = buff[2];   //Byte number offset d where Detailed Timing data begins
    Addr = 4;

    AddrTag = Addr;

    do {
        Data = buff[AddrTag];
        adv_msg("data 0x%x\n", Data);

        if((Data & 0xe0) == 0x40) {
            len = Data & 0x1f;

            for(i = 0; i < len + 1; i++)
                dstedid[i] = buff[Addr + i];

            return  len + 1;
        }

        Addr += (Data & 0x1F) ;     // next Tag Address
        AddrTag = ++Addr;
        /*temp_addr =   Addr + ( Data & 0x1F ) ;
          if(temp_addr >= D)    //force to break;
          break;*/
    } while(Addr < D);

    return 0;
}

static int isMonitorDes(unsigned char * data)
{
    int ret = 0;
    int i;

    for(i = 0; i < 3; i++) {
        if(data[i])
            return 0;
    }

    return 1;
}

static void dump_data(char *s, unsigned char *buf, int len)
{
    int i = 0;
    printf("\n===> func: %s: [%s]\n", __func__, s);

    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }

    printf("\n");
}

static int edid_parse(edid_info_t *edid_info, unsigned char *data, int length)
{
    unsigned int i, j;
    unsigned char *ptr;
    int idx[4];

    for(i = 0; i < 54; i++) {
        edid_info->BasicInfo[i] = data[i];
    }

    for(i = 38; i < 53; i += 2) {
        if(data[i] >= 0xD1) {
            edid_info->BasicInfo[i] = 0xD1;
            edid_info->BasicInfo[i + 1] = 0xC0;
        }
    }

    idx[0] = 0x36; //54
    idx[1] = 0x48; //72
    idx[2] = 0x5A; //90
    idx[3] = 0x6c; //108

    for(i = 0; i < 4; i++) {
        ptr = &data[idx[i]];

        if(isMonitorDes(ptr)) {
            if(ptr[3] == 0xfc) {
                edid_info->HasName = 1;

                for(j = 0; j < 18; j++)
                    edid_info->MonitorName[j] = ptr[j];

                //dump_data("MonitorName", edid_info->MonitorName, 18);

            } else if(ptr[3] == 0xfd) {
                edid_info->HasLimit = 1;
            }

        } else {
            int width = ptr[2] + (ptr[4] >> 4) * 256;
            int height = ptr[5] + (ptr[7] >> 4) * 256;
            int interlaced = ptr[17] & 0x80;
            unsigned char timing[18] = {0,};
            adv_msg("width %d height %d interlaced %d\n",
                    width, height, interlaced);

            if(width * 9 == height * 16 * ((interlaced ? 1 : 0) + 1)) {
                adv_msg("16:9\n");

                if(width >= 1920)
                    for(j = 0; j < 18; j++)
                        timing[j] = dtd_1920x1080[j];

            } else if(width * 3 == height * 4 * ((interlaced ? 1 : 0) + 1)) {
                adv_msg("4:3\n");

                if(width >= 1024)
                    for(j = 0; j < 18; j++)
                        timing[j] = dtd_1024x768[j];
            }

            /*else if(width*10 == height*16*((interlaced?1:0) + 1))
              {
              adv_msg("16:10\n");
              if(width >= 1280)
              for (j=0; j<18; j++)
              timing[j] =dtd_1280x800[j];
              }*/
            if(i == 0) {
                if(timing[0] != 0)
                    for(j = 0; j < 18; j++)
                        edid_info->PreferredTiming[j] = timing[j];

                else
                    for(j = 0; j < 18; j++)
                        edid_info->PreferredTiming[j] = data[idx[i] + j];

            } else {
                unsigned char index = edid_info->DetailLen;
                adv_msg("idx[i] + j %d\n", idx[i]);

                if(timing[0] != 0)
                    for(j = 0; j < 18; j++)
                        edid_info->DetailedTiming[index][j] = timing[j];

                else
                    for(j = 0; j < 18; j++)
                        edid_info->DetailedTiming[index][j] = data[idx[i] + j];

                edid_info->DetailLen++;
            }
        }
    }

    if(is_ingore_extend(data + 128))
        edid_info->HasExtend = data[126];

    else
        edid_info->HasExtend = 0;

    if(edid_info->HasExtend == 0)
        return 0;

    ptr = &data[128];

    for(i = 0; i < 3; i++)
        edid_info->ExtensionHeader[i] = extend_start_data[i];

    edid_info->DisplaySupport = ptr[3];

    edid_constrcut_video_data(edid_info->VideoDataSupport, ptr);

    for(i = 0; i < 4; i++)
        edid_info->AudioDataSupport[i] = extend_audio_data[i];

    unsigned char start_offset = ptr[2]; //DTD Starting Offset
    adv_msg("DTD Starting Offset %d\n", start_offset);

    for(i = start_offset; i + 18 < 127; i += 18) {

        unsigned char index = edid_info->ExtensionDetailLen;
        ptr = &data[128 + i];
        adv_msg("%s, index: %d, id=%d, ptr=%x\n", __func__, index,128+i, ptr[0]);

        if((ptr[0] == 0) && (ptr[1] == 0) && (ptr[2] == 0)) {
            for(j = 0; j < 18; j++)
                edid_info->ExtensionDetailTiming[index][j] = data[128 + i + j];

            edid_info->ExtensionDetailLen++;
            continue;
        }

        adv_msg("%s, ExtensionDetailLen=%d\n", __func__, edid_info->ExtensionDetailLen);
        int width = ptr[2] + (ptr[4] >> 4) * 256;
        int height = ptr[5] + (ptr[7] >> 4) * 256;
        int interlaced = ptr[17] & 0x80;
        unsigned char timing[18] = {0,};
        adv_msg("\textend width %d height %d interlaced %d\n", width, height, interlaced);

        if(width == 0) {
            for(j = 0; j < 18; j++)
                edid_info->ExtensionDetailTiming[index][j] = data[128 + i + j];

            edid_info->ExtensionDetailLen++;
            continue;
        }

        adv_msg("Wx9=%d, Hx16=%d\n", width * 9, height * 16);
        adv_msg("Wx9=%d, Hx16=%d\n", width * 9, height * 16 * ((interlaced ? 1 : 0) + 1));
        //adv_msg("Wx3=%d, Hx04=%d\n", width * 3, height * 4 * ((interlaced ? 1 : 0) + 1));
        if(width * 9 == height * 16 * ((interlaced ? 1 : 0) + 1)) {
            adv_msg("16:9\n");

            if(width >= 1920)
                for(j = 0; j < 18; j++)
                    timing[j] = dtd_1920x1080[j];

        } else if(width * 3 == height * 4 * ((interlaced ? 1 : 0) + 1)) {
            adv_msg("4:3\n");

            if(width >= 1024)
                for(j = 0; j < 18; j++)
                    timing[j] = dtd_1024x768[j];
        }

        /*else if(width*10 == height*16*((interlaced?1:0) + 1))
          {
          adv_msg("16:10\n");
          if(width >= 1280)
          for (j=0; j<18; j++)
          timing[j] =dtd_1280x800[j];
          }*/
        if(timing[0] != 0)
            for(j = 0; j < 18; j++)
                edid_info->ExtensionDetailTiming[index][j] = timing[j];

        else
            for(j = 0; j < 18; j++)
                edid_info->ExtensionDetailTiming[index][j] = data[128 + i + j];

        edid_info->ExtensionDetailLen++;
    }

    return 0;
}

static int edid_build(edid_info_t *edid_info, unsigned char *data)
{
    unsigned char i, j;
    unsigned int index = 0;

    for(i = 0; i < 54; i++)
        data[i] = edid_info->BasicInfo[i];

    index = 54;

    for(i = 0; i < 18; i++)
        data[index++] = edid_info->PreferredTiming[i];

    printf("===> func: %s, line: %d, index=%d, DetailLen=%d\n", __func__, __LINE__, index, edid_info->DetailLen);
    if(edid_info->DetailLen != 0) {
        for(j = 0; j < edid_info->DetailLen; j++)
            for(i = 0; i < 18; i++)
                data[index++] = edid_info->DetailedTiming[j][i];
    }

    if(edid_info->HasName) {
        for(i = 0; i < 18; i++)
            data[index++] = edid_info->MonitorName[i];
    }

    if(edid_info->HasLimit) {
        for(i = 0; i < 18; i++)
            data[index++] = range_limit[i];
    }

    data[0x7e] = edid_info->HasExtend;

    if(0/*is_vga_connect()*/) {
        data[20] = 0x0;
        data[126] = 0; //Extension Block(s)
        //gDriverContext.mInput = VGA_TYPE;

    } else {
        data[20] = 0x80;
        //gDriverContext.mInput = HDMI_TYPE;
    }

    data[0x7f] = calculate_checksum(data);

    if(data[0x7e] == 0)
        return 0;

    unsigned char *ptr = &data[0x80];
    unsigned int len = 0;

    dump_data("ExtensionHeader", edid_info->ExtensionHeader, 3);
    for(i = 0; i < sizeof(edid_info->ExtensionHeader); i++)
        ptr[len++] = edid_info->ExtensionHeader[i];

    ptr[len++] = edid_info->DisplaySupport; //len = 3

    unsigned char data_block_len = 0;
    data_block_len = edid_info->AudioDataSupport[0] & 0x1f;
    adv_msg("Audio Data block len is %d\n", data_block_len);
    if (data_block_len != 0) {
        for(i = 0; i < sizeof(edid_info->AudioDataSupport); i++)
            ptr[len++] = edid_info->AudioDataSupport[i];
    }

    data_block_len = edid_info->VideoDataSupport[0] & 0x1f;
    adv_msg("Video Data Block len is %d\n", data_block_len);
    if (data_block_len != 0) {
        for(i = 0; i < (data_block_len + 1); i++)
            ptr[len++] = edid_info->VideoDataSupport[i];
    }

    for(i = 0; i < sizeof(extend_speaker_data); i++)
        ptr[len++] = extend_speaker_data[i];

    for(i = 0; i < sizeof(extend_vendor_data); i++)
        ptr[len++] = extend_vendor_data[i];

    ptr[2] = len;  //DTD Starting Offset
    adv_msg("edid_info->ExtensionDetailLen %d len %d,\n", edid_info->ExtensionDetailLen, len);

    for(i = 0; i < edid_info->ExtensionDetailLen; i++) {
        adv_msg("len %d\n", len);

        if(len + 18 > 127)
            break;

        for(j = 0; j < 18; j++)
            ptr[len++] = edid_info->ExtensionDetailTiming[i][j];
    }

    ptr[127] = calculate_checksum(ptr);
    return 0;
}

static void edid_prepare(unsigned char *new_edid)
{
    edid_info_t edid_info;
    unsigned char edid[256];
    memset(&edid_info, 0, sizeof(edid_info));
    edid_parse(&edid_info, new_edid, sizeof(new_edid));
    edid_build(&edid_info, new_edid);
}


int main(int argc, const char *argv[])
{
    FILE *in_fp, *out_fp;
    unsigned char buf[1024];
    int size = 0;

    if(argc < 3) {
        printf("usage: %s %s %s\n", argv[0], "infile", "outfile");
        return -1;
    }

    in_fp = fopen(argv[1], "r");

    if(in_fp == NULL) {
        printf("%s, %s", argv[1], "not exit\n");
        return -1;
    }

    out_fp = fopen(argv[2], "wb");

    if(out_fp == NULL) {
        printf("%s, %s", argv[2], "not exit\n");
        fclose(in_fp);
        return -1;
    }


    size = fread(buf, 1, sizeof(buf), in_fp);

    if(size != 256) {
        printf("Read File size error: %s, size=%d\n", strerror(errno), size);
        fclose(in_fp);
        fclose(out_fp);
        return -1;
    }

    printf("File size %d\n", size);

    edid_prepare(buf);

    size = fwrite(buf, 1, size, out_fp);

    if(size != 256) {
        printf("write File size error: %s, size=%d\n", strerror(errno), size);
    }

    fclose(in_fp);
    fclose(out_fp);

    return 0;
}
