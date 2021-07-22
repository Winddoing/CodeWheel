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

/**
 * @brief  set_numa_node 将当前进程绑定到第node个numa节点上
 *
 * @param node  numa节点号，比如两个CPU有两个numa节点即0或者1
 *
 * @returns   
 */
static int set_numa_node(int node)
{
    struct bitmask *bit;
    int numa_max_num = 0;
    int numa_node_num = 0;

    numa_max_num = numa_max_node();
    numa_node_num = numa_max_num + 1;
    printf("numa max num=%d, node num=%d\n", numa_max_num, numa_node_num);

    bit = numa_bitmask_alloc(numa_node_num);
    numa_bitmask_clearall(bit);
    numa_bitmask_setbit(bit, node);
    numa_bind(bit);
    numa_bitmask_free(bit);

    return 0;
}


int main(int argc, const char *argv[])
{
    if(numa_available() < 0){
        printf("your system does not support numa api\n");
        return -1;
    }

    //set_numa_node(0);
    set_numa_node(0);

    printf("pid = %d\n", getpid());
    while(1);


    return 0;
}
