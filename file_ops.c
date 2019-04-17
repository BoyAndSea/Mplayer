#include "file_ops.h"


void link_print(struct list_head *head)
{
	LRC *tmp;
	list_for_each_entry(tmp, head, node)
		printf("%02d:%02d.%02d %s\n",tmp->time/6000,tmp->time%6000/100,tmp->time%100,tmp->lrc_buf);	
	
}

struct list_head *link_creat(struct list_head *head,LRC *pnew)
{
	
	//printf("开始插入链表\n");

	list_add(&pnew->node, head);
	//printf("插入成功\n\n");
	return head;
}


LRC *link_s(struct list_head *head,int num,unsigned int offset)
{
	LRC *tmp=NULL,*tmpcp=NULL,*tmp1=NULL,*tmp2=NULL,*tmp3=NULL;
	
	list_for_each_entry(tmp, head, node)
	{
		if(tmp!=tmpcp)
		{
			tmp3=tmp2;
			tmp2=tmp1;
			tmp1=tmpcp;
			tmpcp=tmp;
		}
		if(tmp->time == num)
		{
			switch(offset)
			{
				case 0:return tmp;
				case 1:return tmp1;
				case 2:return tmp2;
				case 3:return tmp3;
			}
			
		}
	}		

	return NULL;
} 

int link_code(struct list_head *head,char *buf)
{
	LRC *tmp;
	if(strlen(buf)!=0)
	{
		list_for_each_entry(tmp, head, node)
		{
			char *str[2];
			char lrc_buf[200]="";
			strcpy(lrc_buf,tmp->lrc_buf);
			str[0]=strtok(lrc_buf,"/");
			str[1]=strtok(NULL,"/");
			if(strcmp(str[1],buf)==0)
				return tmp->time;
		}
		return 0;
	}
	return -1;
}


int msg_deal(char *msg_src,char *msg_done[],char *str)
{
	int i=0;
	msg_done[i]=strtok(msg_src,str);
	while(msg_done[i]!=NULL)
	{
		i++;
		msg_done[i]=strtok(NULL,str);
	}
	return i;
}


char *file_open(char *file_name)
{
	 
	FILE *fp;
	char *ch;
	int length;
	
	if((fp=fopen(file_name,"rb"))==NULL)
	{
		printf("\n文件打开失败\n");
		return NULL;
	}
	else
	//printf("\n文件打开成功\n");
	
	fseek(fp,0,SEEK_END);
	length=ftell(fp);
	//printf("\n文件长度 %d个字节\n",length);
	
	rewind(fp);
	
	ch=malloc(sizeof(char)*(length+1));
	if(ch==NULL)
	{
		printf("\n内存申请失败\n");
		return NULL;
	}
	else
		//printf("\n内存申请成功,一共%d个字节\n",length+1);
	
	fread(ch,length,1,fp);
	ch=ch+length;
	*ch='\0';
	ch=ch-length;
	//printf("\n%s\n",ch);
	
	fclose(fp);
	return ch;
}


void link_to_array(struct list_head *head,int *array)
{
	LRC *tmp;
	int i=0;
	list_for_each_entry(tmp, head, node)
	{
		array[i]=tmp->time;
		i++;
	}
}

int *array_s(int tar,int*array,int array_len)
{
	int *p;
	int i;
	for(i=0;i<array_len;i++)
	{
		if(*(array+i)==tar)
		{
			p=array+i;
			return p;
		}
	}
	return NULL;
}


char * LoadInfo(char *fold_name)
{
    if(strcmp(fold_name,"skin")==0)
    {
        char file[]="skin_list";
        system("ls skin/*.jpg > skin_list");
        return  file_open(file);
    }
    else if(strcmp(fold_name,"picture")==0)
    {
        char file[]="picture_list";
        system("ls picture/*.jpg picture/*.bmp > picture_list"); 
	    return file_open(file);
    }
	else if(strcmp(fold_name,"Music")==0)
	{
		char file[]="Music_Mp3_list";
		system("ls Music/*.mp3 > Music_Mp3_list");
		return file_open(file);
	}

	return NULL;
}


int list_save_to_link(struct list_head *song_link_head,char *fold_name)
{
	char *q2[100],*songname;
	LRC* pnew;
	int len=0,k=0;

	songname=LoadInfo(fold_name);
	if(songname==NULL)
		return 0;
	INIT_LIST_HEAD(song_link_head);
	len=msg_deal(songname,q2,"\r\n");
	
	for(k=0;k<len;k++)
	{
		pnew=malloc(sizeof(LRC));
		if(pnew==NULL)
		{
			printf("内存申请失败\n");
			return ;
		}
		else
			//printf("内存申请成功\n");
			
			
		pnew->time=k+1;
		stpcpy(pnew->lrc_buf,q2[k]);
		//printf("写入成功\n");
		
		link_creat(song_link_head,pnew);
	}
	
	//link_print(song_link_head);
	return len;
	
}



void lrc_save_to_link_arry(char *q[],int *len,struct list_head *head,char *argv,int *lrc_time)
{

	int i;
	for(i=0;i<200;i++)
	{
		q[i]=NULL;
		lrc_time[i]=0;
	}
	*len=0;
	INIT_LIST_HEAD(head);
	
	char *lrc_buf=file_open(argv);
	if(lrc_buf==NULL)
	{
		printf("歌词文件为不存在\n");
		
		return;
	}
	
	
	LRC* pnew;
	int k=0,j=0,temp=0;
	char *str=NULL;
	int buff[5]={0};
	
	
	*len=msg_deal(lrc_buf,q,"\r\n");
	
	for(k=5;k<*len;k++)
	{
		str=q[k];
		i=0;
		
		while(*str=='[')
		{
			buff[i]=atoi(str+1)*6000+atoi(str+4)*100;
			i++;
			str+=10;
		}
		for(j=0;j<i;j++)
		{
			pnew=malloc(sizeof(LRC));
			if(pnew==NULL)
			{
				printf("内存申请失败\n");
				return ;
			}
			else
				//printf("内存申请成功\n");
			
			//printf("buff[%d]\t%d\t%s\n",j,buff[j],str);
			
			pnew->time=buff[j];
			stpcpy(pnew->lrc_buf,str);
			//printf("写入成功\n");
			
			link_creat(head,pnew);
		}
		
	}
	link_to_array(head,lrc_time);
	
	for(i=0;i<200;i++)				//升序排列
		for(j=i+1;j<200;j++)
		{
			if(lrc_time[i]<lrc_time[j])
			{
				temp=lrc_time[i];
				lrc_time[i]=lrc_time[j];
				lrc_time[j]=temp;
			}
		}
		
	//for(i=0;i<200;i++)
	//{
		//printf("%d\n",lrc_time[i]);
	//}

	//link_print(head);
	
}


void printf_lrc_info(void)
{
	printf("song_path\t%s\n",lrc_info_structure.song_path);
	printf("num_of_sentence\t%d\n",lrc_info_structure.num_of_sentence);
	printf("skin_code\t%d\n",lrc_info_structure.skin_code);
	printf("song_code\t%d\n",lrc_info_structure.song_code);
	printf("song_num\t%d\n",lrc_info_structure.song_num);
	printf("skin_num\t%d\n",lrc_info_structure.skin_num);
	printf("skin_list_head\t%p\n",&lrc_info_structure.skin_list_head);
	printf("song_list_head\t%p\n",&lrc_info_structure.song_list_head);
	printf("song_lrc_head\t%p\n",&lrc_info_structure.song_lrc_head);
}


