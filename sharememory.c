#include "sharememory.h"

char *shared_memory(const char *pathname, int proj_id,int memsize,int *shmid)
{
	
	key_t key;
	char *shmadd;
	key=ftok(pathname,proj_id);
	if(key==1)
	{
		perror("ftok");
	}
	
	*shmid =shmget(key,memsize,IPC_CREAT|0666);
	if(*shmid<0)
	{
		perror("shmget");
		return NULL;
	}
	shmadd = shmat(*shmid,NULL,0);
	if(shmadd<0)
	{
		perror("shmat");
		return NULL;
	}
	return shmadd;
}

void del_shm(char *shmadd,int shmid)
{
    int ret=shmdt(shmadd);
	if(ret<0)
	{
		perror("shmdt");
		exit(1);
	}
	else
	{
		printf("deleted shared-memory\n");
	}
	shmctl(shmid,IPC_RMID,NULL);
}
