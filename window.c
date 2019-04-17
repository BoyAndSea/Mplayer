#include "window.h"




WINDOW *Create_Main_Wind(WINDOW * wind,int *argc, char ***argv,char *title,char *background_file_path)
{
    gtk_init(argc,argv);
    wind->main_wind=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(wind->main_wind),title);
    gtk_widget_set_size_request(wind->main_wind,1024,600);
    gtk_window_set_resizable(GTK_WINDOW(wind->main_wind),FALSE);
    gtk_window_set_position(GTK_WINDOW(wind->main_wind),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(wind->main_wind),5);
    
    
    chang_background(wind->main_wind, 1034, 600, background_file_path);

    return wind;
}

void chang_background(GtkWidget *widget, int w, int h, const gchar *path)
{
	gtk_widget_set_app_paintable(widget, TRUE);		//允许窗口可以绘图
	gtk_widget_realize(widget);	
 
	/* 更改背景图时，图片会重叠
	 * 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。
	 */
	gtk_widget_queue_draw(widget);
 
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL);	// 创建图片资源对象
	// w, h是指定图片的宽度和高度
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);
 
	GdkPixmap *pixmap = NULL;
 
	/* 创建pixmap图像; 
	 * NULL：不需要蒙版; 
	 * 123： 0~255，透明到不透明
	 */
	gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
 
	// 释放资源
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	g_object_unref(pixmap);
}


GdkPixbufAnimation *create_pixbuf_animation_from_date(const char *path, int w, int h)
{
	FILE  *fp = NULL;
	
	fp = fopen(path,"rb");		// 打开文件
	if(NULL == fp){
		printf("open %s faild\n", path);
		return NULL;
	}
	// 获得文件的大小
	fseek(fp, 0, SEEK_END);
	unsigned long int FILE_SIZE = ftell(fp);
	
	rewind(fp);		//一定要把“文件指针”移动到文件开始处
	
	char *buffer = (char *)malloc(FILE_SIZE);
	
	if( 0 == fread(buffer, FILE_SIZE, 1, fp) ){	// 把gif图片写到buffer缓冲区
		printf("read from file faild\n");
		return NULL;
	}
	
	fclose(fp);
	
	GdkPixbufLoader *pixbuf_loader = gdk_pixbuf_loader_new(); // create a pixbuf loader object.
	// 调用完gdk_pixbuf_loader_new()，就得指定其大小，否则，无效。不指定，原始大小
	gdk_pixbuf_loader_set_size(pixbuf_loader, w, h);	// 指定动画的大小，
	
	gdk_pixbuf_loader_write( pixbuf_loader, buffer, FILE_SIZE, 0 ); // parse the image.
	// tell the pixbuf loader the end of the parsing the image.
	gdk_pixbuf_loader_close(pixbuf_loader, 0 ); 
	
	// get the new pixbuf just created by the pixbuf loader.
	GdkPixbufAnimation *gif_buf = gdk_pixbuf_loader_get_animation(pixbuf_loader); 
	
	free(buffer);
	buffer = NULL;
	
	return gif_buf;
}

void set_label_font_size(GtkWidget *label, int size)
{
    PangoFontDescription *font; // 字体指针
    font = pango_font_description_from_string(""); //参数为字体名字，任意即可
    // #define PANGO_SCALE 1024
    pango_font_description_set_size(font, size*PANGO_SCALE);// 设置字体大小
    gtk_widget_modify_font(label, font); // 改变 label 的字体大小
    pango_font_description_free(font); // 释放字体指针分配的空间
}

void sungtk_clist_set_label_color(GtkWidget *label, const char *color_buf)
{
	GdkColor color;
	gdk_color_parse(color_buf, &color);
	gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
}



GtkWidget *create_button_from_file(const char *file_path, const int w, const int h)
{
    GtkWidget *temp_image=gtk_image_new_from_pixbuf(NULL);
    load_image(temp_image,file_path,w,h);

    GtkWidget *button=gtk_button_new(); //创建空按钮
    gtk_button_set_image(GTK_BUTTON(button),temp_image);    //给按钮设置图标
    gtk_button_set_relief(GTK_BUTTON(button),GTK_RELIEF_NONE);    //按钮背景颜色透明
    return button;
}

