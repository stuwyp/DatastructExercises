#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "conio.h"
#define ROWS_PER_PAGE 20
#define ACTIVEMAXLEN 100
//文学助手
#define Max_Str_Len 255 //最大串长
FILE * fin = NULL, *fout = NULL;

typedef char  SString[Max_Str_Len + 1]; //串的定长顺序存储表示
int next[Max_Str_Len];  //KMP算法中用到的next
int noFinName = 0;
int totalRowNum = 0;
int totalCharNum = 0;
int currentRow = 0;
int activeNum = 0;
int currentActive = 0;
int pageNum = 0;
int currentPage = 0;
int currentActiveLen = 0;
int activeRowBegin = 0;
int listLength = 100;
char finName[30] = "\0";
char foutName[30] = "\0";

typedef struct node
{
	//struct text * pre;
	struct node * next;
	char  string[81];
	int charNum;
} rowBlock;

typedef struct rBNode
{
	struct rBNode * next;
	int toDelete;
	rowBlock * rB;
} row;

row * activePage[5];
row * first, *locate;

int rowEdit();
void fileClose();
int deleteRows();
void deleteAToB(int A, int B);
int saveAfterDelete(int curAct);
void destroy(row * r);
int insertRow();
int freeMem(row * locate);
int copyFile();
int saveInfo(row * first, int flag);
void traversal();
void display(int message);
int changePage(int pg);
void clearRB();
int changeActive(int act);
int index_KMP(SString S, SString T, int pos);
int length(SString str);
int find(SString keys);
row * addRow(rowBlock * rB, int num);
void display(int message);
int assistant();
void initRowList();
void inputFileName();
rowBlock * saveRow(char * string);

int index_KMP(SString S, SString T, int pos)
{  //KMP算法
   //下面四个函数是文学研究助手的
	int i = pos, j = 1;
	while (i <= S[0] && j <= T[0])
	{
		if (j == 0 || S[i] == T[j])
		{
			++i;
			++j;
		}
		else
		{
			j = next[j];
		}
	}
	if (j > T[0])
	{
		return (i - T[0]);
	}
	else
		return 0;
}

int length(SString str)
{   //求字符串长度
	int i = 1;
	while (str[i])
	{
		i++;
	}
	return (i - 1);
}

int find(SString keys)
{
	//keys[0]是这个串的长度，keys[1]是这个串的内容
	SString text;   //存放从文件里读取来的一行字符串
	int i = 1, j = 0, q = 0, k; //i存放行号，j存放列号，k控制输出格式，q统计次数
	keys[0] = length(keys); //关键字的长度
	printf("\n%s出现在: \n", &keys[1]);     //打印关键字
	fin = fopen(finName, "r+");
	while (!feof(fin))
	{
		k = 0;
		fgets(&text[1], Max_Str_Len, fin);    //从文件里读取一行，存入text串里
		text[0] = length(text); //读入的串的长度
		j = index_KMP(text, keys, j + 1); //统计关键字在该行出现的位置
		if (j != 0)
		{
			printf("行=%d  列=%d", i,j);
			k++;    //次数+1
		}
		while (j != 0)
		{    //若改行找到了关键字，再往后看还有没有
			j = index_KMP(text, keys, j + 1);
			if (j != 0)
			{
				printf("  列=%d",j);
				k++;   //次数+1
			}
		}
		i++;    //行号+1，寻找下一行
		q += k; //累加k

		if (k) printf("\n");    //格式控制
	}
	printf("出现了%d次\n", q);
	rewind(fin);
	fflush(stdin);
	getchar();
	return 0;
}

int assistant()
{
	//文学研究助手
	SString words[10];  //用来存储输入的关键字
	int n, i;
	printf("\n  请输入要查询的字符串的个数: ");
	scanf("%d", &n);
	printf("\n  请输入你要查询的字符串 \n");
	for (i = 0; i < n; i++)
	{
		scanf("%s", &words[i][1]);  //[i][0]用来存字符串的长度
	}
	for (i = 0; i < n; i++)
	{
		find(words[i]); //对每一个关键字，都调用find函数来查找统计
	}
	fflush(stdin);
	printf("\n  按回车键返回\n");
	getchar();
	return 0;
}

