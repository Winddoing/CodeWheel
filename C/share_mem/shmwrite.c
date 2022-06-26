/*#############################################################
 *     File Name	: shmwrite.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月09日 星期五 22时35分29秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/ipc.h>                           
#include <sys/shm.h> 

void print_shm_ds(int shmid)
{
    struct shmid_ds ds = {0};

    shmctl(shmid, IPC_STAT, &ds);

    printf("\nds.shm_nattch\tds.shm_segsz\n%u\t%u\n", (unsigned) ds.shm_nattch, (unsigned) ds.shm_segsz);
}

void* get_shm(key_t key ,size_t size)
{
    int shmid = 0;
    void *attaddr = NULL;

    shmid = shmget(key, size, 0666 | IPC_CREAT);
    if (-1 == shmid) {
        perror("shmget");
        return NULL;
    }

    attaddr = shmat(shmid, NULL, 0);
    if (!attaddr) {
        perror("shmat");
        return NULL;
    }

    printf("shmid[%d] attaddr[%p]", shmid, attaddr);

    print_shm_ds(shmid);

    return attaddr;
}

int release_shm(int shmid)
{
    if (-1 == shmctl(shmid, IPC_RMID, NULL))
    {
        perror("shmctl return -1");
        return -1;
    }

    return 0;
}

struct shm_test {
    int tst1;
    int tst2;
};

int main(int argc, const char *argv[])
{
    void* shm = get_shm(123456, sizeof(struct shm_test));

    printf("===> func: %s, line: %d, shm=%p\n", __func__, __LINE__, shm);

    
    return 0;
}