void load_image(GtkWidget *image,const char *file_path,const int w,const int h)
{
    gtk_image_clear(GTK_IMAGE(image));  //清除图像
    GdkPixbuf *src_pixbuf=gdk_pixbuf_new_from_file(file_path,NULL); //创建图片资源
    GdkPixbuf *dest_pixbuf=gdk_pixbuf_scale_simple(src_pixbuf,w,h,GDK_INTERP_BILINEAR);    //指定大小
    gtk_image_set_from_pixbuf(GTK_IMAGE(image),dest_pixbuf);    //重新设一张图片
    g_object_unref(src_pixbuf); //释放资源
    g_object_unref(dest_pixbuf);    //释放资源
}


void set_button_image(char* picture_path,GtkWidget *button)
{
	GtkWidget *temp_image=gtk_image_new_from_pixbuf(NULL);
	load_image(temp_image,picture_path,50,50);
	gtk_button_set_image(GTK_BUTTON(button),temp_image);   
}




void deal_button(GtkButton *button,gpointer user_data)
{
	
    if(button == GTK_BUTTON(wind.change_skin))
    {
        
		struct struct_lrc *p2=(struct struct_lrc *)user_data;
		set_button_image("./ico/skin.png",wind.change_skin);
        if(p2->skin_code == p2->skin_num)
        {
            p2->skin_code=1;
        }
        else
        {
            p2->skin_code+=1;
        }

        LRC *p=link_s(&p2->skin_list_head,p2->skin_code,0);
		if(p!=NULL)
        	chang_background(wind.main_wind, 1024, 600, p->lrc_buf);
    }
    else if(button == GTK_BUTTON(wind.button_next))
    {
		struct struct_lrc *p=(struct struct_lrc *)user_data;
		set_button_image("./ico/right-circle.png",wind.button_next);
		set_button_image("./ico/close-stop.png",wind.button_start);
		if(lrc_info_structure.play_order_flag == 0)
		{
			write_to_pid_lrc("pt_step 1\n");
		}
		else
		{
			if(p->song_code==p->song_num)
			{
				p->song_code=1;
				char buf[20]="";
				sprintf(buf,"pt_step -%d\n",p->song_num-1);
				//write_fifo_cmd(buf);
				write_to_pid_lrc(buf);
			}
			else
			{
				p->song_code++;
				//write_fifo_cmd("pt_step 1\n");
				write_to_pid_lrc("pt_step 1\n");
			}
			
		}
		p->button_start_flag=1;
    }
    else if(button == GTK_BUTTON(wind.button_prev))
    {
        struct struct_lrc *p=(struct struct_lrc *)user_data;
		set_button_image("./ico/left-circle.png",wind.button_prev);
		set_button_image("./ico/close-stop.png",wind.button_start);
		
		if(lrc_info_structure.play_order_flag == 0)
		{
			write_to_pid_lrc("pt_step 1\n");
		}
		else
		{
			if(p->song_code<=1)
			{
				p->song_code=p->song_num;
				char buf[20]="";
				sprintf(buf,"pt_step %d\n",p->song_num-1);
				//write_fifo_cmd(buf);
				write_to_pid_lrc(buf);
			}
			else
			{
				p->song_code--;
				//write_fifo_cmd("pt_step -1\n");
				write_to_pid_lrc("pt_step -1\n");
			}
		}
		p->button_start_flag=1;
    }
    else if(button == GTK_BUTTON(wind.button_start))
    {
		struct struct_lrc *p=(struct struct_lrc *)user_data;
		if(p->button_start_flag==0)
		{
			set_button_image("./ico/close-stop.png",wind.button_start);
			p->button_start_flag=1;	
			write_to_pid_lrc("loop\n");	
		}
		else if(p->button_start_flag==1)
		{
			set_button_image("./ico/play-start.png",wind.button_start);
			p->button_start_flag=2;
			write_to_pid_lrc("pause\n");
		}
		else if(p->button_start_flag==2)
		{
			set_button_image("./ico/close-stop.png",wind.button_start);
			p->button_start_flag=1;
			write_to_pid_lrc("pause\n");
		}
		//printf("%d\n",p->button_start_flag);
    }
	else if(button == GTK_BUTTON(wind.button_song_ctrl))
	{
		if(lrc_info_structure.button_start_flag != 0)
		{

			if(lrc_info_structure.play_order_flag == -1)
			{
				write_to_pid_lrc("loop 1\n");
				if(lrc_info_structure.button_start_flag==2)
					write_to_pid_lrc("pause\n");
				lrc_info_structure.play_order_flag=0;
				set_button_image("./ico/单曲循环_32.png",wind.button_song_ctrl);
				
			}
			else if(lrc_info_structure.play_order_flag == 0) 
			{
				write_to_pid_lrc("loop -1\n");
				if(lrc_info_structure.button_start_flag==2)
					write_to_pid_lrc("pause\n");
				lrc_info_structure.play_order_flag=1;
				set_button_image("./ico/循环播放_32.png",wind.button_song_ctrl);
			}
			else if(lrc_info_structure.play_order_flag == 1)
			{
				write_to_pid_lrc("loop 1\n");
				if(lrc_info_structure.button_start_flag==2)
					write_to_pid_lrc("pause\n");
				lrc_info_structure.play_order_flag=0;
				set_button_image("./ico/单曲循环_32.png",wind.button_song_ctrl);
			}
		}
	}
	else if(button == GTK_BUTTON(wind.button_create_chlid_wind))
	{
		if(lrc_info_structure.sorclled_window_hide_flag==0)
		{
			lrc_info_structure.sorclled_window_hide_flag=1;
			gtk_widget_show(wind.scrolled_window);
		}
		else if(lrc_info_structure.sorclled_window_hide_flag==1)
		{
			lrc_info_structure.sorclled_window_hide_flag=0;
			gtk_widget_hide(wind.scrolled_window);
		}
		
	}
	
}

