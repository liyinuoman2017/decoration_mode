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
}decoration_base_t;  /* 装饰类型*/
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
#define POOL_NUM 50
decoration_base_t decoration_pool[POOL_NUM+1];  /* 装饰内存池*/
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/

/*
日志：
0901 
完成 装饰池;完成装饰组件分配，完成装饰功能增加
问题：需要增加一个指针指向上个装饰组件 形成一个双向链表
0904
完成双向链表 
增加名字
增加文件头 
*/

/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void decoration_create(decoration_base_t* object ,void (*func)(void))
{
	/* 初始化一个装饰组件*/
	object->node_next = NULL;
	object->node_previous = NULL;
	object->func = func;
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
decoration_base_t* decoration_pool_find_idle(void)
{
	/* 寻找一个空闲装饰组件*/
	for(char i = 0 ; i < POOL_NUM ; i ++)
	{
		if(decoration_pool[i].flag == 0)
		{
			decoration_pool[i].flag = 1;
			/* 返回空闲装饰组件地址*/
			return	&decoration_pool[i];		
		}
	}
	return NULL;
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void decoration_add_node(decoration_base_t* object ,decoration_base_t* container,void (*func)(void), char *name ,char name_length)
{
	decoration_base_t* ram_i;

	/* 判断组件是否为尾部*/
	if(object->node_next !=NULL)
	{
		/*递归调用*/
		ram_i =	 object->node_next;
		decoration_add_node(ram_i, container , func , name , name_length);
	}
	else
	{
		/*分配操作函数 和地址*/
		object->node_next = container;
		container->node_previous = object;
		container->func = func;
		container->node_next = NULL;
		memcpy(container->name , name , name_length);
		printf("增加功能%s\n\r",name);
	}
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void decoration_add_function(decoration_base_t* object ,void (*func)(void) , char *name ,char name_length)
{
	decoration_base_t* user_container;
	/*分配空闲节点*/
	user_container = decoration_pool_find_idle();
	/*增加功能节点*/
	decoration_add_node(object, user_container, func ,name , name_length);
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void decoration_delete_function(decoration_base_t* object ,void (*func)(void))
{
	decoration_base_t* ram_i;
	/*判断不是根节点*/
	if(object->node_previous !=NULL)
	{
		/* 判断组件是否为尾部*/
		if(object->node_next !=NULL)
		{
			/* 中间节点 删除功能， 上节点的后指针 和下节点前指针 修改对象*/
			if(object->func == func)
			{
				object->node_previous->node_next = object->node_next;
				object->node_next->node_previous = object->node_previous;
				object->flag = 0;
				printf("删除功能%s\n\r",object->name);		
			}
			/*递归调用*/
			ram_i =	 object->node_next;		
			decoration_delete_function(ram_i, func);
		}
		else
		{
			/* 尾部节点 删除功能， 上节点的后指针幅值NULL*/
			if(object->func == func)
			{
				object->node_previous->node_next = NULL;
				object->flag = 0;
				printf("删除功能%s\n\r",object->name);	
			}				
		}
	}
	else
	{
		/*递归调用*/
		ram_i =	 object->node_next;		
		decoration_delete_function(ram_i, func);	
	} 
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void implement_decoration_function(decoration_base_t* user)
{
	decoration_base_t* ram_i;
	/*执行本次功能*/
	user->func();
	/*递归调用*/
	if(user->node_next !=NULL)
	{
		ram_i =	 user->node_next;
		implement_decoration_function(ram_i);
	}
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void function_create(void)
{
	printf("执行根组件function_create\n\r");
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/

void function_test2(void)
{
	printf("执行function_test2\n\r");
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void function_test3(void)
{
	printf("执行function_test3\n\r");
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
void function_test4(void)
{
	printf("执行function_test4\n\r");
}
/*********************************************************************************************************
@名称
@功能
*********************************************************************************************************/
int main(int argc, char *argv[])
{
	for(char i = 0 ; i < POOL_NUM ; i ++)
		decoration_pool[i].flag = 0;
	decoration_base_t user;
	/*初始化*/
	decoration_create(&user, function_create);
	implement_decoration_function(&user);	/*执行*/
	/*增加功能*/
	decoration_add_function(&user, function_test2, "function_test2" , sizeof("function_test2"));
	implement_decoration_function(&user);/*执行*/
	/*增加功能*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*执行*/
	/*增加功能*/
	decoration_add_function(&user, function_test4, "function_test4" , sizeof("function_test4"));
	implement_decoration_function(&user);/*执行*/
	/*增加功能*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*执行*/
	/*删除功能*/		
	decoration_delete_function(&user, function_test3);
	implement_decoration_function(&user);/*执行*/
	/*删除功能*/	
	decoration_delete_function(&user, function_test4);
	implement_decoration_function(&user);/*执行*/	
	/*增加功能*/
	decoration_add_function(&user, function_test3, "function_test3" , sizeof("function_test3"));
	implement_decoration_function(&user);/*执行*/		
	return 0;
}

/***************************************************END*****************************************************/


