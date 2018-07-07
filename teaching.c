#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#define VERTEX_NUM 100
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 50
#define STACK_OVERFLOW -1

typedef struct DataType {
	char name[40];
	char id[10];
	int xuefen;
	int term;
} DataType;

typedef struct vex {
	DataType data;
	int indegree;   //�������
	int flag;
} VexType;

typedef struct {
	VexType vexs[VERTEX_NUM];
	int arcs[VERTEX_NUM][VERTEX_NUM];
	int vexnum, arcnum;
} MGraph;

MGraph G;

int indegree[VERTEX_NUM];

// ջ
typedef int StackData;

typedef struct {
	StackData *base;
	StackData *top;
	int stackSize;
} stack;

stack * S;

int StackEmpty(stack S) {
	//��ջ��
	return (S.top == S.base);
}

int StackFull(stack S) {
	//��ջ��
	return (S.top - S.base == S.stackSize);
	//���򷵻�1,���򷵻�0
}


int InitStack(stack *S) {
	S->base = (StackData *) malloc(STACK_INIT_SIZE * sizeof(StackData));
	if (!S->base) return (STACK_OVERFLOW);
	S->top = S->base;
	S->stackSize = STACK_INIT_SIZE;
	return 0;
}

int Push(stack *S, StackData d) {
	if (StackFull(*S)) {
		S->base = (StackData *) realloc(S->base, (STACK_INIT_SIZE + STACK_INCREMENT) * sizeof(StackData));
		if (!S->base) return (STACK_OVERFLOW);
		S->top = S->base + S->stackSize;
		S->stackSize += STACK_INCREMENT;
	}
	*(S->top++) = d;
	return 0;
}

int Pop(stack *S, StackData *d) {
	if (StackEmpty(*S))
		return 1;
	*d = *(--S->top);
	return 0;
}

int GetTop(stack *S, StackData *d) {
	if (StackEmpty(*S))
		return ERROR;
	S->top--;
	*d = *(S->top++);
	return 0;
}

void PushInStack(char *arr, stack *S) {
	int len = strlen(arr);
	for (int i = len - 1; i >= 0; i--)
		Push(S, arr[i]);
}

void MakeEmpty (stack *S) {
    if(S){
    	free(S->base);
    	free(S);
	}
}


void Clear(stack *S) {
	if(S){
		S->top = S->base;
	}
}
///////////////////////////////////////////////

int termsNum;      // ѧ������
int maxXuefen;     // ѧ������
int maxVexs;       //ѧ�����γ���
int vexsNum;       //��������
int arcsNum;       //��������
MGraph graph;      //�ƻ�
FILE * fp1;
FILE * fp2;



int FirstAdjVex(MGraph *G, int v) {
	for (int w = 0 ; w < G->vexnum; w++) {
		if (G->arcs[v][w] != 0) {
			return w;
		}
	}
	return -1;
}

int NextAdjVex(MGraph *G, int v, int w) {
	for (int x = w; x < G->vexnum; x++) {
		if (G->arcs[v][x] != 0) {
			return x;
		}
	}
	return -1;
}

void FindInDegree(MGraph G, int indegree[]) {
	for (int i = 0; i < G.vexnum; i++) {
		indegree[i] = 0;
	}

	for (int i = 0; i < G.vexnum; i++) {
		for (int j = 0; j < G.vexnum; j++) {
			if (G.arcs[j][i]) {
				indegree[i]++;
			}
		}
	}
	//
	//  for(int i=0;i<G.vexnum;i++){
	//    if(indegree[i]==0){
	//      Push(S, i);
	//    }
	//  }
}

int LocateVex(MGraph *graph, char id[], int n) {
	int i;

	for (i = 0; i < n; i++)
		if (!strcmp(graph->vexs[i].data.id, id))
			break;

	if (i < n)
		return i;
	else
		return -1;
}