void selection_made( GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data )
{
	gchar *text;
	if(column==1)
		gtk_clist_get_text(GTK_CLIST(clist), row, column, &text);
	else if(column==0)
		gtk_clist_get_text(GTK_CLIST(clist), row, column+1, &text);

	set_button_image("./ico/close-stop.png",wind.button_start);
	
	
	struct struct_lrc *p=(struct struct_lrc *)data;
	
	if((p->button_start_flag==0)&&(row==0))
	{
	
		write_to_pid_lrc("pause\n");
		write_to_pid_lrc("pause\n");
		
	}
	else
	{

		if(lrc_info_structure.play_order_flag == 0)
		{
			write_to_pid_lrc("loop -1\n");

			int diff=(row+1)-(p->song_code);
			p->song_code=row+1;
			if(diff==0)
			{
				;
			}
			else
			{
				char buf[20]="";
				sprintf(buf,"pt_step %d\n",diff);
				write_to_pid_lrc(buf);
			}

			write_to_pid_lrc("loop 1\n");
		}
		else
		{
			int diff=(row+1)-(p->song_code);
			p->song_code=row+1;
			if(diff==0)
			{
				;
			}
			else
			{
				char buf[20]="";
				sprintf(buf,"pt_step %d\n",diff);
				write_to_pid_lrc(buf);
			}
		}
	}
	p->button_start_flag=1;
}

