#include<stdio.h>
#include<stdlib.h>
typedef int ListData;

typedef struct node {       // ����ͷ���
	struct node* next;
	int rank;
	ListData data;
} ListNode;

typedef  ListNode* LinkList;

//����1 �ɹ�������0 ʧ��
int ListDelete(LinkList first,  int i) {
	LinkList p,q;
	if ( first&&i == 1 ) {                      //ɾ�����е� 1 �����
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
			//�Ҳ�����i-1�����
			printf ("��Ч��ɾ��λ��!\n" );
			return 0;
		} else {
			q = p->next;                     //ɾ���м����β���Ԫ��
			p->next = q->next;
		}
	}
	
	return 1;
}
int YSF() {
	int n = 0,m = 0;
	printf("\n ������nֵ: ");
	while(scanf("%d",&n)!=1 || n <= 1) {
		printf("\n n��������������: ");
	}
	printf("\n �������ʼ��������ֵm: ");
	while(scanf("%d",&m)!=1 || m <= 1) {
		printf("\n m��������������: ");
	}
	LinkList L = NULL;
	ListNode* p,* q;
	printf("\n ����������%d��������\n",n);
	for(int i = 1; i <= n; i++) {
		ListData tmp;
		while(scanf("%d",&tmp)!=1 && tmp <= 1) {
			printf("\n s������������������: ");
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
	printf("\n ��ʼ����......");
	p = L;
	int k = 1,rank= 0;;
	do {
		while ( p && k < m) {
			p = p->next;     //�ҵ� m�����
			k++;
		}
		rank = p->rank;
		printf("\n �������: %d",rank);
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