void initRowList()
{
	int rowCount = 0;
	row * p, *q = NULL;
	while (rowCount < 80)
	{
		p = (row*)malloc(sizeof(row));
		p->rB = NULL;
		p->toDelete = 0;
		rowCount++;
		if (rowCount == 1)
		{
			first = activePage[0] = q = p;
		}
		else
		{
			q->next = p;
			q = p;
		}
	}
	q->next = NULL;
}

void inputFileName()
{
	noFinName = 0;

	while (1)
	{

		system("cls");
		printf("\n  请输入打开的文件名（绝对路径或相对路径）  ");
		scanf("%s", finName);
		/*char str[] = "f://desktop/old.txt";
		strcpy(finName, str);*/
		fflush(stdin);
		if (strlen(finName) == 0)
		{
			printf("\n  输入有误\n");
			Sleep(100);
			noFinName = 1;
			break;
		}
		else
		{
			break;
		}
	}
	printf("\n  请输入要写入的文件名（绝对路径或相对路径）  ");
	while (1)
	{
		scanf("%s", foutName);
		char str[] = "f://desktop/new.txt";
		//strcpy(foutName, str);

		if (strcmp(finName, foutName) == 0)
		{
			printf("\n  文件名相同，请检查你输入的文件名是否有误\n");
			printf("\n  请重新输入  ");
			continue;
		}
		else
			break;
	}
}

// 新增一行内容 
row * addRow(rowBlock * rB, int num)
{
	row * newRow = (row *)malloc(sizeof(row));
	if (!newRow)
		return NULL;

	newRow->rB = rB;
	newRow->next = NULL;
	row * p = first;
	if (p == NULL)
	{
		first = newRow;
	}
	else
	{
		if (num == -1)
		{
			while (p->next)
				p = p->next;
			p->next = newRow;
		}
		else if (num == 1)
		{
			newRow->next = first;
			first = activePage[0] = newRow;
		}
		else
		{
			int count = 1;
			while (p->next && ++count < num)
				p = p->next;
			newRow->next = p->next;
			p->next = newRow;
		}
	}
	return newRow;

}

// 将获取的一行文本内容分开储存到一个行块链表中
rowBlock * saveRow(char * string)
{

	int num = 0, next = 0;
	rowBlock * rB = NULL;
	int i = 0;
	while (string[next] != '\0')
	{
		rowBlock * temp = (rowBlock *)malloc(sizeof(rowBlock));
		for (i = 0; i<81; i++)
			temp->string[i] = ' ';

		while (num<80 && (temp->string[num] = string[next]) != '\0')
		{
			next++;
			num++;
		}

		temp->next = NULL;

		if (rB == NULL)
		{
			rB = temp;
		}
		else
		{
			rowBlock *p = rB;
			while (p->next)	p = p->next;
			p->next = temp;
		}
		num = 0;
	}
	return rB;
}

void display(int message)
{
	system("cls");
	printf("\n  目前是第 %d 页\n", currentPage);
	printf("\n");
	row * r = locate;
	rowBlock * rB = NULL;
	currentRow = (currentActive - 1) * 80 + (currentPage - 1) % 4 * 20;
	int count = 0;
	while (r  && count < 20 && currentRow < totalRowNum)
	{
		count++;
		printf("%4d ", ++currentRow);
		rB = r->rB;
		while (rB)
		{
			printf("%s", rB->string);
			rB = rB->next;
		}
		r = r->next;
	}
	//	getchar();
	fflush(stdin);
	printf("\n 1. 上一页    2. 下一页    3. 插入    4. 删除    5. 返回上一级\n");

	int ans;
	while (1)
	{
		scanf("%d", &ans);
		if (ans >= 1 && ans <= 5)
		{
			break;
		}
		else
		{
			printf("\n 输入有误，请重输...\n ");
			fflush(stdin);
			continue;
		}
	}
	if (ans == 1)
	{
		if (currentPage == 1)
		{
			printf("\n 本页是第一页，无上一页");
			Sleep(1500);
		}

		else
			currentPage--;
		changePage(currentPage);
		display(1);
	}
	else if (ans == 2)
	{
		if (currentPage == pageNum)
		{
			printf("\n 本页是最后一页，无下一页");
			Sleep(1500);
		}

		else
			currentPage++;
		changePage(currentPage);
		display(1);
	}
	else if (ans == 3)
	{
		int page = (insertRow() - 1) / 20 + 1;
		traversal();
		int act = (page - 1) / 4 + 1;
		changeActive(act);
		int actPage = (page - 1) % 4;
		currentPage += actPage;
		locate = activePage[actPage];
		display(0);
	}
	else if (ans == 4)
	{
		int page = (deleteRows() - 1) / 20 + 1;
		traversal();
		int act = (page - 1) / 4 + 1;
		changeActive(act);
		int actPage = (page - 1) % 4;
		currentPage += actPage;
		locate = activePage[actPage];
		display(0);
	}
}

