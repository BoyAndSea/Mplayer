#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <gtk/gtk.h>
#include <string.h>
#include "list.h"
#include <sys/wait.h>
#include <semaphore.h>

pid_t pid_lrc,pid_play;


void print_label(GtkWidget *label,char *str);


void str_tran(char str[]);
void write_fifo_cmd(char *cmd);
void write_to_pid_lrc(char *cmd);




#endif