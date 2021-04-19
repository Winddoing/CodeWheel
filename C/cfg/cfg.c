/*#############################################################
 *     File Name	: cfg.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月06日 星期二 19时38分50秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


struct gpu_info {
    int gpu_id;
    int max_gpclient_cnt;

    //GPU info
    char gpu_bus_id[16];
    char gpu_card_name[32];
    char gpu_name_desc[128];
    int gpu_numa_id;

    //for egl
    char gpu_render_name[32];
    //for glx
    char display[8];

    //Graphics card and network card mapping
    char rdma_dev[16];
    int rdma_ib_port;
    int rdma_gid_idx;
};

struct sdg_s {
    int gpu_sum;
    char *cfg_file;
    struct gpu_info* gpu;
};


static char *find_first_char(char *in)
{
    while(*in == ' ' || *in == '\t'){
        in++;
    }

    return in;
}

static void clear_end_of_word(char *in)
{
    while(*in != '\0'){
        //if(*in == ' ' || *in == '\t' || *in =='\r' || *in == '\n' || *in == ']'){
        if(*in == '\t' || *in =='\r' || *in == '\n'){
            *in = '\0';
             return;
        }

        in++;
    }
}

static int split_item_value(char *buf, char **item, char **value)
{
    char *it, *vt;

    if(!buf || !item ||!value){
        return -1;
    }

    it = find_first_char(buf);
    if(!it){
        return -1;
    }

    vt = strstr(it, "=");
    if(!vt){
        return -1;
    }

    *vt = 0 ;

    vt++;

    vt = find_first_char(vt);

    if(!vt){
        return -1;
    }

    clear_end_of_word(vt);

    *item  = it;
    *value = vt;

    return 0;
}

static int get_gpu_sum(struct sdg_s *sdg)
{
    FILE *fp = NULL;
    char buf[256] = {0};
    char *item, *value;
    int gpu_sum = 0;

    if(NULL == (fp = fopen(sdg->cfg_file, "r"))){
        printf("cannot open file:%s\n", sdg->cfg_file);
        return -1;
    }

    while(fgets(buf, sizeof(buf), fp)){
        char *pos = find_first_char(buf);
        if (*pos == '#' || *pos == '\r' || *pos == '\n') {
            memset(buf, 0, sizeof(buf));
            continue;
        }

        item = strstr(pos, "gpu_sum");
        if (item) {
            split_item_value(item, &item, &value);
            gpu_sum = atoi(value);
            break;
        }
    }

    fclose(fp);

    sdg->gpu_sum = gpu_sum;

    return 0;
}

static int parse_gpu_cfg_info(struct sdg_s* sdg)
{
    FILE *fp = NULL;
    char buf[256] = {0};
    int new_config = 0;
    char *item, *value;
    int gpu_index = 0;
    struct gpu_info* gpu = sdg->gpu;

    if(NULL == (fp = fopen(sdg->cfg_file, "r"))){
        printf("cannot open file:%s\n", sdg->cfg_file);
        return -1;
    }

    while(fgets(buf, sizeof(buf), fp)){
        char *pos = find_first_char(buf);
        if (*pos == '#' || *pos == '\r' || *pos == '\n') {
            memset(buf, 0, sizeof(buf));
            continue;
        }

        if (*pos=='[') {
            item = strstr(pos, "GPU_ID");
            if (item) {
                new_config = 1;
                split_item_value(item, &item, &value);
                int gpu_id = atoi(value);
                gpu_index = gpu_id - 1;
                gpu[gpu_index].gpu_id = gpu_id;
                memset(buf, 0, sizeof(buf));
            } else {
                item = strstr(pos,"END");
                if (item && new_config) {
                    new_config = 0;
                }
            }
        }

        //printf("===> func: %s, line: %d, pos=%s\n", __func__, __LINE__, pos);
        if (!new_config || 0 > split_item_value(pos ,&item,&value)) {
            memset(buf, 0, 256);
            continue;
        }


        
        //printf("===> func: %s, line: %d, item=%s\n", __func__, __LINE__, item);
        if(!strcmp(item, "BusID")){
            strncpy(gpu[gpu_index].gpu_bus_id, value, sizeof(gpu[gpu_index].gpu_bus_id));
        } else if (!strcmp(item, "GPU_name_desc")) {
            strncpy(gpu[gpu_index].gpu_name_desc, value, sizeof(gpu[gpu_index].gpu_name_desc));
        } else if (!strcmp(item, "NUMA_node")) {
            gpu[gpu_index].gpu_numa_id = atoi(value);
        } else if (!strcmp(item, "RDMA_dev")) {
            strncpy(gpu[gpu_index].rdma_dev, value, sizeof(gpu[gpu_index].rdma_dev));
        } else if (!strcmp(item, "Card_name")) {
            strncpy(gpu[gpu_index].gpu_card_name, value, sizeof(gpu[gpu_index].gpu_card_name));
        } else if (!strcmp(item, "Render_name")) {
            strncpy(gpu[gpu_index].gpu_render_name, value, sizeof(gpu[gpu_index].gpu_render_name));
        }
    }

    fclose(fp);

    return 0;
}

static int custom_gpu_cfg_info(struct sdg_s *sdg)
{
    int i = 0;

    for (i = 0; i < sdg->gpu_sum; i++) {
        sdg->gpu[i].max_gpclient_cnt = 5;
        sprintf(sdg->gpu[i].display, ":%d", sdg->gpu[i].gpu_id);
    }

    return 0;
}

static int alloc_gpu_info(struct sdg_s* sdg)
{

    sdg->gpu = (struct gpu_info*)malloc(sdg->gpu_sum * sizeof(struct gpu_info));
    if (!sdg->gpu) {
        printf("===> func: %s, line: %d, malloc error\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}

static int free_gpu_info(struct sdg_s* sdg)
{
    if (sdg->gpu) {
        free(sdg->gpu);
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    struct sdg_s sdg;

    sdg.cfg_file = "gpu.conf";

    get_gpu_sum(&sdg);
    if (alloc_gpu_info(&sdg)) {
        printf("Alloc gpu info failed!\n");
        return -1;
    }

    parse_gpu_cfg_info(&sdg);
    custom_gpu_cfg_info(&sdg);


    printf("===> func: %s, line: %d, gpu_sum=%d\n", __func__, __LINE__, sdg.gpu_sum);
    int i = 0;
    for (i = 0; i < sdg.gpu_sum; i++) {
        printf("index=%d, GPU_ID=%d, BusID=%s, gpu_name_desc=%s, gpu_card_name=%s, gpu_render_name=%s\n", i, 
                sdg.gpu[i].gpu_id, sdg.gpu[i].gpu_bus_id, sdg.gpu[i].gpu_name_desc, sdg.gpu[i].gpu_card_name, sdg.gpu[i].gpu_render_name);
        printf("\tgpu_numa_id=%d, rdma_dev=%s\n", sdg.gpu[i].gpu_numa_id, sdg.gpu[i].rdma_dev);
        printf("\tmax_gpclient_cnt=%d, display=%s\n", sdg.gpu[i].max_gpclient_cnt, sdg.gpu[i].display);
    }

    printf("===> func: %s, line: %d, cfg_file=%s\n", __func__, __LINE__, sdg.cfg_file);
    free_gpu_info(&sdg);

    return 0;
}