void clearRB()
{
	row * r = first;
	rowBlock * rB;
	while (r&&r->rB)
	{
		while (r->rB)
		{
			rB = r->rB;
			r->rB = rB->next;
			free(rB);
		}
		r = r->next;
	}
}

// 切换活区
int changeActive(int act)
{
	clearRB();
	fin = fopen(finName, "r+");
	int actCount = 0;
	FILE *fp = NULL;
	int min = act * 80 - 80, max = act * 80, step = 0;
	locate = first;
	int count = 0;
	while (!feof(fin) && locate)
	{

		fflush(stdin);
		char string[320];

		if (fgets(string, 320, fin) == NULL)
			strcpy(string, "\0");

		if (step >= min && step < max)
		{
			count++;
			rowBlock* rB = saveRow(string);
			locate->rB = rB;
			if ((count - 1) % 20 == 0)
			{
				activePage[count / 20] = locate;
			}

			locate = locate->next;
		}
		else if (step >= max)
		{
			break;
		}
		step++;
	}
	currentActive = act;
	currentRow = (act - 1) * 80 + 1;
	currentPage = (currentActive - 1) * 4 + 1;
	locate = first;
	fclose(fin);

	return 0;
}

// 换页
int changePage(int pg)
{
	system("cls");

	int actOfPg = (pg - 1) / 4 + 1;//跳转页数所在活区
	if (actOfPg == currentActive)
	{

		int begin = (actOfPg - 1) * 4 + 1;
		int count = pg - begin;
		locate = activePage[count];

	}
	else if (actOfPg > currentActive)
	{
		changeActive(actOfPg);
		//		locate = first;
	}
	else
	{
		changeActive(actOfPg);
		changePage(pg);
		currentPage += 3;
	}
	return 0;
}

void traversal()
{
	totalRowNum = 0;
	fin = fopen(finName, "r+");
	//rewind(fin);
	int len;
	char string[81] = "\0";
	while (!feof(fin))
	{
		if (fgets(string, sizeof(char) * 80, fin))
		{
			len = strlen(string);
			totalCharNum += len;
			if (string[len - 1] == '\n')
				totalRowNum++;
		}


	}
	totalRowNum++;
	activeNum = (totalRowNum - 1) / 80 + 1;
	pageNum = (totalRowNum - 1) / 20 + 1;
	fclose(fin);
}

int saveInfo(row * first, int flag)
{
	int count = 0;
	FILE *fp;
	int i = 0;
	row * p = first;
	rowBlock * start;
	if (!(fp = fopen(foutName, "a+"))) printf("文件不存在！");
	while (p)
	{
		count++;
		if (flag && p->toDelete)
			p->toDelete = 0;
		else
		{

			start = p->rB;
			while (p->rB)
			{
				fprintf(fp, "%s", p->rB->string);
				p->rB = p->rB->next;
			}

			p->rB = start;
		}
		p = p->next;
	}
	fclose(fp);
	return 0;
}

int copyFile()
{

	int bufferLen = 1024 * 4; // 缓冲区长度
	char *buffer = (char*)malloc(bufferLen); // 开辟缓存
	int readCount; // 实际读取的字节数
	if ((fout = fopen(foutName, "rb")) == NULL || (fin = fopen(finName, "wb")) == NULL)
	{
		printf("Cannot open file, press any key to exit!\n");
		_getch();
		exit(1);
	}
	// 不断从fileRead读取内容，放在缓冲区，再将缓冲区的内容写入fileWrite
	while ((readCount = fread(buffer, 1, bufferLen, fout)) > 0)
	{
		fwrite(buffer, readCount, 1, fin);
	}
	free(buffer);
	fclose(fout);
	fclose(fin);
	return 0;
}