void deal_event_motion(GtkWidget *eventbox, GdkEventMotion *event, gpointer guidata)
{
	if(eventbox == wind.eventbox)
	{
		if(lrc_info_structure.button_start_flag!=0)
		{
			double x=event->x;
			if(x>693)
				x=693;
			if(x<1)
				x=1;
			coor_percent=x/693;
			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(guidata), coor_percent);
		}
	}
	else if (eventbox == wind.eventbox_vol)
	{
		double x=event->x;
		if(x>120)
			x=120;
		if(x<1)
			x=1;
		coor_percent_vol=x/120;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(guidata), coor_percent_vol);
		volume=(int)(coor_percent_vol*100);
		printf("%d\n",volume);
		char cmd[50]="";
		sprintf(cmd,"volume %d 1\n",volume);
		write_to_pid_lrc(cmd);
		usleep(50);
		lrc_info_structure.button_start_flag=1;
		set_button_image("./ico/close-stop.png",wind.button_start);
	}
}

gboolean deal_event_release( GtkWidget *eventbox, GdkEventButton *event, gpointer guidata )
{

		double diff=coor_percent-progress_pos;
		double time=atof((char *)guidata);

		printf("%lf\t%d\t%d\n",diff,(int)time,(int)(time*diff));
		if(lrc_info_structure.button_start_flag!=0)
		{
			char cmd[50]="";
			sprintf(cmd,"seek %d\n",(int)(time*diff));
			write_to_pid_lrc(cmd);
		}
		return TRUE;
	

}

gboolean set_label_song_time(gpointer data)
{
	struct shm_inf *song_info=(struct shm_inf *)data;
	int time_pos=(int)(atof((char *)song_info[0].shmadd)*10);
	char *markup = g_markup_printf_escaped ("<span foreground=\"#000000\">%d:%02d</span>",time_pos/600,time_pos/10%60);
	gtk_label_set_markup(GTK_LABEL(wind.label_percent), markup);
	g_free (markup);

	progress_pos=atof((char* )song_info[0].shmadd)/atof((char *) song_info[1].shmadd);
	if((progress_pos>=0)&&(progress_pos<=1.0))
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(wind.progress), progress_pos);
	return TRUE;
}

gboolean set_label_time_length(gpointer data)
{
	int time_length=(int)(atof((char *)data)*10);
	char *markup = g_markup_printf_escaped ("<span foreground=\"#000000\">%d:%02d</span>",time_length/600,time_length/10%60);
	gtk_label_set_markup(GTK_LABEL(wind.label_time), markup);
	g_free (markup);
	return TRUE;
}

gboolean set_label_file_name(gpointer data)
{
	char *str=(char *)data;
	static int code=1;
	static char song_lrc_path[200]="  ";
	code=link_code(&lrc_info_structure.song_list_head,str);
	//printf("code\t%d\n",code);
	//printf("song_code\t%d\n",lrc_info_structure.song_code);
	if(!(code<1))
	{
		lrc_info_structure.song_code=code;

		LRC*pnew=link_s(&lrc_info_structure.song_list_head,lrc_info_structure.song_code,0); //获得当前歌曲路径的节点地址
		if(pnew!=NULL)
		{
			strcpy(lrc_info_structure.song_path,pnew->lrc_buf);
			char *p=lrc_info_structure.song_path;
			while(*p)
			{
				p++;
			}
			p--;*p='c';
			p--;*p='r';
			p--;*p='l';
			if(strcmp(song_lrc_path,lrc_info_structure.song_path)!=0)
			{
				strcpy(song_lrc_path,lrc_info_structure.song_path);
				printf("%s\n",song_lrc_path);

				lrc_save_to_link_arry(lrc_info_structure.lrc_sentence,
				&lrc_info_structure.num_of_sentence,
				&lrc_info_structure.song_lrc_head,
				lrc_info_structure.song_path,
				lrc_info_structure.lrc_time);  //将歌词文件存储到链表
				int i;
				for(i=0;i<7;i++)
				{
					set_label_font_size(wind.label_lrc[i], 15);
					if(i==3)
						gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#00e080\"> 载入歌词中  </span>");
					else
					{
						gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#8080ff\">  </span>");
					}
				}
				//printf_lrc_info();
			}
		}
	}
	if(code>=1)
	{
		int i;
		gdk_color_parse("white",&wind.color);
		for(i=0;i<lrc_info_structure.song_num;i++)
		{
			gtk_clist_set_background(GTK_CLIST(wind.clist),i,&wind.color);
		}
		gdk_color_parse("green",&wind.color);
		gtk_clist_set_background(GTK_CLIST(wind.clist),code-1,&wind.color);
	}
	set_label_font_size(wind.label_current_song_name, 10);
	char *markup = g_markup_printf_escaped ("<span foreground=\"#000000\">%d. %s</span>",lrc_info_structure.song_code ,str);
	gtk_label_set_markup(GTK_LABEL(wind.label_current_song_name), markup);
	g_free(markup);
	return 1;
}

