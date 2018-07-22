#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lyrics_process.h"

/*******************************************************
*���ܣ�     �򿪸���ļ��������ݵ�������
*������		����ļ���ַ��addr
*����ֵ��	�������׵�ַ
********************************************************/
static char *open_lrc(char *addr)
{
	FILE *fptr = NULL;
	char *lycris_buf = NULL;	//the address of malloc buf which saved lycris content
	int size =0;
	
	if(addr == NULL)
	{
		printf("error: addr or size is NULL!\n");
	}
	else
	{
		fptr = fopen(addr, "rb");
		if(fptr == NULL)
		{
		   // printf("open lycris [%s] error!\n",addr);
		}
		else
		{
			fseek(fptr, 0, SEEK_END);
			size = ftell(fptr);
			rewind(fptr);
			lycris_buf = (char *)malloc(size+1);
			bzero(lycris_buf, size+1);
			if(lycris_buf == NULL)
			{

				printf("malloc lycris_buf error\n");
			}
			else
			{

				fread(lycris_buf, size, 1, fptr);
			}
			fclose(fptr);
		}
	}
	return lycris_buf;
}

/*******************************************************
*���ܣ�     ����һ�������Ϣ�ڵ��������β
*������		����ı�ͷ��head
			���ʱ�䣺time
			��ʣ�lrc
*����ֵ��	����ı�ͷ��head
********************************************************/
static LRC *add_lrc_to_link(LRC *head, uint time, char *lrc)
{
	LRC *nextpr,*temp;
	nextpr = head;

	if(nextpr==NULL)			//��������
	{
        head = (LRC *)malloc(sizeof(LRC));
        if(head == NULL)
        {
            printf("�ڴ����ʧ�ܣ���������ʧ��\n");
            head = NULL;
        }
        else
        {
            head->time = time;
            strcpy(head->lrc, lrc);
            head->next = NULL;
        }
	}
	else	//����һ������Ԫ���������β
	{
		while(nextpr)
		{
 			temp = nextpr;
			nextpr = nextpr->next;
		}
		nextpr = (LRC *)malloc(sizeof(LRC));
		if(nextpr == NULL)
		{
			printf("�ڴ����ʧ�ܣ���������Ԫ��ʧ��\n");
		}
		else
		{
			nextpr->time = time;
			strcpy(nextpr->lrc, lrc);
			nextpr->next = NULL;
			temp->next = nextpr;
		}
	}
	return head;
}

/*******************************************************
*���ܣ�     �����������
*������		��ʾ����lrc
*����ֵ��	��
********************************************************/
void print_lrc_link(LRC_PTR lrc)
{
	uint i =0;

	for(i=0;i<lrc.lrc_arry_size;i++)
	{
		//printf("��%d���ڵ㣺\n", i);
		printf("ʱ��: %dms ���: %s\n", lrc.lrc_arry[i]->time, lrc.lrc_arry[i]->lrc);
	}
}

/*******************************************************
*���ܣ�     �����Ԫ�ذ�ʱ���С��С��������
*������		��ʾ����lrc
*����ֵ��	��
********************************************************/
static void inorder_link(LRC_PTR *lrc)
{
	LRC *nextpr,*temp;
	uint j,k;

	nextpr = lrc->lrc_head;
	lrc->lrc_arry_size = 0;
	while(nextpr)
	{
		lrc->lrc_arry_size++;
		nextpr = nextpr->next;
	}
	lrc->lrc_arry = calloc(lrc->lrc_arry_size, sizeof(lrc->lrc_arry));
	nextpr = lrc->lrc_head;
	for(j=0;j<lrc->lrc_arry_size;j++)
	{
		*(lrc->lrc_arry+j) = nextpr;		//���������Ԫ�ص�ַ
		nextpr = nextpr->next;
	}
	for(j=0;j<lrc->lrc_arry_size-1;j++)	//���ո�����Ԫ�ص�id��С�������򣬽���Ԫ�ط���ָ��������
	{
		for(k=j+1;k<lrc->lrc_arry_size;k++)
		{
			if(lrc->lrc_arry[j]->time > lrc->lrc_arry[k]->time)
			{
				temp = *(lrc->lrc_arry+k);
				lrc->lrc_arry[k] = lrc->lrc_arry[j];
				lrc->lrc_arry[j]= temp;
			}
		}
	}
	lrc->lrc_head = *(lrc->lrc_arry);
	for(j=0;j<(lrc->lrc_arry_size-1);j++)	//����ָ�������еĸ�����Ԫ�صĵ�ַ��˳�����¸�������ָ����
	{
		lrc->lrc_arry[j]->next = lrc->lrc_arry[j+1];
	}
	lrc->lrc_arry[lrc->lrc_arry_size-1]->next = NULL;
}

