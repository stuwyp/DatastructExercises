#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int ListData;
//双向循环链表
typedef struct dnode {
	struct dnode *prior, *next;
	ListData data;
} DblNode;
typedef DblNode *DblList;

int minus[2] = {0, 0};
int len[3];

//判断字符串是否纯数字字符串，若是转换为整数，并返回该数；否则返回-1；
int transfer(char *s) {
	int num = 0;
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (s[i] >= 48 && s[i] <= 57) {
			num = num * 10 + s[i] - 48;
			continue;
		} else
			return -1;
	}
	return num;
}

//遍历链表，isInput 表示是否输出链表数据， type表示符号类型（同号、异号）
int listTraverse(DblList first,int type) {
	int count = 0;
	DblNode *r = first;
	if (first->data == 0 && r->next == first) {
		printf("0");
		return 0;
	}
	if (r->data < 0)
		printf("-");
	while (r && r->next != first) {
		r = r->next;
		count++;

		if (type == 1) {
			if (r->next != first)
				if (count == 1)
					printf("%d,", r->data);
				else
					printf("%04d,", r->data);
			else if (count == 1)
				printf("%d", r->data);
			else
				printf("%04d", r->data);
		} else {
			if (r->next != first) {
				if (r->data == 0) {
					if (r->next == first) {
						printf("0");
						return 0;
					}
					if (count == 1) {
						count--;
						continue;
					} else printf(",%04d", r->data);
				} else {
					if (count == 1)
						printf("%d", r->data);
					else
						printf(",%04d", r->data);
				}

			} else {
				if (count == 1)
					printf("%d", r->data);
				else
					printf(",%04d", r->data);
			}
		}

	}
	return count;
}

//插入   //place == 0 前插 ;1 后插
int listInsert(DblList first, ListData d, int* len,int place) {
	DblNode *p = first->prior;
	DblNode *q = first->next;
	DblNode *r = (DblNode *) malloc(sizeof(DblNode));
	if(place) {
		if (r) {
			r->data = d;
			r->prior = p;
			r->next = p->next;
			p->next = r;
			r->next->prior = r;
			(*len)++;
		} else
			printf("分配空间失败");
	} else {
		if (r) {
			r->data = d;
			r->prior = q->prior;
			r->next = first->next;
			q->prior = r;
			r->prior->next = r;
			(*len)++;
		} else
			printf("分配空间失败");
	}
	return *len;
}

//清空链表
void clearList(DblList first,int *len) {
	DblNode *p, *q = first->next;
	while (q != first) {
		p = q;
		q = q->next;
		free(p);
	}
	first->next = first->prior = first;
	*len = 0;
}

//同号相加运算
int addLong(DblNode *first[2]) {
	int carry = 0;
	int minusAns = 0;
	int t = minus[0] == minus[1];

	if (t) {
		if (minus[0] == 0 && minus[1] == 0) minusAns = 0;
		else minusAns = 1;
		DblNode *p = first[0]->prior, *q = first[1]->prior, *r = first[2];
		while (p != first[0] && q != first[1]) {
			int tmp = (p->data + q->data + carry) % 10000;
			carry = (p->data + q->data + carry) / 10000;
			listInsert(first[2],tmp,&len[2],0);
			p = p->prior;
			q = q->prior;
		}
		while (p != first[0]) {
			int tmp = (p->data + carry) % 10000;
			carry = (p->data + carry) / 10000;
			listInsert(first[2],tmp,&len[2],0);
			p = p->prior;
		}
		while (q != first[1]) {
			int tmp = (q->data + carry) % 10000;
			carry = (q->data + carry) / 10000;
			listInsert(first[2],tmp,&len[2],0);
			q = q->prior;
		}
		first[2]->data = len[2];
		if (minusAns)
			first[2]->data*=-1;
		if (carry) {
			listInsert(first[2],1,&len[2],0);
			if (minusAns)
				first[2]->data -= 1;
			else
				first[2]->data += 1;
		}
		printf("\n 结果是:");
		listTraverse(first[2], 1);
		puts("\n");
	} else
		puts("error\n");

}

