/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2020年12月09日 星期三 11时24分23秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <numa.h>

int main(int argc, const char *argv[])
{

    if(numa_available() < 0){
        printf("your system does not support numa api\n");
        return -1;
    }


    numa_run_on_node(0);
    numa_set_preferred(0);

    //获取系统可用的CPU数量
    int numcpus=numa_num_task_cpus();
    printf("numa_num_task_cpus numcpus=%d\n", numcpus);

    //获取最大节点的numa id
    int nodenum=numa_max_node();
    printf("numa_max_node nodenum=%d\n", nodenum);

    //numa_set_localalloc();
    
    //创建cpu的bitmask
    struct bitmask* bm = numa_bitmask_alloc(numcpus);
    void *m_buf = NULL;

    for(int numa_id = 0; numa_id <= nodenum; numa_id++) {
        //得到每个节点的cpu的bitmask， 每一位代表一个cpu
        numa_node_to_cpus(numa_id, bm);

        //numa_node_size: 获取节点占用内存的大小
        printf("numa_node=%d, bm-size=%ld, bm-maskp=%lx  %ld\n", numa_id, bm->size, *bm->maskp, numa_node_size(numa_id, 0));

        //在指定节点上分配内存，主要要用numa_free()来释放内存)
        m_buf = numa_alloc_onnode(100, numa_id);
    }
    numa_free(m_buf,100);
    return 0;
}
