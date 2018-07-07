#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<windows.h>
#define MAXSIZE 100
typedef  struct node {
	//	char data;
	int weight;   /* 用来存放各个结点的权值*/
	int parent, LChild, RChild; /*指向双亲、 孩子结点的指针*/
} htNode, *huffmanTree;

typedef char ** huffmanCode;

int length = 0;
huffmanTree  ht = NULL;
huffmanCode  hc = NULL;
char * c = NULL;
int * w = NULL;
int n = 0;

FILE * fpht;

void selectTwo(int end, int * s1, int * s2) {
	int count = 0;
	for (int i = 1; i <= end; i++) {
		if (ht[i].parent == 0) {
			count++;
			if (count == 1) {
				*s1 = i;
			} else if (count == 2) {
				if (ht[i].weight < *s1) {
					int tmp = *s1;
					*s1 = i;
					*s2 = tmp;
				} else {
					*s2 = i;
				}
			} else {
				if (ht[i].weight < *s2)
					if (ht[i].weight < *s1) {
						int tmp = *s1;
						*s1 = i;
						*s2 = tmp;
					} else {
						*s2 = i;
					}
			}
		}
	}
}

int crtHuffmanTree() {
	if (n <= 1)
		return -1;
	int m = 2 * n - 1;

	for (int i = n + 1; i <= m; i++) {
		int s1, s2;
		selectTwo(i - 1, &s1, &s2);
		ht[s1].parent = i;
		ht[s2].parent = i;
		ht[i].LChild = s1;
		ht[i].RChild = s2;
		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}

	char  * cd = (char *)malloc(sizeof(char)*n);
	cd[n - 1] = '\0';
	int c, p, start;
	for (int i = 1; i <= n; i++) {
		start = n - 1;
		for (c = i, p = ht[i].parent; p != 0; c = p, p = ht[p].parent)

			if (ht[p].LChild == c)
				cd[--start] = '0';
			else
				cd[--start] = '1';
		hc[i] = (char *)malloc((n - start - 1) * sizeof(char));

		strcpy(hc[i], &cd[start]);
	}
	free(cd);
	return 0;
}

void initHuffman() {
	int needInit = 0;
	char ch;
	FILE *fp;
	fp = fopen("hfmTree.txt", "r+");
	if (fp == NULL) {
		needInit = 1;
	} else {

		printf("\n  存在hfmTree.txt文件，是否读取该文件(Y/N) : ");
		while (1) {
			scanf("%c", &ch);
			getchar();
			if (ch == 'n' || ch == 'N') {
				needInit = 1;
				break;
			} else if (ch == 'y' || ch == 'Y') {
				needInit = 0;
				printf("\n  正在读取哈夫曼树...\n");
				fscanf(fp, "%d\n", &n);

				if (ht == NULL)
					ht = (huffmanTree)malloc((2 * n) * sizeof(htNode));
				if (hc == NULL)
					hc = (huffmanCode)malloc((n + 1) * sizeof(char *));
				if (c == NULL)
					c = (char *)malloc(sizeof(char)*(n + 1));

				for (int i = 1; i <= n; i++)
					fscanf(fp, "%c:%d ", &c[i], &ht[i].weight);
				for (int i = 1; i <= n; i++) {
					ht[i].parent = 0;
					ht[i].LChild = 0;
					ht[i].RChild = 0;
				}
				for (int i = n + 1; i <= 2 * n - 1; i++) {
					ht[i].weight = 0;
					ht[i].parent = 0;
					ht[i].LChild = 0;
					ht[i].RChild = 0;
				}
				break;
			}
			printf("\n  输入有误，请重输\n");
			continue;
		}
	}
	if (needInit) {
		int i;
		printf("\n  请输入字符集大小 n \n");
		scanf("%d", &n);
		getchar();
		fflush(stdin);
		if (ht == NULL)
			ht = (huffmanTree)malloc((2 * n) * sizeof(htNode));
		if (hc == NULL)
			hc = (huffmanCode)malloc((n + 1) * sizeof(char *));
		if (c == NULL)
			c = (char *)malloc(sizeof(char)*(n + 1));


		printf("\n  请输入 n 个字符 \n");
		for (i = 1; i <= n; i++) {
			scanf("%c", &c[i]);
			ht[i].parent = 0;
			ht[i].LChild = 0;
			ht[i].RChild = 0;
			getchar();
		}
		fflush(stdin);
		printf("\n  请输入 n 个权值\n");
		for (i = 1; i <= n; i++) {
			scanf("%d", &ht[i].weight);
		}
		for (; i <= 2 * n - 1; ++i) {
			ht[i].weight = 0;
			ht[i].parent = 0;
			ht[i].LChild = 0;
			ht[i].RChild = 0;
		}
		printf("\n  正在建立哈夫曼树中...\n");
		getchar();
		fflush(stdin);
		fpht = fopen("hfmtree.txt", "w+");
		fprintf(fpht, "%d\n", n);
		for (int i = 1; i <= n; i++) {
			if (i != n)
				fprintf(fpht, "%c:%d ", c[i], ht[i].weight);
			else
				fprintf(fpht, "%c:%d \n", c[i], ht[i].weight);
		}
		if(fpht)
			fclose(fpht);
	}

	crtHuffmanTree();
	printf("\n  建立哈夫曼树成功...\n");
}

