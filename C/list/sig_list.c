#include "stdio.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAXSIZE 20 /* 存储空间初始分配量 */

typedef int Status;/* Status是函数的类型,其值是函数结果状态代码，如OK等 */
typedef int ElemType;/* ElemType类型根据实际情况而定，这里假设为int */

typedef struct Node
{
    ElemType data;
    struct Node *next;
}Node;
/* 定义LinkList */
typedef struct Node *LinkList;

/* 初始化顺序线性表 */
Status InitList(LinkList *L)
{
    *L=(LinkList)malloc(sizeof(Node)); /* 产生头结点,并使L指向此头结点 */
    if(!(*L)) /* 存储分配失败 */
    {
        return ERROR;
    }
    (*L)->next=NULL; /* 指针域为空 */

    return OK;
}

/* 初始条件：顺序线性表L已存在。操作结果：返回L中数据元素个数 */
int ListLength(LinkList L)
{
    int i=0;
    LinkList p=L->next; /* p指向第一个结点 */
    while(p)
    {
        i++;
        p=p->next;
    }
    return i;
}

/* 初始条件：顺序线性表L已存在。操作结果：将L重置为空表 */
Status ClearList(LinkList *L)
{
	LinkList p,q;
	p=(*L)->next;           /*  p指向第一个结点 */
	while(p)                /*  没到表尾 */
	{
		q=p->next;
		free(p);
		p=q;
	}
	(*L)->next=NULL;        /* 头结点指针域为空 */
	return OK;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：依次对L的每个数据元素输出 */
Status ListTraverse(LinkList L)
{
    LinkList p=L->next;
    while(p)
    {
        visit(p->data);
        p=p->next;
    }
    printf("\n");
    return OK;
}

Status visit(ElemType c)
{
    printf("-> %d ",c);
    return OK;
}

/* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/* 操作结果：用e返回L中第i个数据元素的值 */
Status GetElem(LinkList L,int i,ElemType *e)
{
	int j;
	LinkList p;		/* 声明一结点p */
	p = L->next;		/* 让p指向链表L的第一个结点 */
	j = 1;		/*  j为计数器 */
	while (p && j < i)  /* p不为空或者计数器j还没有等于i时，循环继续 */
	{
		p = p->next;  /* 让p指向下一个结点 */
		++j;
	}
	if ( !p || j>i )
		return ERROR;  /*  第i个元素不存在 */
	*e = p->data;   /*  取第i个元素的数据 */
	return OK;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：返回L中第1个与e满足关系的数据元素的位序。 */
/* 若这样的数据元素不存在，则返回值为0 */
int LocateElem(LinkList L,ElemType e)
{
    int i=0;
    LinkList p=L->next;
    while(p)
    {
        i++;
        if(p->data==e) /* 找到这样的数据元素 */
                return i;
        p=p->next;
    }

    return 0;
}

/*  随机产生n个元素的值，建立带表头结点的单链线性表L（头插法） */
void CreateListHead(LinkList *L, int n)
{
	LinkList p;
	int i;
	srand(time(0));                         /* 初始化随机数种子 */
	*L = (LinkList)malloc(sizeof(Node));
	(*L)->next = NULL;                      /*  先建立一个带头结点的单链表 */
	for (i=0; i < n; i++)
	{
		p = (LinkList)malloc(sizeof(Node)); /*  生成新结点 */
		p->data = rand()%100+1;             /*  随机生成100以内的数字 */
		p->next = (*L)->next;
		(*L)->next = p;						/*  插入到表头 */
	}
}

/*  随机产生n个元素的值，建立带表头结点的单链线性表L（尾插法） */
void CreateListTail(LinkList *L, int n)
{
	LinkList p,r;
	int i;
	srand(time(0));                      /* 初始化随机数种子 */
	*L = (LinkList)malloc(sizeof(Node)); /* L为整个线性表 */
	r=*L;                                /* r为指向尾部的结点 */
	for (i=0; i < n; i++)
	{
		p = (Node *)malloc(sizeof(Node)); /*  生成新结点 */
		p->data = rand()%100+1;           /*  随机生成100以内的数字 */
		r->next=p;                        /* 将表尾终端结点的指针指向新结点 */
		r = p;                            /* 将当前的新结点定义为表尾终端结点 */
	}
	r->next = NULL;                       /* 表示当前链表结束 */
}

/* 初始条件：顺序线性表L已存在,1≤i≤ListLength(L)， */
/* 操作结果：在L中第i个位置之前插入新的数据元素e，L的长度加1 */
Status ListInsert(LinkList *L,int i,ElemType e)
{
	int j;
	LinkList p,s;
	p = *L;     /* 声明一个结点 p，指向头结点 */
	j = 1;
	while (p && j < i)     /* 寻找第i个结点 */
	{
		p = p->next;
		++j;
	}
	if (!p || j > i)
		return ERROR;   /* 第i个元素不存在 */
	s = (LinkList)malloc(sizeof(Node));  /*  生成新结点(C语言标准函数) */
	s->data = e;
	s->next = p->next;      /* 将p的后继结点赋值给s的后继  */
	p->next = s;          /* 将s赋值给p的后继 */
	return OK;
}

/* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/* 操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1 */
Status ListDelete(LinkList *L,int i,ElemType *e)
{
	int j;
	LinkList p,q;
	p = *L;
	j = 1;
	while (p->next && j < i)	/* 遍历寻找第i个元素 */
	{
        p = p->next;
        ++j;
	}
	if (!(p->next) || j > i)
	    return ERROR;           /* 第i个元素不存在 */
	q = p->next;
	p->next = q->next;			/* 将q的后继赋值给p的后继 */
	*e = q->data;               /* 将q结点中的数据给e */
	free(q);                    /* 让系统回收此结点，释放内存 */
	return OK;
}

/* 单链表反转/逆序 */
Status ListReverse(LinkList L)
{
    LinkList current,pnext,prev;
    if(L == NULL || L->next == NULL)
        return L;
    current = L->next;  /* p1指向链表头节点的下一个节点 */
    pnext = current->next;
    current->next = NULL;
    while(pnext)
    {
        prev = pnext->next;
        pnext->next = current;
        current = pnext;
        pnext = prev;
    }
    //printf("current = %d,next = %d \n",current->data,current->next->data);
    L->next = current;  /* 将链表头节点指向p1 */
    return L;
}

Status ListReverse2(LinkList L)
{
    LinkList current, p;

    if (L == NULL)
    {
        return NULL;
    }
    current = L->next;
    while (current->next != NULL)
    {
        p = current->next;
        current->next = p->next;
        p->next = L->next;
        L->next = p;
        ListTraverse(L);
        printf("current = %d, \n", current -> data);
    }
    return L;
}

LinkList ListReverse3(LinkList list)
{
	LinkList  newList;    //新链表的头结点
	Node       *tmp;       //指向list的第一个结点，也就是要摘除的结点

	//
	//参数为空或者内存分配失败则返回NULL
	//
	if (list == NULL || (newList = (LinkList )malloc(sizeof(Node))) == NULL)
	{
		return NULL;
	}

	//
	//初始化newList
	//
	newList->data = list->data;
	newList->next = NULL;

	//
	//依次将list的第一个结点放到newList的第一个结点位置
	//
	while (list->next != NULL)
	{
		tmp = newList->next;         //保存newList中的后续结点
		newList->next = list->next;       //将list的第一个结点放到newList中
		list->next = list->next->next;     //从list中摘除这个结点
		newList->next->next = tmp;        //恢复newList中后续结点的指针
        ListTraverse(newList);
	}

	//
	//原头结点应该释放掉，并返回新头结点的指针
	//
	free(list);
	return newList;

}

int main()
{
    LinkList L;
    Status i;
    int j,k,pos,value;
    char opp;
    ElemType e;

    i=InitList(&L);
    printf("链表L初始化完毕，ListLength(L)=%d\n",ListLength(L));

    printf("\n1.整表创建（头插法） \n2.整表创建（尾插法） \n3.遍历操作 \n4.插入操作");
    printf("\n5.删除操作 \n6.获取结点数据 \n7.查找某个数是否在链表中 \n8.置空链表");
    printf("\n9.链表反转逆序");
    printf("\n0.退出 \n请选择你的操作：\n");
    while(opp != '0'){
        scanf("%c",&opp);
        switch(opp){
            case '1':
                CreateListHead(&L,10);
                printf("整体创建L的元素(头插法)：\n");
                ListTraverse(L);
                printf("\n");
                break;

            case '2':
                CreateListTail(&L,10);
                printf("整体创建L的元素(尾插法)：\n");
                ListTraverse(L);
                printf("\n");
                break;

            case '3':
                ListTraverse(L);
                printf("\n");
                break;

            case '4':
                printf("要在第几个位置插入元素？");
                scanf("%d",&pos);
                printf("插入的元素值是多少？");
                scanf("%d",&value);
                ListInsert(&L,pos,value);
                ListTraverse(L);
                printf("\n");
                break;

            case '5':
                printf("要删除第几个元素？");
                scanf("%d",&pos);
                ListDelete(&L,pos,&e);
                printf("删除第%d个元素成功，现在链表为：\n", pos);
                ListTraverse(L);
                printf("\n");
                break;

            case '6':
                printf("你需要获取第几个元素？");
                scanf("%d",&pos);
                GetElem(L,pos,&e);
                printf("第%d个元素的值为：%d\n", pos, e);
                printf("\n");
                break;

            case '7':
                printf("输入你需要查找的数：");
                scanf("%d",&pos);
                k=LocateElem(L,pos);
                if(k)
                    printf("第%d个元素的值为%d\n",k,pos);
                else
                    printf("没有值为%d的元素\n",pos);
                printf("\n");
                break;

            case '8':
                i=ClearList(&L);
                printf("\n清空L后：ListLength(L)=%d\n",ListLength(L));
                ListTraverse(L);
                printf("\n");
                break;

            case '9':
                //ListReverse2(L);
                printf("\n反转L后\n");
                //ListTraverse(L);
                ListTraverse(ListReverse3(L));
                printf("\n");
                break;

            case '0':
                exit(0);
        }
    }

}