int freeMem(row * locate)
{
	row *r;
	rowBlock *rB;
	//	int len = 0;
	while (locate)
	{
		while (locate->rB)
		{
			rB = locate->rB->next;
			free(locate->rB);
			locate->rB = rB;
		}

		r = locate->next;
		free(locate);
		locate = r;
		//		len++;
		//		printf("+1 len %d\n",len);
	}
	return 0;
}

int insertRow()
{
	row * temp = NULL;
	int rowNum;
	currentRow = (currentPage - 1) * 20 + 1;
	printf("\n  请输入行数（共 %d 行)  ", totalRowNum);

	while (1)
	{
		if (scanf("%d", &rowNum) != 1)
		{
			fflush(stdin);
			continue;
		}

		if (rowNum >= currentRow && rowNum  < currentRow + 20)
		{
			FILE * temF = fopen(foutName, "w+");
			fclose(temF);
			temp = first;
			int i;
			for (i = 1; i < currentActive; i++)
			{
				changeActive(i);
				saveInfo(first, 0);
				temp = first;

			}
			changeActive(i);
			fflush(stdin);
			char input[320] = "\0";
			printf("\n 请输入该行内容（不超过320个）");
			fgets(input, 320, stdin);
			rowBlock * rB = saveRow(input);
			row * r = addRow(rB, (rowNum - 1) % 80 + 1);
			//			changeActive(currentActive);


			saveInfo(first, 0);

			for (i = currentActive + 1; i <= activeNum; i++)
			{
				changeActive(i);
				saveInfo(first, 0);
			}
			copyFile();

			int num = (rowNum - 1) % 80 + 1;

			if (num <= 20)
			{
				row * loc = activePage[0];
				int count = 0;
				while (count++ < 20)
					loc = loc->next;
				activePage[1] = loc;
			}
			if (num <= 40)
			{
				if (num == 21)
					activePage[1] = r;
				row * loc = activePage[1];
				int count = 0;
				while (count++ < 20)
					loc = loc->next;
				activePage[2] = loc;
			}
			if (num <= 60)
			{
				if (num == 41)
					activePage[2] = r;
				row * loc = activePage[2];
				int count = 0;
				while (count++ < 20)
					loc = loc->next;
				activePage[3] = loc;
			}
			else if (num == 61)
				activePage[3] = r;

			break;
		}
		else
		{
			printf("\n  请输入所在页的行数");
			continue;
		}
	}
	locate = temp;

	return rowNum;
}

void destroy(row * r)
{
	row * temp;
	rowBlock * rB;
	while (r&&r->rB)
	{
		while (r->rB)
		{
			rB = r->rB;
			r->rB = rB->next;
			free(rB);
		}
		temp = r;
		r = r->next;
		free(temp);
	}
}

int saveAfterDelete(int curAct)
{
	int i, flag;
	FILE * temF = fopen(foutName, "w+");
	fclose(temF);
	for (i = 1; i <= activeNum; i++)
	{
		changeActive(i);
		if (i == curAct)
			flag = 1;
		else
			flag = 0;
		saveInfo(first, flag);
	}
	copyFile();
	return 0;
}

void deleteAToB(int A, int B)
{

	if (A == B)
	{
		int actPage = (currentPage - 1) % 4;
		locate = activePage[actPage];
		row * r = locate;
		int step;
		if (A % 20 == 1)
			r->toDelete = 1;
		else
		{
			step = (A - 1) % 80 - actPage * 20;
			while (step--&&r)
				r = r->next;
			r->toDelete = 1;
		}
		saveAfterDelete(currentActive);
	}
	else
	{
		int actPage = (currentPage - 1) % 4;
		locate = activePage[actPage];
		row * r = locate;
		row * move;
		int step, num;
		num = B - (A - 1);
		if (A % 20 == 1)
		{
			while (num--)
			{
				r->toDelete = 1;
				r = r->next;
			}
		}
		else
		{
			step = (A - 1) % 80 - actPage * 20;

			while (step--&&r)
				r = r->next;

			move = r;
			while (move && num--)
			{
				move->toDelete = 1;
				move = move->next;
			}
		}

		saveAfterDelete(currentActive);
	}
}

