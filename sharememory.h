#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

struct shm_inf
{
    char *shmadd;
    int shmid;
};
struct shm_inf shm_song_info[3];

char *shared_memory(const char *pathname, int proj_id,int memsize,int *shmid);
void del_shm(char *shmadd,int shmid);


#endif