void printCode() {
	char ch;
	int count = 0;
	FILE * fp = fopen("CodeFile.txt", "r+");
	while (!feof(fp)) {
		count++;
		fscanf(fp, "%c", &ch);
		printf("%c", ch);
		if (count % 50 == 0)
			printf("\n");
	}
	printf("\n");
	fclose(fp);
	Sleep(1000);
}

void encoding() {
	if (ht == NULL || hc == NULL || c == NULL) {
		printf("\n  请先初始化，正在跳转至上一级...\n");
		Sleep(800);
		return;
	}
	int i = 1, j = 0;
	char string[100];
	char sentence[100];

	FILE * fp, *ffp;

	fp = fopen("tobetrans.txt", "r+");
	if (!fp) {
		printf("\n  不存在文件 tobetrans.txt\n");
		printf("\n  请输入要进行编码的句子: ");
		scanf("%s", &sentence);
		fp = fopen("tobetrans.txt", "w+");
		fprintf(fp, "%s", sentence);
		fclose(fp);
		fp = fopen("tobetrans.txt", "r+");
	}
	printf("\n  读取文件 tobetrans.txt中\n");
	fscanf(fp, "%s", &string);
	if(fp)
		fclose(fp);

	ffp = fopen("CodeFile.txt", "w+");

	while (string[j]) {

		for (i = 1; i <= n; i++)
			if (string[j] == c[i]) {
				fprintf(fp, "%s", hc[i]);
				break;
			}

		j++;
	}
	if(ffp)
		fclose(ffp);
	printf("\n  已经写入到文件 CodeFile 中\n");
	Sleep(800);
}

void	decoding() {
	if (ht == NULL || hc == NULL || c == NULL) {
		printf("\n  请先初始化，正在跳转至上一级...\n");
		Sleep(800);
		return;
	}


	FILE * fp;

	if ((fp = fopen("CodeFile.txt", "r+")) == NULL) {
		printf("\n  无 CodeFile.txt \n\n  请先编码，正在返回上一级.....\n");
		Sleep(800);
		return;
	}
	length = 0;
	char ch;
	while (!feof(fp)) {
		fscanf(fp, "%c", &ch);
		length++;
	}

	int num = 1, i = 0, j = 0;
	char  * str = (char *)malloc(sizeof(char)*length);

	rewind(fp);
	fscanf(fp, "%s", str);
	if(fp)
		fclose(fp);

	huffmanTree p;
	huffmanTree temht = ht;

	FILE * fop = fopen("textfile.txt", "w+");
	printf("\n 解码后，文件内容为：");
	while (str[j]) {
		p = &ht[2 * n - 1];
		while (p->LChild || p->RChild) {
			if (str[j] == '0') {
				i = p->LChild;
				p = &ht[i];
			} else {
				i = p->RChild;
				p = &ht[i];
			}
			j++;
		}
		printf("%c", c[i]);
		fprintf(fop, "%c", c[i]);

	}
	if(fop)
		fclose(fop);
	free(str);
	printf("\n\n 按回车键返回");
	getchar();
	fflush(stdin);
}

void printTree() {
	int depth = 0;
	if (ht) {
		int i, j;
		printf("\n\n******************它的编码规则****************\n\n");//权值相同时，按照输入的字符的顺序，从左向右排列
		for (i = 1; i <= n; i++) {
			printf("\n");
			printf("字符: %c    ", c[i]);
			printf("权值: %d    ", ht[i].weight);
			printf("对应的哈夫曼编码: ");
			puts(hc[i]);
		}
		printf("\n\n******************打印哈夫曼树****************\n\n");
		printf("  结点  weight  parent   LChild RChild");
		for (j = 1; j <= n; j++)
			printf("\n %c (%d)%5d%8d%8d%8d", c[j], j, ht[j].weight, ht[j].parent, ht[j].LChild, ht[j].RChild);
		for (int K = n + 1; K <= 2 * n - 1; K++)
			printf("\n %d %8d%8d%8d%8d", K, ht[K].weight, ht[K].parent, ht[K].LChild, ht[K].RChild);
		printf("\n\n按回车键返回");
		getchar();


	} else {
		printf("  请先初始化 \n");
		Sleep(800);
		return;
	}
}

void menu() {
	printf("\n  I. 初始化\n");
	printf("\n  C. 编码  \n");
	printf("\n  D. 译码 \n");
	printf("\n  T. 打印哈夫曼树\n");
	printf("\n  P. 打印代码文件 \n");
	printf("\n  Q. 退出系统\n\n");
}

int main() {

	char ch;
	do {
		system("cls");
		menu();
		fflush(stdin);
		scanf("%c", &ch);
		getchar();
		switch (ch) {
			case 'i':
			case 'I':
				initHuffman();
				break;
			case 'p':
			case 'P':
				printCode();
				break;
			case 'c':
			case 'C':
				encoding();
				break;
			case 'd':
			case 'D':
				decoding();
				break;
			case 't':
			case 'T':
				printTree();
				break;
			case 'q':
			case 'Q':
				break;
			default:
				printf("\n  输入有误，请重新输入");
		}
		Sleep(800);
	} while (ch != 'Q'&&ch != 'q');

	if(c)
		free(c);
	if(ht)
		free(ht);
	if (hc) {
		for (int i = 1; i <= n; i++)
			free(hc[i]);
		free(hc);
	}
	return 0;
}
