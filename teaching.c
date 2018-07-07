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
	int indegree;   //顶点入度
	int flag;
} VexType;

typedef struct {
	VexType vexs[VERTEX_NUM];
	int arcs[VERTEX_NUM][VERTEX_NUM];
	int vexnum, arcnum;
} MGraph;

MGraph G;

int indegree[VERTEX_NUM];

// 栈
typedef int StackData;

typedef struct {
	StackData *base;
	StackData *top;
	int stackSize;
} stack;

stack * S;

int StackEmpty(stack S) {
	//判栈空
	return (S.top == S.base);
}

int StackFull(stack S) {
	//判栈满
	return (S.top - S.base == S.stackSize);
	//满则返回1,否则返回0
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

int termsNum;      // 学期总数
int maxXuefen;     // 学分上限
int maxVexs;       //学期最大课程数
int vexsNum;       //顶点数量
int arcsNum;       //弧的数量
MGraph graph;      //计划
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
		printf("\n 请先录入课程 \n");
		return;
	}
	int j;
	int countTerm = 0; //学期计数
	int countXuefen = 0; //学分上限
	int countCourse = 0;  //课程数目
	int tempCount = 0; //这学期课程
	fp1 = fopen("data1.txt", "w+");
	if (fp1 == NULL) {
		return;
	}
	Clear(S);
	FindInDegree (G, indegree);      // 对各顶点求入度
	for (int i = 0; i < G.vexnum; i++) {
		printf("%d ", indegree[i]);
		G.vexs[i].indegree = indegree[i];
		G.vexs[i].flag = 0;
	}
	for (int i = 0; i < G.vexnum; ++i )
		// 建立零入度顶点栈 S
		if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
			//  printf("\n添加了%d\n",i);
			Push(S, i);   // 入度为 0 的顶点序号入栈
			G.vexs[i].flag = 1;   // 标志入栈
			G.vexs[i].indegree = -1;   // -1
		}
	int ave = 0;
	int count = 0;
	// InitStack (T);        // 初始化拓扑序列顶点栈 T
	while (countCourse < G.vexnum && countTerm < termsNum) {
		count = 0;
		countXuefen = 0;
		countTerm += 1;
		printf("\n  第 %d 个学期课程有：\n", countTerm);
		fprintf(fp1, "第 %d 个学期课程有：\n", countTerm );
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
				//  printf("\n添加了%d\n",i);
				Push(S, i);
				G.vexs[i].flag = 1;
				G.vexs[i].indegree = -1;
			}
		}
		tempCount = 0;
		printf("\n  总学分为：%d\n", countXuefen);
		if(count > ave)
			ave = count;
	}


	if (countCourse == G.vexnum) {
		printf("\n 编排成功！\n");
		printf("\n 平均策略：每学期平均 %d 门课，最多 %d 门\n",maxVexs,ave);
		getchar();
	} else {
		printf("\n 平均策略：每学期平均 %d 门课，未能实现...\n",maxVexs);
		printf("\n 编制失败！\n");
	}
	printf("\n 写入文件成功！\n");

	fclose(fp1);

}
void TopoSort_2(MGraph G) {
	if (G.vexnum == 0)
	{
		printf("\n 请先录入课程 \n");
		return;
	}
	int j;
	int countTerm = 0; //学期计数
	int countXuefen = 0; //学分上限
	int countCourse = 0;  //课程数目
	int tempCount = 0; //这学期课程
	fp2 = fopen("data2.txt", "w+");
	if (fp2 == NULL) {
		return;
	}
	Clear(S);
	FindInDegree (G, indegree);      // 对各顶点求入度
	for (int i = 0; i < G.vexnum; i++) {
		printf("%d ", indegree[i]);
		G.vexs[i].indegree = indegree[i];
		G.vexs[i].flag = 0;
	}
	for (int i = 0; i < G.vexnum; ++i )
		// 建立零入度顶点栈 S
		if (G.vexs[i].indegree == 0 && G.vexs[i].flag == 0) {
			//  printf("\n添加了%d\n",i);
			Push(S, i);   // 入度为 0 的顶点序号入栈
			G.vexs[i].flag = 1;   // 标志入栈
			G.vexs[i].indegree = -1;   // -1
		}
	int ave = 0;
	int count = 0;
	// InitStack (T);        // 初始化拓扑序列顶点栈 T
	while (countCourse < G.vexnum && countTerm < termsNum) {
		count = 0; 
		countXuefen = 0;
		countTerm += 1;
		printf("\n  第 %d 个学期课程有：\n", countTerm);
		fprintf(fp2, "第 %d 个学期课程有：\n", countTerm );
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
				//  printf("\n添加了%d\n",i);
				Push(S, i);
				G.vexs[i].flag = 1;
				G.vexs[i].indegree = -1;
			}
		}
		tempCount = 0;
		printf("\n  总学分为：%d\n", countXuefen);
		if(count > ave)
			ave = count;
	}


	if (countCourse == G.vexnum) {
		printf("\n 编排成功！\n");
		printf("\n 靠前策略：其中一学期最多 %d 门\n",ave);
		getchar();
	} else {
		printf("\n 编制失败！\n");
	}
	printf("\n 写入文件成功！\n");

	fclose(fp2);
}

