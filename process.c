#include "process.h"





void print_label(GtkWidget *label,char *str)
{
	gtk_label_set_text(GTK_LABEL(label),str);
}








void str_tran(char str[])
{
	char buf[100]="0";
	char *p1=str,*p2=buf;
	while(*p1)
	{
		if(*p1 == ' ')
		{
			*p2='\\';
			p2++;
		}
		*p2=*p1;
		p2++;
		p1++;
	}
	*p2='\0';
	strcpy(str,buf);
	
}

void write_fifo_cmd(char *cmd)
{

	int fd;
	int ret;
	ret=mkfifo("fifo_cmd",0777);
	fd=open("fifo_cmd",O_WRONLY);
	if(fd<0)
	{
		perror("open fifo");
	}
	write(fd,cmd,strlen(cmd));
	close(fd);

}

void write_to_pid_lrc(char *cmd)
{
	int fd;
	int ret;

	ret=mkfifo("father_to_pid_lrc_cmd",0777);
	fd=open("father_to_pid_lrc_cmd",O_WRONLY);
	if(fd<0)
	{
		perror("open father_to_pid_lrc_cmd");
	}
	write(fd,cmd,strlen(cmd));
	close(fd);
}