gboolean set_label_lrc(gpointer data)
{
	int i;
	struct struct_lrc *p=(struct struct_lrc *)data;
	if(lrc_info_structure.button_start_flag==0)
	{

		for(i=0;i<7;i++)
		{
		
			set_label_font_size(wind.label_lrc[i], 15);
			if(i==3)
			{
				gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#000000\">选择一首歌曲播放吧</span>");
			}
			else
			{
				gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#8080ff\">  </span>");
			}
		}
	}
	else
	{
		

		if(lrc_info_structure.num_of_sentence==0)
		{
			for(i=0;i<7;i++)
			{
				set_label_font_size(wind.label_lrc[i], 15);
				if(i==3)
				{
					gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#00e080\"> 没有歌词  </span>");
				}
				else
				{
					gtk_label_set_markup(GTK_LABEL(wind.label_lrc[i]), "<span foreground=\"#8080ff\">  </span>");
				}
			}
		}
		else 
		{
			char *str_lrc[8]={NULL};
			LRC *pnew;
			int time_pos=(int)(atof((char *)data)*100);
			//printf("%d\n",time_pos);
			
			int *lrc_time_p=array_s(time_pos,lrc_info_structure.lrc_time,200);
			if(lrc_time_p!=NULL)
			{
				for(i=0;i<4;i++)
				{
					pnew=link_s(&lrc_info_structure.song_lrc_head,*(lrc_time_p+i),0);
					if(pnew!=NULL)
						str_lrc[i]=pnew->lrc_buf;	

					
					pnew=link_s(&lrc_info_structure.song_lrc_head,*(lrc_time_p-i),0);
					if(pnew!=NULL)
						str_lrc[4+i]=pnew->lrc_buf;	
				}
			}
			

			for(i=0;i<4;i++)
			{
				if(i==0)
				{
					if(str_lrc[0]!=NULL)
					{
						char *markup = g_markup_printf_escaped ("<span foreground=\"#00ff00\">%s</span>",str_lrc[0]);
						gtk_label_set_markup(GTK_LABEL(wind.label_lrc[3]),markup);
						g_free(markup);
					}
				}
				else
				{
					if(str_lrc[4+i]!=NULL)
					{
						char *markup = g_markup_printf_escaped ("<span foreground=\"#000000\">%s</span>",str_lrc[4+i]);
						gtk_label_set_markup(GTK_LABEL(wind.label_lrc[3+i]), markup);
						g_free(markup);
					}
					
					if(str_lrc[i]!=NULL)
					{
						char *markup = g_markup_printf_escaped ("<span foreground=\"#000000\">%s</span>",str_lrc[i]);
						gtk_label_set_markup(GTK_LABEL(wind.label_lrc[3-i]),markup);
						g_free(markup);
					}
				}
			}
		}	
	}	
	return 1;
}


void quit_app(GtkWidget * window,gpointer user_data)
{

  
	kill(pid_lrc,9);
	kill(pid_play,9);
	
	struct shm_inf *p=(struct shm_inf *)user_data;
	del_shm(p[0].shmadd,p[0].shmid);
	del_shm(p[1].shmadd,p[1].shmid);
	del_shm(p[2].shmadd,p[2].shmid);
	system("rm father_to_pid_lrc_cmd");
	system("rm fifo_cmd");

	gtk_main_quit();
}