/*******************************************************
*���ܣ�     �ж��Ƿ�����ȷ��ʱ���ǩ
*������		ʱ���ǩ��label
*����ֵ��	1���ǣ�0������
********************************************************/
static char judge_time_label(char *label)
{
    //�Ϸ�ʱ���ǩʾ��[00:07.41
    if(label)//label�ǿ�
    {
        if(strlen(label) == 9)
        {
            if((*(label+0)=='[') && (*(label+3)==':') && (*(label+6)=='.')
                && (*(label+1)>='0' && *(label+1)<='9')
                && (*(label+2)>='0' && *(label+2)<='9')
                && (*(label+4)>='0' && *(label+4)<='9')
                && (*(label+5)>='0' && *(label+5)<='9')
                && (*(label+7)>='0' && *(label+7)<='9')
                && (*(label+8)>='0' && *(label+8)<='9'))
            {
                //printf("label=%s\n", label);
                return 1;
            }
        }
    }
    return 0;
}

/*******************************************************
*���ܣ�     ����ʱ���ǩ
*������		ʱ���ǩ��label
*����ֵ��	ʱ���ǩ�����ʱ��(��λ:ms)
********************************************************/
static uint calculate_time_label(char *label)
{
	uint mtime = 0;
    uint minute = 0;
	uint second = 0;
	uint msecond = 0;

	//printf("ʱ���ǩ��%s\n", label);
    sscanf(label, "[%d:%d.%d", &minute, &second, &msecond);
    //printf("minute=%d\n,second=%d\n,msecond=%d\n", minute, second, msecond);
    mtime = minute*60000 + second*1000 + msecond*10;
    return mtime;
}

/*******************************************************
*���ܣ�     �������ļ�һ����Ϣ
*������		�е�ַ��line
			����ı�ͷ��head
*����ֵ��	����ı�ͷ��head
********************************************************/
static LRC *dispose_line(char *line,LRC *head)
{
	uint i = 0;
	uint argc = 0;
	uint mtime = 0;
	char *lrc_text = NULL;
	char *argv[lrc_time_labels] = {NULL};

	//printf("line=%s=====\n", line);
	argv[argc] = strtok(line, "]");
    while((argv[++argc] = strtok(NULL,"]"))!=NULL)
    {
        if(argc>=lrc_time_labels)
        {
            printf("lrc: too many labels\n");
            break;
        }
    }
	/*
	printf("argc =%d\n", argc);
	for(i=0;i<argc;i++)
    {
		printf("argv[%d]=%s\n", i, argv[i]);
	}*/
    //ȡ�����
    if((judge_time_label(argv[0]) != 0) && (argv[argc-1] != NULL) && argc)
    {
       // printf("lyrics��%s\n", argv[argc-1]);
        lrc_text = argv[argc-1];
    }
	//printf("\n");
    for(i=0;i<argc && argc>1;i++)
    {
		//printf("argc = %d\targv[%d]=%s\n", argc, i, argv[i]);
        if(judge_time_label(argv[i]))//�ж�ʱ���ǩ
        {
			mtime = calculate_time_label(argv[i]);
            if(lrc_text && (strlen(lrc_text)>1))
            {
			//	printf("========%s======\n", lrc_text);
                head = add_lrc_to_link(head, mtime, lrc_text);
            }
        }
    }
	return head;
}

/*******************************************************
*���ܣ�     ���������ڵ�'\n'����'\0',����ÿ���滻ʱ
			�������ļ�����Ϣ
*������		�������׵�ַ��lycris_buf
			��ʾ����lrc
*����ֵ��	��
********************************************************/
static void get_dispose_line(char *lycris_buf, LRC_PTR *lrc)
{
	char *line_ptr = NULL;
	
	while((line_ptr = strtok(lycris_buf, "\n")) != NULL)
	{
		lycris_buf += strlen(lycris_buf)+1; 
		lrc->lrc_head = dispose_line(line_ptr, lrc->lrc_head);
	}
}

/*******************************************************
*���ܣ�     �������ļ�
*������		����ļ��� ��name
			��ʾ����lrc
*����ֵ��	�����Ϣ�ṹ��(����)
********************************************************/
LRC *dispose_lrc(char *name, LRC_PTR *lrc)
{
	char *lycris_buf = NULL;
	//printf("&&&&&&&&&&&&===%s\n", name);
	memset(lrc, 0, sizeof(LRC_PTR));	//init lrc struct
	lycris_buf = open_lrc(name);
	if(lycris_buf != NULL)
	{
		get_dispose_line(lycris_buf, lrc);
	 	free(lycris_buf);
        inorder_link(lrc);
	}
	else
	{
		printf("open_lrc error in dispose_lrc fun\n");
		return NULL;
	}
	return lrc->lrc_head;
}

/*******************************************************
*���ܣ�     �������ļ�
*������		��ʾ����lrc
*����ֵ��	NULL
********************************************************/
void free_lrc_arry(LRC_PTR *lrc)
{
	int i;
	if(lrc->lrc_arry != NULL)
	{
		for(i=0; i<lrc->lrc_arry_size; i++)
		{
			free(lrc->lrc_arry[i]);
		}
		free(lrc->lrc_arry);
	}
	lrc->lrc_arry = NULL;
}