void TopoSort_1(MGraph G) {
	if (G.vexnum == 0)
	{
		printf("\n ����¼��γ� \n");
		return;
	}
	int j;
	int countTerm = 0; //ѧ�ڼ���
	int countXuefen = 0; //ѧ������
	int countCourse = 0;  //�γ���Ŀ
	int tempCount = 0; //��ѧ�ڿγ�
	fp1 = fopen("data1.txt", "w+");
	if (fp1 == NULL) {
		return;
	}
	Clear(S);
	FindInDegree (G, indegree);      // �Ը����������
	for (int i = 0; i < G.vexnum; i++) {
		printf("%d ", indegree[i]);
		G.vexs[i].indegree = indegree[i];
		G.vexs[i].flag = 0;
	}
	for (int i = 0; i < G.vexnum; ++i )
		// ��������ȶ���ջ S
		if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
			//  printf("\n�����%d\n",i);
			Push(S, i);   // ���Ϊ 0 �Ķ��������ջ
			G.vexs[i].flag = 1;   // ��־��ջ
			G.vexs[i].indegree = -1;   // -1
		}
	int ave = 0;
	int count = 0;
	// InitStack (T);        // ��ʼ���������ж���ջ T
	while (countCourse < G.vexnum && countTerm < termsNum) {
		count = 0;
		countXuefen = 0;
		countTerm += 1;
		printf("\n  �� %d ��ѧ�ڿγ��У�\n", countTerm);
		fprintf(fp1, "�� %d ��ѧ�ڿγ��У�\n", countTerm );
		while (!StackEmpty(*S) && tempCount < maxVexs &&
		        countXuefen < maxXuefen) {
			GetTop(S, &j);
			if (countXuefen + G.vexs[j].data.xuefen <= maxXuefen)
				Pop(S, &j);
			else
				break;
			countXuefen += G.vexs[j].data.xuefen;
			countCourse++;
			tempCount++;
			printf("    %s %s\n", G.vexs[j].data.id, G.vexs[j].data.name);
			fprintf(fp1, "%s %s\n", G.vexs[j].data.id, G.vexs[j].data.name);

			for (int k = 0; k < G.vexnum; k++) {
				if (G.arcs[j][k] && !G.vexs[k].flag)
					G.vexs[k].indegree--;
			}
			count++;
		}

		for (int i = 0; i < G.vexnum; i++) {
			if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
				//  printf("\n�����%d\n",i);
				Push(S, i);
				G.vexs[i].flag = 1;
				G.vexs[i].indegree = -1;
			}
		}
		tempCount = 0;
		printf("\n  ��ѧ��Ϊ��%d\n", countXuefen);
		if(count > ave)
			ave = count;
	}


	if (countCourse == G.vexnum) {
		printf("\n ���ųɹ���\n");
		printf("\n ƽ�����ԣ�ÿѧ��ƽ�� %d �ſΣ���� %d ��\n",maxVexs,ave);
		getchar();
	} else {
		printf("\n ƽ�����ԣ�ÿѧ��ƽ�� %d �ſΣ�δ��ʵ��...\n",maxVexs);
		printf("\n ����ʧ�ܣ�\n");
	}
	printf("\n д���ļ��ɹ���\n");

	fclose(fp1);

}
void TopoSort_2(MGraph G) {
	if (G.vexnum == 0)
	{
		printf("\n ����¼��γ� \n");
		return;
	}
	int j;
	int countTerm = 0; //ѧ�ڼ���
	int countXuefen = 0; //ѧ������
	int countCourse = 0;  //�γ���Ŀ
	int tempCount = 0; //��ѧ�ڿγ�
	fp2 = fopen("data2.txt", "w+");
	if (fp2 == NULL) {
		return;
	}
	Clear(S);
	FindInDegree (G, indegree);      // �Ը����������
	for (int i = 0; i < G.vexnum; i++) {
		printf("%d ", indegree[i]);
		G.vexs[i].indegree = indegree[i];
		G.vexs[i].flag = 0;
	}
	for (int i = 0; i < G.vexnum; ++i )
		// ��������ȶ���ջ S
		if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
			//  printf("\n�����%d\n",i);
			Push(S, i);   // ���Ϊ 0 �Ķ��������ջ
			G.vexs[i].flag = 1;   // ��־��ջ
			G.vexs[i].indegree = -1;   // -1
		}
	int ave = 0;
	int count = 0;
	// InitStack (T);        // ��ʼ���������ж���ջ T
	while (countCourse < G.vexnum && countTerm < termsNum) {
		count = 0; 
		countXuefen = 0;
		countTerm += 1;
		printf("\n  �� %d ��ѧ�ڿγ��У�\n", countTerm);
		fprintf(fp2, "�� %d ��ѧ�ڿγ��У�\n", countTerm );
		while (!StackEmpty(*S) &&
		        countXuefen < maxXuefen) {
			GetTop(S, &j);
			if (countXuefen + G.vexs[j].data.xuefen <= maxXuefen)
				Pop(S, &j);
			else
				break;
			countXuefen += G.vexs[j].data.xuefen;
			countCourse++;
			tempCount++;
			printf("    %s %s\n", G.vexs[j].data.id, G.vexs[j].data.name);
			fprintf(fp2, "%s %s\n", G.vexs[j].data.id, G.vexs[j].data.name);

			for (int k = 0; k < G.vexnum; k++) {
				if (G.arcs[j][k] && !G.vexs[k].flag)
					G.vexs[k].indegree--;
			}
			count++;
		}

		for (int i = 0; i < G.vexnum; i++) {
			if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
				//  printf("\n�����%d\n",i);
				Push(S, i);
				G.vexs[i].flag = 1;
				G.vexs[i].indegree = -1;
			}
		}
		tempCount = 0;
		printf("\n  ��ѧ��Ϊ��%d\n", countXuefen);
		if(count > ave)
			ave = count;
	}


	if (countCourse == G.vexnum) {
		printf("\n ���ųɹ���\n");
		printf("\n ��ǰ���ԣ�����һѧ����� %d ��\n",ave);
		getchar();
	} else {
		printf("\n ����ʧ�ܣ�\n");
	}
	printf("\n д���ļ��ɹ���\n");

	fclose(fp2);
}