//异号相加运算
int subLong(DblNode *first[2]) {
	int carry = 0;
	int count = 0;
	int end = 0;
	int lastEqual = 0;
	int lenCompare = 0;
	int minusAns = 0;
	int t = minus[0] == minus[1];
	if (!t) {
		if (len[0] > len[1])
			lenCompare = 1;
		else if (len[0] == len[1])
			lenCompare = 0;
		else
			lenCompare = -1;

		DblNode *p = first[0]->prior, *q = first[1]->prior, *r = first[2];
		DblNode *pp = first[0]->next, *qq = first[1]->next;

		while (p != first[0] && q != first[1]) {
			count++;
			if (lenCompare == 1) {
				if (p->data < q->data) {
					int tmp = p->data +carry + 10000 - q->data;
					listInsert(first[2],tmp,&len[2],0);
					carry = -1;
				} else {
					int tmp = p->data+carry - q->data;
					listInsert(first[2],tmp,&len[2],0);
					carry = 0;
				}
				q = q->prior;
				p = p->prior;
			} else if (lenCompare == -1) {
				if (q->data < p->data) {
					int tmp = q->data+carry + 10000 - p->data;
					listInsert(first[2],tmp,&len[2],0);
					carry = -1;
				} else {
					int tmp = q->data+carry - p->data;
					listInsert(first[2],tmp,&len[2],0);
					carry = 0;
				}
				p = p->prior;
				q = q->prior;
			} else {
				if (pp != first[0] || qq != first[1]) {

					if (pp->data > qq->data) {
						lenCompare = 1;
						count--;
					} else if (pp->data == qq->data) {
						if (pp->next == first[0] || qq->next == first[1]) {
							end = 1;
							break;
						}
						pp = pp->next;
						qq = qq->next;
						count--;
					} else {
						lenCompare = -1;
						count--;
					}
				}
			}
		}
		if(!end) {
			while (p != first[0]) {
				if (p->prior != first[0]) {
					int tmp;
					if(p->data+carry < 0)
						tmp = p->data+carry +10000;
					else
						tmp = p->data+carry;
					listInsert(first[2],tmp,&len[2],0);
					carry =-1;
				} else {
					int tmp = p->data+carry;
					listInsert(first[2],tmp,&len[2],0);
				}
				p = p->prior;
			}
			while (q != first[1]) {
				if (q->prior != first[1]) {
					int tmp;
					if(q->data+carry < 0)
						tmp = q->data+carry +10000;
					else
						tmp = q->data+carry;
					listInsert(first[2],tmp,&len[2],0);
					carry = -1;
				} else {
					int tmp = q->data+carry;
					listInsert(first[2],tmp,&len[2],0);
				}
				q = q->prior;
			}
		}
		if(lenCompare == 1)
			minusAns = first[0]->data < 0;
		else if(lenCompare == -1)
			minusAns = first[1]->data < 0;
		first[2]->data = len[2];
		if (minusAns)
			first[2]->data*=-1;
		printf("\n 结果是:");
		listTraverse(first[2], -1);
		puts("\n");
	} else
		printf(" error\n");
}

//总运算
int operation() {
	int success, count, end = 0;        //    int valid = 0;  int char lastCh;
	char a[7];
	DblNode *first[3];

	for (int i = 0; i < 3; i++) {
		first[i] = (DblNode *) malloc(sizeof(DblNode));
		if (!first[i]) {
			printf("存储分配错误!\n");
			exit(1);
		} else
			first[i]->prior = first[i]->next = first[i];
		len[i] = 0;
	}

	printf(" 本程序实现两个任意长整数的求和运算\n");
	printf("\n 请依次输入两个长整数\n");
	printf("\n 输入形式:每四位一组，组间用逗号隔开(如1,0001,0001)\n");

	for (int j = 0; j < 2; j++) {
		do {
			fflush(stdin);
			minus[j] = 0;
			count = 0;
			success = 1;
			printf("\n 请输入第%d个:", j + 1);
			do {
				count++;
				if (count == 1) {
					for (int i = 0; i < 6 - minus[0]; i++) {
						a[i] = (char) getchar();
						if (i == 0 && a[i] == '-' && minus[j] == 0) {
							minus[j] = 1;
							i--;
						} else if (a[i] == '\n') {
							end = 1;
							a[i] = '\0';
							break;
						} else if (a[i] == ',') {
							end = 0;
							a[i] = '\0';
							break;
						}
					}
					if (success) {
						int data = transfer(a);
						if (data != -1 && data < 10000)
							listInsert(first[j], data,&len[j],1);
						else {
							end = 1;
							success = 0;
						}
					}
				} else {
					for (int i = 0; i < 5; i++) {
						a[i] = (char) getchar();
						if (a[i] == '\n') {
							if (i == 4) {
								end = 1;
								a[i] = '\0';
								break;
							} else {
								success = 0;
								end = 1;
								break;
							}
						} else if (i == 4 && a[i] == ',') {
							end = 0;
							a[i] = '\0';
							break;
						}
					}
					if (success) {
						int data = transfer(a);
						if (data != -1)
							listInsert(first[j], data,&len[j],1);
						else {
							end = 1;
							success = 0;
						}
					}
				}
			} while (!end);

			if (!success) {
				puts("\n 输入有误，请重新输入");
				clearList(first[j],&len[j]);
			}
		} while (!success);
	}
	for (int i = 0; i < 2; i++)
		first[i]->data = (1 - 2 * minus[i]) * len[i];

	first[2]->data = 0;
	if (minus[0] == minus[1])
		addLong(first);
	else
		subLong(first);

	free(first[0]);
	free(first[1]);
}

int main() {
	while(1)
		operation();
}