void CreateGraph(MGraph * graph) {
	FILE * fin;
	char toRead;
	int needInput = 1;
	if (fin = fopen("courses.txt", "rt+")) {

		while (1) {
			system("cls");
			printf("\n 是否重新录入课程？\n\n Y(y) 是\n\n N(n) 否\n\n ");
			fflush(stdin);
			scanf("%c", &toRead);
			getchar();
			fflush(stdin);

			if (toRead == 'n' || toRead == 'N') {
				printf("\n 正在读取文件...\n\n");
				needInput = 0;
				break;
			}
			else if (toRead == 'y' || toRead == 'Y') {
				needInput = 1;
				break;
			}
			else {
				printf("\n 输入有误，请重输");
				fflush(stdin);
				Sleep(800);
				continue;
			}
		}
	}
	else
		needInput = 1;

	if (needInput) {

		printf("\n 请输入学期总数：\n");
		scanf("%d", &termsNum);
		printf("\n 请输入一学期学分上限：\n");
		scanf("%d", &maxXuefen);
		printf("\n 请输入课程数量：\n");
		scanf("%d", &vexsNum);
		graph->vexnum = vexsNum;
		maxVexs = vexsNum / termsNum + 1;

		printf("\n 请分别输入课程代号，课程名称及学分(先输代号，名称，再输学分): \n");
		for (int i = 0; i < vexsNum; i++) {
			scanf("%s", graph->vexs[i].data.id);
			scanf("%s", graph->vexs[i].data.name);
			scanf("%d", &graph->vexs[i].data.xuefen);
		}

		// 初始化
		for (int i = 0; i < vexsNum; i++)
			for (int j = 0; j < vexsNum; j++) {
				graph->arcs[i][j] = 0;
			}

		printf("\n 请输入先后修关系的数量（即弧数）\n");
		scanf("%d", &arcsNum);
		graph->arcnum = arcsNum;

		printf("\n 请分别输入课程代号和先修课程代号\n");
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
				printf("\n 输入有误，请重输\n\n");
			}
		}
	}
	else {
		fscanf(fin, "%d", &termsNum);
		fscanf(fin, "%d", &maxXuefen);
		fscanf(fin, "%d\n", &vexsNum);
		printf("学期总数：%d\n", termsNum);
		printf("一学期学分上限：%d\n", maxXuefen);
		printf("课程总数：%d\n", vexsNum);

		graph->vexnum = vexsNum;
		maxVexs = vexsNum / termsNum + 1;
		for (int i = 0; i < vexsNum; i++) {
			fscanf(fin, "%s", graph->vexs[i].data.id);
			fscanf(fin, "%s", graph->vexs[i].data.name);
			fscanf(fin, "%d\n", &graph->vexs[i].data.xuefen);
			printf("%s %s %d\n", graph->vexs[i].data.id, graph->vexs[i].data.name, graph->vexs[i].data.xuefen);

		}

		// 初始化
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
		printf("\n 读取文件成功...\n");
	}
	if (fin)
		fclose(fin);
}

void Menu() {
	printf("\n  教学计划编制\n");
	printf("\n  1、录入课程\n");
	printf("\n  2、学期课程平均策略\n");
	printf("\n  3、学期课程靠前策略\n");
	printf("\n  4、退出\n");
	printf("\n  请输入：\n");
}

int main() {
	int choice;
	// int flag = 1;
	S = (stack *) malloc(sizeof(stack));
	InitStack (S); // 初始化零入度顶点栈 S
	// while(flag)
	do {

		while (1) {
			system("cls");
			Menu();
			if (scanf("%d", &choice) == 1 && choice <= 4 && choice >= 1 ) {
				break;
			} else {
				printf("\n 输入有误，请重输......");
				Sleep(800);
				fflush(stdin);

			}
		}
		switch (choice) {
		case 1:
			CreateGraph(&G);
			printf("\n 按回车键返回\n");
			fflush(stdin);
			getchar();
			break;
		case 2:
			TopoSort_1(G);
			printf("\n 按回车键返回\n");
			fflush(stdin);
			getchar();
			break;
		case 3:
			TopoSort_2(G);
			printf("\n 按回车键返回\n");
			fflush(stdin);
			getchar();
			break;
		case 4:
			printf("\n 谢谢使用！");
			// flag = 1;
			break;
		}
	} while (choice != 4);  //flag == 0
	MakeEmpty(S);
	
	return 0;
}