void CreateGraph(MGraph * graph) {
	FILE * fin;
	char toRead;
	int needInput = 1;
	if (fin = fopen("courses.txt", "rt+")) {

		while (1) {
			system("cls");
			printf("\n �Ƿ�����¼��γ̣�\n\n Y(y) ��\n\n N(n) ��\n\n ");
			fflush(stdin);
			scanf("%c", &toRead);
			getchar();
			fflush(stdin);

			if (toRead == 'n' || toRead == 'N') {
				printf("\n ���ڶ�ȡ�ļ�...\n\n");
				needInput = 0;
				break;
			}
			else if (toRead == 'y' || toRead == 'Y') {
				needInput = 1;
				break;
			}
			else {
				printf("\n ��������������");
				fflush(stdin);
				Sleep(800);
				continue;
			}
		}
	}
	else
		needInput = 1;

	if (needInput) {

		printf("\n ������ѧ��������\n");
		scanf("%d", &termsNum);
		printf("\n ������һѧ��ѧ�����ޣ�\n");
		scanf("%d", &maxXuefen);
		printf("\n ������γ�������\n");
		scanf("%d", &vexsNum);
		graph->vexnum = vexsNum;
		maxVexs = vexsNum / termsNum + 1;

		printf("\n ��ֱ�����γ̴��ţ��γ����Ƽ�ѧ��(������ţ����ƣ�����ѧ��): \n");
		for (int i = 0; i < vexsNum; i++) {
			scanf("%s", graph->vexs[i].data.id);
			scanf("%s", graph->vexs[i].data.name);
			scanf("%d", &graph->vexs[i].data.xuefen);
		}

		// ��ʼ��
		for (int i = 0; i < vexsNum; i++)
			for (int j = 0; j < vexsNum; j++) {
				graph->arcs[i][j] = 0;
			}

		printf("\n �������Ⱥ��޹�ϵ����������������\n");
		scanf("%d", &arcsNum);
		graph->arcnum = arcsNum;

		printf("\n ��ֱ�����γ̴��ź����޿γ̴���\n");
		char temp1[10];
		char temp2[10];
		for (int i = 0; i < arcsNum; i++) {
			scanf("%s%s", temp1, temp2);
			int m = LocateVex(graph, temp1, arcsNum);
			int n = LocateVex(graph, temp2, arcsNum);
			printf("m: %d, n: %d\n", m, n);
			if (m >= 0 && n >= 0)
				graph->arcs[n][m] = 1;
			else {
				i--;
				printf("\n ��������������\n\n");
			}
		}
	}
	else {
		fscanf(fin, "%d", &termsNum);
		fscanf(fin, "%d", &maxXuefen);
		fscanf(fin, "%d\n", &vexsNum);
		printf("ѧ��������%d\n", termsNum);
		printf("һѧ��ѧ�����ޣ�%d\n", maxXuefen);
		printf("�γ�������%d\n", vexsNum);

		graph->vexnum = vexsNum;
		maxVexs = vexsNum / termsNum + 1;
		for (int i = 0; i < vexsNum; i++) {
			fscanf(fin, "%s", graph->vexs[i].data.id);
			fscanf(fin, "%s", graph->vexs[i].data.name);
			fscanf(fin, "%d\n", &graph->vexs[i].data.xuefen);
			printf("%s %s %d\n", graph->vexs[i].data.id, graph->vexs[i].data.name, graph->vexs[i].data.xuefen);

		}

		// ��ʼ��
		for (int i = 0; i < vexsNum; i++)
			for (int j = 0; j < vexsNum; j++) {
				graph->arcs[i][j] = 0;
			}

		fscanf(fin, "%d\n", &arcsNum);
		graph->arcnum = arcsNum;

		char temp1[10];
		char temp2[10];
		for (int i = 0; i < arcsNum; i++) {
			fscanf(fin, "%s %s\n", temp1, temp2);
			printf("%s %s\n", temp1, temp2);
			int m = LocateVex(graph, temp1, arcsNum);
			int n = LocateVex(graph, temp2, arcsNum);
			printf("m: %d, n: %d\n", m, n);
			if (m >= 0 && n >= 0)
				graph->arcs[n][m] = 1;
		}
		printf("\n ��ȡ�ļ��ɹ�...\n");
	}
	if (fin)
		fclose(fin);
}

