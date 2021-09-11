/**
*********************************************************************************************************
*                                                decoration_mode
*                                      (c) Copyright 2020-2030,liwei
*                                           All Rights Reserved
*
* @File    : main.c
* @By      : liwei
* @Version : V0.01
*
*********************************************************************************************************
**/
/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/
#include<stdio.h>
#include<string.h>
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/
typedef struct  decoration_base_def
{
	struct decoration_base_def *node_next;
	struct decoration_base_def *node_previous;
	char name[20]; 
	void (*func)(void);
	char flag;
}decoration_base_t;  /* װ������*/
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
#define POOL_NUM 50
decoration_base_t decoration_pool[POOL_NUM+1];  /* װ���ڴ��*/
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/

/*
��־��
0901 
��� װ�γ�;���װ��������䣬���װ�ι�������
���⣺��Ҫ����һ��ָ��ָ���ϸ�װ����� �γ�һ��˫������
0904
���˫������ 
��������
�����ļ�ͷ 
*/

/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void decoration_create(decoration_base_t* object ,void (*func)(void))
{
	/* ��ʼ��һ��װ�����*/
	object->node_next = NULL;
	object->node_previous = NULL;
	object->func = func;
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
decoration_base_t* decoration_pool_find_idle(void)
{
	/* Ѱ��һ������װ�����*/
	for(char i = 0 ; i < POOL_NUM ; i ++)
	{
		if(decoration_pool[i].flag == 0)
		{
			decoration_pool[i].flag = 1;
			/* ���ؿ���װ�������ַ*/
			return	&decoration_pool[i];		
		}
	}
	return NULL;
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void decoration_add_node(decoration_base_t* object ,decoration_base_t* container,void (*func)(void), char *name ,char name_length)
{
	decoration_base_t* ram_i;

	/* �ж�����Ƿ�Ϊβ��*/
	if(object->node_next !=NULL)
	{
		/*�ݹ����*/
		ram_i =	 object->node_next;
		decoration_add_node(ram_i, container , func , name , name_length);
	}
	else
	{
		/*����������� �͵�ַ*/
		object->node_next = container;
		container->node_previous = object;
		container->func = func;
		container->node_next = NULL;
		memcpy(container->name , name , name_length);
		printf("���ӹ���%s\n\r",name);
	}
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void decoration_add_function(decoration_base_t* object ,void (*func)(void) , char *name ,char name_length)
{
	decoration_base_t* user_container;
	/*������нڵ�*/
	user_container = decoration_pool_find_idle();
	/*���ӹ��ܽڵ�*/
	decoration_add_node(object, user_container, func ,name , name_length);
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void decoration_delete_function(decoration_base_t* object ,void (*func)(void))
{
	decoration_base_t* ram_i;
	/*�жϲ��Ǹ��ڵ�*/
	if(object->node_previous !=NULL)
	{
		/* �ж�����Ƿ�Ϊβ��*/
		if(object->node_next !=NULL)
		{
			/* �м�ڵ� ɾ�����ܣ� �Ͻڵ�ĺ�ָ�� ���½ڵ�ǰָ�� �޸Ķ���*/
			if(object->func == func)
			{
				object->node_previous->node_next = object->node_next;
				object->node_next->node_previous = object->node_previous;
				object->flag = 0;
				printf("ɾ������%s\n\r",object->name);		
			}
			/*�ݹ����*/
			ram_i =	 object->node_next;		
			decoration_delete_function(ram_i, func);
		}
		else
		{
			/* β���ڵ� ɾ�����ܣ� �Ͻڵ�ĺ�ָ���ֵNULL*/
			if(object->func == func)
			{
				object->node_previous->node_next = NULL;
				object->flag = 0;
				printf("ɾ������%s\n\r",object->name);	
			}				
		}
	}
	else
	{
		/*�ݹ����*/
		ram_i =	 object->node_next;		
		decoration_delete_function(ram_i, func);	
	} 
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void implement_decoration_function(decoration_base_t* user)
{
	decoration_base_t* ram_i;
	/*ִ�б��ι���*/
	user->func();
	/*�ݹ����*/
	if(user->node_next !=NULL)
	{
		ram_i =	 user->node_next;
		implement_decoration_function(ram_i);
	}
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void function_create(void)
{
	printf("ִ�и����function_create\n\r");
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/

void function_test2(void)
{
	printf("ִ��function_test2\n\r");
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void function_test3(void)
{
	printf("ִ��function_test3\n\r");
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
void function_test4(void)
{
	printf("ִ��function_test4\n\r");
}
/*********************************************************************************************************
@����
@����
*********************************************************************************************************/
int main(int argc, char *argv[])
{
	for(char i = 0 ; i < POOL_NUM ; i ++)
		decoration_pool[i].flag = 0;
	decoration_base_t user;
	/*��ʼ��*/
	decoration_create(&user, function_create);
	implement_decoration_function(&user);	/*ִ��*/
	/*���ӹ���*/
	decoration_add_function(&user, function_test2, "function_test2" , sizeof("function_test2"));
	implement_decoration_function(&user);/*ִ��*/
	/*���ӹ���*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*ִ��*/
	/*���ӹ���*/
	decoration_add_function(&user, function_test4, "function_test4" , sizeof("function_test4"));
	implement_decoration_function(&user);/*ִ��*/
	/*���ӹ���*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*ִ��*/
	/*ɾ������*/		
	decoration_delete_function(&user, function_test3);
	implement_decoration_function(&user);/*ִ��*/
	/*ɾ������*/	
	decoration_delete_function(&user, function_test4);
	implement_decoration_function(&user);/*ִ��*/	
	/*���ӹ���*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*ִ��*/		
	return 0;
}

/***************************************************END*****************************************************/


