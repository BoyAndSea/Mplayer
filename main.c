
#include <stdio.h>
#include <unistd.h>
#include "window.h"
#include <string.h>



sem_t* share_sem1;


int main(int argc, char  *argv[])
{
    //无名管道配置
    int fd_pipe[2];
    if( pipe(fd_pipe) < 0)
        perror("pipe");

    pid_lrc=fork();
    if(pid_lrc==0)
    {

        int fd[2];
        int ret=mkfifo("father_to_pid_lrc_cmd",0777); 
        if(ret != 0)
        {
            perror("mkfifo");
        }
        fd[0]=open("father_to_pid_lrc_cmd",O_RDONLY|O_NONBLOCK);
        if(fd[0]<0)
        {
            perror("open father_to_pid_lrc_cmd");
        }

        struct shm_inf shm_song_info[3];
       
	    shm_song_info[0].shmadd=shared_memory(".", 0,100,&shm_song_info[0].shmid);
        shm_song_info[1].shmadd=shared_memory(".", 1,100,&shm_song_info[1].shmid);
        shm_song_info[2].shmadd=shared_memory(".", 2,200,&shm_song_info[2].shmid);
        int flag=0;
        int i=0;
        while(1)
        { 
            char recv_father[50]="";
            int len = read(fd[0],recv_father,sizeof(recv_father));
            if(len!=0)
            {
                write_fifo_cmd(recv_father);
            }
            char *str[2];
            str[0]=strtok(recv_father," \n");
            str[1]=strtok(NULL," \n");
            if(str[0]!=NULL)
            {
                if(strcmp(str[0],"pause")==0)
                    flag=!flag;
                if((strcmp(str[0],"loop")==0)||(strcmp(str[0],"pt_step")==0)||(strcmp(str[0],"volume")==0)||(strcmp(str[0],"seek")==0))
                    flag=1;
            }
            
            //printf("%d %s\r" ,flag,str[0]);
            //fflush(NULL);
            
            if(flag!=0)
            {
                char recv_time_pos[200]="";
                write_fifo_cmd("get_time_pos\n");
                usleep(100);
                len = read(fd_pipe[0],recv_time_pos,sizeof(recv_time_pos));
                if(len!=0)
                {
                    char *str[2];
                    str[0]=strtok(recv_time_pos,"=");
                    str[1]=strtok(NULL," \n");
                    if(strcmp(str[0],"ANS_TIME_POSITION")==0)
                    {
                        bzero(shm_song_info[0].shmadd,100);
                        strcpy(shm_song_info[0].shmadd,str[1]);
                        //printf("%s\n",shm_song_info[0].shmadd);
                    }
                    else
                    {
                        //printf("recv_time_pos   %s\n",recv_time_pos);
                    }
                    
                }
                else
                {
                    //printf("len = %d\n",len);
                }
                
                char recv_time_length[100]="";
                write_fifo_cmd("get_time_length\n");
                usleep(100);
                len = read(fd_pipe[0],recv_time_length,sizeof(recv_time_length));
                if(len!=0)
                {
                    char *str[2];
                    str[0]=strtok(recv_time_length,"=");
                    str[1]=strtok(NULL," \n");
                    if(strcmp(str[0],"ANS_LENGTH")==0)
                    {
                        bzero(shm_song_info[1].shmadd,100);
                        strcpy(shm_song_info[1].shmadd,str[1]);
                        //printf("%s\n",shm_song_info[1].shmadd);
                        
                    }
                    else
                    {
                        //printf("recv_time_length %s\n",recv_time_length);
                    }
                }
                else
                {
                    //printf("len = %d",len);
                }
                
                char recv_file_name[300]="";
                write_fifo_cmd("get_file_name\n");
                usleep(100);
                len = read(fd_pipe[0],recv_file_name,sizeof(recv_file_name));
                if(len!=0)
                {
                    char *str[3];
                    str[0]=strtok(recv_file_name,"='");
                    str[1]=strtok(NULL,"='");
                    if(strcmp(str[0],"ANS_FILENAME")==0)
                    {
                        bzero(shm_song_info[2].shmadd,200);
                        strcpy(shm_song_info[2].shmadd,str[1]);
                        //printf("%s\n",shm_song_info[2].shmadd);
                        
                    }
                    else
                    {
                        //printf("recv_file_name %s\n",recv_file_name);
                    }
                }
                else
                {
                    //printf("len = %d",len);
                }
            }  
        }
        exit(0);
    }

    pid_play=fork();
    if(pid_play==0)
    { 
       
        int fd;
        int ret;
        ret=mkfifo("fifo_cmd",0777);
        fd=open("fifo_cmd",O_RDONLY);
        if(fd<0)
        {
            perror("open fifo");
        }
        
        dup2(fd_pipe[1],1);
        //execlp("mplayer","mplayer","-ac", "mad","-slave", "-quiet","-idle","-input", "file=./fifo_cmd",NULL, NULL);
        execlp("mplayer","mplayer","-ac","mad","-idle","-slave","-quiet","-playlist","Music_Mp3_list","-loop","0","-input","file=./fifo_cmd",NULL);
        perror("execlp");
        exit(-1);
    }
    else
    {
        
        LRC *pnew_skin,*pnew_picture,*pnew_song_list;
        char *p;
        int i; 
        lrc_info_structure.skin_code=1;
        lrc_info_structure.picture_code=1;
        lrc_info_structure.num_of_sentence=0;
        lrc_info_structure.song_code=1;
        lrc_info_structure.play_order_flag=-1;
        lrc_info_structure.sorclled_window_hide_flag=0;
        
        
    
        INIT_LIST_HEAD(&lrc_info_structure.skin_list_head);
        INIT_LIST_HEAD(&lrc_info_structure.picture_list_head);
        INIT_LIST_HEAD(&lrc_info_structure.song_lrc_head);
        INIT_LIST_HEAD(&lrc_info_structure.song_list_head);

        lrc_info_structure.skin_num=list_save_to_link(&lrc_info_structure.skin_list_head,"skin");   //将skin目录下文件名存储到链表 同时返回文件数量
        lrc_info_structure.picture_num=list_save_to_link(&lrc_info_structure.picture_list_head,"picture");  //将picture目录下文件名存储到链表 同时返回文件数量
        lrc_info_structure.song_num=list_save_to_link(&lrc_info_structure.song_list_head,"Music");    //将歌曲文件夹目录存储到链表
        pnew_skin=link_s(&lrc_info_structure.skin_list_head,lrc_info_structure.skin_code,0);  //获得存储第一张皮肤图片路径的节点地址
        pnew_picture=link_s(&lrc_info_structure.picture_list_head,lrc_info_structure.picture_code,0); //获得存储第一张图片路径的节点地址
        pnew_song_list=link_s(&lrc_info_structure.song_list_head,1,0); //获得存储第一首歌曲路径的节点地址
        
        strcpy(lrc_info_structure.song_path,pnew_song_list->lrc_buf);

        p=lrc_info_structure.song_path;
        while(*p)
        {
            p++;
        }
        p--;*p='c';
        p--;*p='r';
        p--;*p='l';

        lrc_save_to_link_arry(lrc_info_structure.lrc_sentence,&lrc_info_structure.num_of_sentence,&lrc_info_structure.song_lrc_head,
        lrc_info_structure.song_path,lrc_info_structure.lrc_time);  //将歌词文件存储到链表
        lrc_info_structure.button_start_flag=0;


        
    
        
        shm_song_info[0].shmadd=shared_memory(".", 0,100,&shm_song_info[0].shmid);
	    shm_song_info[1].shmadd=shared_memory(".", 1,100,&shm_song_info[1].shmid);
        shm_song_info[2].shmadd=shared_memory(".", 2,200,&shm_song_info[2].shmid);
        





        WINDOW *wp=Create_Main_Wind(&wind,&argc, &argv,"奇怪的MP3播放器 版本3.0",pnew_skin->lrc_buf);   //创建一个主窗口
        wp->table=gtk_table_new(100,160,TRUE);
        wp->button_next=create_button_from_file("./ico/right-circle.png",50,50);
        wp->button_prev=create_button_from_file("./ico/left-circle.png",50,50);
        wp->button_start=create_button_from_file("./ico/play-start.png",50,50);
        wp->button_exit=create_button_from_file("./ico/关闭.png",50,50);
        wp->change_skin=create_button_from_file ("./ico/skin.png",50,50);
        wp->button_volup=create_button_from_file("./ico/Image-audio-volume-medium-panel.png",50,50);
        wp->button_song_ctrl=create_button_from_file("./ico/循环播放_32.png",50,50);
        wp->button_create_chlid_wind=gtk_button_new_with_label("歌曲列表");
        wp->eventbox = gtk_event_box_new(); // 事件盒子的创建
        wp->eventbox_vol=gtk_event_box_new();
        gtk_widget_set_events(wp->eventbox, GDK_BUTTON_PRESS_MASK); // 捕获鼠标点击事件
        gtk_widget_set_events(wp->eventbox_vol, GDK_BUTTON_PRESS_MASK); // 捕获鼠标点击事件
        wp->progress=gtk_progress_bar_new();
        wp->progress_vol=gtk_progress_bar_new();
     
        wp->scrolled_window=gtk_scrolled_window_new(NULL, NULL);
        for(i=0;i<7;i++)
        {
            wp->label_lrc[i]=gtk_label_new("");
            set_label_font_size(wp->label_lrc[i], 15);
            if(i==3)
                gtk_label_set_markup(GTK_LABEL(wp->label_lrc[i]), "<span foreground=\"#00e080\"> 点击播放  </span>");
            else
            {
                gtk_label_set_markup(GTK_LABEL(wp->label_lrc[i]), "<span foreground=\"#8080ff\">  </span>");
            }

        }

       
        wp->label_time=gtk_label_new("");
        wp->label_percent=gtk_label_new("");
        wp->label_current_song_name=gtk_label_new("");
        set_label_font_size(wp->label_current_song_name, 20);
        gtk_label_set_markup(GTK_LABEL(wp->label_current_song_name),"<span foreground=\"#000000\">当前播放歌曲名</span>");
        gtk_label_set_markup(GTK_LABEL(wp->label_time), "<span foreground=\"#000000\"> 0:00  </span>");
        gtk_label_set_markup(GTK_LABEL(wp->label_percent), "<span foreground=\"#000000\"> 0:00  </span>");
       
    
        
        
        
        gtk_container_add(GTK_CONTAINER(wp->main_wind),wp->table);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_start,10,20,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_next,20,30,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_prev,0,10,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->change_skin,10,20,0,10);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_volup,30,40,0,10);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->eventbox_vol,40,60,5,6);
        gtk_container_add( GTK_CONTAINER(wp->eventbox_vol), wp->progress_vol );
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_exit,0,10,0,10);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_song_ctrl,20,30,0,10);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->button_create_chlid_wind,0,20,85,90);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->label_time,150,160,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->label_percent,30,40,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->eventbox,40,150,95,96);
        gtk_container_add( GTK_CONTAINER(wp->eventbox), wp->progress );
        //gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->progress,40,150,90,100);
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->scrolled_window, 0, 60, 10, 85);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wp->scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        
        gchar *titles[2] = { "       ", "音乐标题" };
        
        wp->clist = gtk_clist_new_with_titles(2, titles);// 创建 GtkCList 构件。本例中，我们使用了两列
  



        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(wp->scrolled_window), wp->clist); // 将 GtkCList 构件添加到滚动窗口构件中
    
        gtk_clist_set_column_justification(GTK_CLIST(wp->clist), 1, GTK_JUSTIFY_LEFT);
        gtk_clist_set_column_width(GTK_CLIST(wp->clist), 100, 20);
        
        
        for(i=0;i<lrc_info_structure.song_num;i++)
        {
            LRC*p=link_s(&lrc_info_structure.song_list_head,i+1,0);
            if(p!=NULL)
            {
                
                
                char str1[10]="";
                char str2[100]="";
                sprintf(str1," %d ",p->time);
                strcpy(str2,p->lrc_buf);
                char *text[2];
                text[0]=str1;
                text[1]=str2;
                gtk_clist_append((GtkCList *)wp->clist, text);

            }
        }
        for(i=0;i<7;i++)
        {
            gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->label_lrc[i],100,150,(i+1)*10,(i+2)*10);
        }
        gtk_table_attach_defaults(GTK_TABLE(wp->table),wp->label_current_song_name,100,150,0,10);
   
        
        g_signal_connect(wp->main_wind,"destroy",G_CALLBACK(quit_app),shm_song_info);
        g_signal_connect(wp->button_start,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->button_next,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->button_prev,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->button_volup,"clicked",G_CALLBACK(deal_button),&volume);
        g_signal_connect(wp->change_skin,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->button_exit,"clicked",G_CALLBACK(quit_app),shm_song_info);
        g_signal_connect(wp->button_song_ctrl,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->button_create_chlid_wind,"clicked",G_CALLBACK(deal_button),&lrc_info_structure);
        g_signal_connect(wp->clist, "select-row", G_CALLBACK(selection_made), &lrc_info_structure);
        g_signal_connect(wp->eventbox, "motion_notify_event", G_CALLBACK(deal_event_motion), (gpointer)wp->progress);
        g_signal_connect(wp->eventbox,"button-release-event",G_CALLBACK(deal_event_release),shm_song_info[1].shmadd);
        g_signal_connect(wp->eventbox_vol, "motion_notify_event", G_CALLBACK(deal_event_motion), (gpointer)wp->progress_vol);
        g_signal_connect(wp->eventbox_vol,"button-release-event",G_CALLBACK(deal_event_release),&volume);

        g_timeout_add(100,set_label_song_time,shm_song_info);

        g_timeout_add(500,set_label_time_length,shm_song_info[1].shmadd);

        g_timeout_add(500,set_label_file_name,shm_song_info[2].shmadd);

        g_timeout_add(100,set_label_lrc,shm_song_info[0].shmadd);
        

        GdkPixbufAnimation *gif_buf = create_pixbuf_animation_from_date("./gif/1.gif", 500, 500);
        
        GtkWidget *image = gtk_image_new();		// 创建空的图片控件
        gtk_image_set_from_animation(GTK_IMAGE(image), gif_buf); // 加载动画
        g_object_unref(gif_buf);	// 释放gif_buf
        
        //gtk_container_add(GTK_CONTAINER(window), image);

        gtk_table_attach_defaults(GTK_TABLE(wp->table),image,30,70,30,70);


        gtk_widget_show_all(wp->main_wind);
        gtk_widget_hide(wind.scrolled_window);
        gtk_main(); // 主事件循环
    
    }
    return 0;
}