void Menu() {
	printf("\n  ��ѧ�ƻ�����\n");
	printf("\n  1��¼��γ�\n");
	printf("\n  2��ѧ�ڿγ�ƽ������\n");
	printf("\n  3��ѧ�ڿγ̿�ǰ����\n");
	printf("\n  4���˳�\n");
	printf("\n  �����룺\n");
}

int main() {
	int choice;
	// int flag = 1;
	S = (stack *) malloc(sizeof(stack));
	InitStack (S); // ��ʼ������ȶ���ջ S
	// while(flag)
	do {

		while (1) {
			system("cls");
			Menu();
			if (scanf("%d", &choice) == 1 && choice <= 4 && choice >= 1 ) {
				break;
			} else {
				printf("\n ��������������......");
				Sleep(800);
				fflush(stdin);

			}
		}
		switch (choice) {
		case 1:
			CreateGraph(&G);
			printf("\n ���س�������\n");
			fflush(stdin);
			getchar();
			break;
		case 2:
			TopoSort_1(G);
			printf("\n ���س�������\n");
			fflush(stdin);
			getchar();
			break;
		case 3:
			TopoSort_2(G);
			printf("\n ���س�������\n");
			fflush(stdin);
			getchar();
			break;
		case 4:
			printf("\n ллʹ�ã�");
			// flag = 1;
			break;
		}
	} while (choice != 4);  //flag == 0
	MakeEmpty(S);
	
	return 0;
}


