#include<stdio.h>
#include<stdlib.h>
typedef int ListData;

typedef struct node {       // 不带头结点
	struct node* next;
	int rank;
	ListData data;
} ListNode;

typedef  ListNode* LinkList;

//返回1 成功，返回0 失败
int ListDelete(LinkList first,  int i) {
	LinkList p,q;
	if ( first&&i == 1 ) {                      //删除表中第 1 个结点
		q = first;
		first = first->next;
	} else {
		p = first;
		int k = 1;
		while ( p && k < i-1 ) {
			p = p->next;
			k++;
		}
		if ( !p  || !(p->next) || i<1 ) {
			//找不到第i-1个结点
			printf ("无效的删除位置!\n" );
			return 0;
		} else {
			q = p->next;                     //删除中间结点或尾结点元素
			p->next = q->next;
		}
	}
	
	return 1;
}
int YSF() {
	int n = 0,m = 0;
	printf("\n 请输入n值: ");
	while(scanf("%d",&n)!=1 || n <= 1) {
		printf("\n n有误，请重新输入: ");
	}
	printf("\n 请输入初始报数上限值m: ");
	while(scanf("%d",&m)!=1 || m <= 1) {
		printf("\n m有误，请重新输入: ");
	}
	LinkList L = NULL;
	ListNode* p,* q;
	printf("\n 请依次输入%d个正整数\n",n);
	for(int i = 1; i <= n; i++) {
		ListData tmp;
		while(scanf("%d",&tmp)!=1 && tmp <= 1) {
			printf("\n s输入有误，请重新输入: ");
		}
		if(i==1) {
			L = (ListNode*)malloc(sizeof(ListNode)); 
			p = L;
			p->data = tmp;
			p->rank = i;
			p->next = NULL;
		}
		else{
			q = (ListNode*)malloc(sizeof(ListNode)); 
			q->data = tmp;
			q->rank = i;
			q->next = NULL;
			p->next = q;
			p = p->next;
		}	
	}
	p->next = L;
	int delNum = 0;
	printf("\n 开始报数......");
	p = L;
	int k = 1,rank= 0;;
	do {
		while ( p && k < m) {
			p = p->next;     //找第 m个结点
			k++;
		}
		rank = p->rank;
		printf("\n 出列序号: %d",rank);
		m = p->data;
		p = p->next;
		ListDelete(p,n-delNum);
		delNum++;
		k = 1;
	} while(delNum<n);
	free(L);
	free(q);
}
int main() {
	YSF();
}