int deleteRows()
{
	int rowBegin, rowEnd;
	int min = (currentPage - 1) * 20 + 1;
	int max = currentPage * 20;
	while (1)
	{

		printf("\n  请输入行号 1[(空格)行号 2] ");
		scanf("%d", &rowBegin);
		char ch = getchar();
		if (ch == '\n')
		{
			if (rowBegin< min || rowBegin>max)
			{
				printf("\n  请输入当前页码的行数\n");
				continue;
			}
			else
			{
				printf("\n  正在删除行号 %d 的行...", rowBegin);
				deleteAToB(rowBegin, rowBegin);
				break;
			}

		}
		else if (ch == ' ')
		{
			scanf("%d", &rowEnd);
			if (rowBegin <= rowEnd && rowEnd <= max && rowBegin >= min)
			{
				printf("\n  正在删除行号 %d 至 %d(包括)的一至多行...", rowBegin, rowEnd);
				deleteAToB(rowBegin, rowEnd);
				break;
			}
			else
			{
				printf("\n  请输入当前页码的行数\n");
				continue;
			}

		}
		else
		{

		}

	}
	return rowBegin;
}

//int fileOpen()
//{
//	//char foutName[30] = "\0";
//	// printf("\n  请输入要写入的文件名（绝对路径或相对路径）  ");
//	// while(1) {
//	// 	gets(foutName);
//	// 	if(strcmp(finName,foutName)==0) {
//	// 		printf("\n  文件名相同，请检查你输入的文件名是否有误\n");
//	// 		printf("\n  请重新输入  ");
//	// 		continue;
//	// 	} else if((fout = fopen(foutName,"w+"))==NULL) {
//	// 		printf("\n  文件打开失败，请检查你输入的文件名是否有误\n");
//	// 		printf("\n  请重新输入  ");
//	// 		continue;
//	// 	} else {
//	// 		break;
//	// 	}
//	// }
//	return 0;
//}
//
void fileClose()
{
	if (fin)	fclose(fin);
	if (fout) fclose(fout);
}

int rowEdit()
{

	char ch;
	if (noFinName)
	{
		printf("\n  正在开发...");
		exit(0);
	}
	else
	{
		do
		{

			fflush(stdin);
			Sleep(500);
			system("cls");
			printf("\n  活区切换			格式: c<回车>");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			printf("\n  活区显示			格式: p<回车>");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			//			printf("\n  上一页		        格式: l<回车>");
			//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			//			printf("\n  下一页		        格式: n<回车>");
			//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			//			printf("\n  行插入 	       		格式: i<回车>");
			//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			//			printf("\n  行删除  			格式: d<回车>");
			//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			printf("\n  文学研究助手   		格式: a<回车><个数><回车><内容>");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			printf("\n  保存				格式: w<回车>");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			printf("\n  退出				格式: q<回车>\n");
			printf("\n  一共有 %d  行 \n", totalRowNum);


			printf("\n  目前是第 %d 个活区，共 %d 个\n", currentActive, activeNum);
			printf("\n  目前是第 %d 页，共 %d 页\n", currentPage, pageNum);
			ch = getchar();
			fflush(stdin);
			switch (ch)
			{
				case '\n':
					break;
				case 'c':
					currentActive = currentActive % activeNum + 1;
					changeActive(currentActive);
					break;
				case 'p':
					display(0);

					break;
				case 'a':
					assistant();
					break;
				case 'w':
					printf("\n  正在保存中...");
					rewind(fin);
					locate = first;
					fileClose();

					break;
				case 'q':
					printf("\n  正在退出中...");
					Sleep(500);
					break;
				default:
					fflush(stdin);
					printf("\n  输入有误,请重新输入...");

			}
		} while (ch != 'q'&&ch != 'Q');
	}
	return 0;
}

int main()
{
	inputFileName();
	initRowList();
	traversal();
	changeActive(1);
	rowEdit();
	freeMem(first);
	fileClose();

	return 0;
}


