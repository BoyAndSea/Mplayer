#ifndef __FILE_OPS_H__
#define __FILE_OPS_H__

#include <stdio.h>
#include <string.h>
#include "list.h"
#include <termios.h>
#include <unistd.h>



typedef struct lrc{
	struct list_head node;
	int time;  
	char lrc_buf[300];
}LRC;




struct struct_lrc
{
    char *lrc_sentence[200];
    char song_path[300];
    int num_of_sentence;
    int lrc_time[200];
    struct list_head song_lrc_head;
    struct list_head song_list_head;
    struct list_head skin_list_head;
    struct list_head picture_list_head;
    int button_start_flag;
    int song_code;
    int skin_code;
    int picture_code;
    int song_num;
    int skin_num;
    int picture_num;
    int play_order_flag;
    int sorclled_window_hide_flag;
};
struct struct_lrc lrc_info_structure;


void link_print(struct list_head *head);
struct list_head *link_creat(struct list_head *head,LRC *pnew);
LRC *link_s(struct list_head *head,int num,unsigned int offset);
int link_code(struct list_head *head,char *buf);
int msg_deal(char *msg_src,char *msg_done[],char *str);
char *file_open(char *file_name);
void link_to_array(struct list_head *head,int *array);
int *array_s(int tar,int*array,int array_len);
char * LoadInfo(char *fold_name);
int list_save_to_link(struct list_head *song_link_head,char *fold_name);
void lrc_save_to_link_arry(char *q[],int *len,struct list_head *head,char* argv,int *lrc_time);



void printf_lrc_info(void);
#endif