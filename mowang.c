//
// Created by wyp on 2018/4/30.
//

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
#define STACK_OVERFLOW -1
#define QUEUE_MAXSIZE 100
#define ERROR -1
#define OK 0

typedef char StackData;
typedef char QueueData;
typedef struct {
    StackData *base;
    StackData *top;
    int stackSize;
} stack;

typedef struct {
    int rear;
    int front;
    QueueData *data;
} queue;


int stackEmpty(stack S) {
    //判栈空
    return (S.top == S.base);
}

int stackFull(stack S) {
    //判栈满
    return (S.top - S.base == S.stackSize);
    //满则返回1,否则返回0
}


int initStack(stack *S) {
    S->base = (StackData *) malloc(STACK_INIT_SIZE * sizeof(StackData));
    if (!S->base) return (STACK_OVERFLOW);
    S->top = S->base;
    S->stackSize = STACK_INIT_SIZE;
    return (OK);
}

int push(stack *S, StackData d) {
    if (stackFull(*S)) {
        S->base = (StackData *) realloc(S->base, (STACK_INIT_SIZE + STACK_INCREMENT) * sizeof(StackData));
        if (!S->base) return (STACK_OVERFLOW);
        S->top = S->base + S->stackSize;
        S->stackSize += STACK_INCREMENT;
    }
    *(S->top++) = d;
    return OK;
}

int pop(stack *S, StackData *d) {
    if (stackEmpty(*S))
        return ERROR;
    *d = *(--S->top);
    return OK;
}

int getTop(stack *S, StackData *d) {
    if (stackEmpty(*S))
        return ERROR;
    S->top--;
    *d = *(S->top++);
    return OK;
}

int queueEmpty(queue Q) {
    return Q.rear == Q.front;
}

int queueFull(queue Q) {
    return (Q.rear + 1) % QUEUE_MAXSIZE == Q.front;
}

int initQueue(queue *Q) {
    Q->data = (QueueData *) malloc(QUEUE_MAXSIZE * sizeof(QueueData));
    if (!Q->data)
        return -1;
    Q->rear = Q->front = 0;
    return OK;
}

int enQueue(queue *Q, QueueData d) {
    if (queueFull(*Q))
        return -1;
    Q->data[Q->rear] = d;
    Q->rear = (Q->rear + 1) % QUEUE_MAXSIZE;
    return OK;
}

int deQueue(queue *Q, QueueData *d) {
    if (queueEmpty(*Q))
        return -1;
    *d = Q->data[Q->front];
    Q->front = (Q->front + 1) % QUEUE_MAXSIZE;
    return OK;
}

int getFront(queue *Q, QueueData *d) {
    if (queueEmpty(*Q))
        return -1;
    *d = Q->data[Q->front];
    return OK;
}


void pushInStack(char *arr, stack *S) {
    int len = strlen(arr);
    for (int i = len - 1; i >= 0; i--)
        push(S, arr[i]);
}


void printA(char *A) {
    int lenA = strlen(A);
    for(int i = 0;i< lenA;i++){
        switch (A[i]){
            case 't':
                printf("天");
                break;
            case 'd':
                printf("地");
                break;
            case 's':
                printf("上");
                break;
            case 'a':
                printf("一个");
                break;
            case 'e':
                printf("鹅");
                break;
            case 'z':
                printf("追");
                break;
            case 'g':
                printf("赶");
                break;
            case 'x':
                printf("下");
                break;
            case 'n':
                printf("蛋");
                break;
            case 'i':
                printf("恨");
                break;
        }
    }
}
void printB(char *B,char *A) {
    int lenB = strlen(B);
    for(int i = 0;i< lenB;i++){
        switch (B[i]){
            case 'A':
                printA(A);
                break;
            case 't':
                printf("天");
                break;
            case 'd':
                printf("地");
                break;
            case 's':
                printf("上");
                break;
            case 'a':
                printf("一个");
                break;
            case 'e':
                printf("鹅");
                break;
            case 'z':
                printf("追");
                break;
            case 'g':
                printf("赶");
                break;
            case 'x':
                printf("下");
                break;
            case 'n':
                printf("蛋");
                break;
            case 'i':
                printf("恨");
                break;
        }
    }
}
int mowang() {
    char A[] = "sae";
    char B[] = "tAdA";
    char arr[100] = "\0";
    char ch;
    char firstCh;
    int queueStatus = 0; //1: 正在入队；0 : 队列无状态； -1：正在出队
    int error = 0;
    int existFirstCh = 0;
    stack S, S2;
    queue Q;
    initStack(&S);
    initStack(&S2);
    initQueue(&Q);

    printf("\n  请输入要翻译的魔王语言 : ");
    gets(arr);
    pushInStack(arr, &S);
    printf("  ");
    while (!stackEmpty(S)) {
        pop(&S, &ch);
        if (queueStatus == 0) {
            if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
                push(&S2, ch);
            } else if (ch == '(') {
                queueStatus = 1;
                push(&S2, '.');
                enQueue(&Q, ch);
                continue;
            } else {
                error = 1;
                printf("\n  error");
                break;
            }
        } else if (queueStatus == 1) {
            if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
                enQueue(&Q, ch);
            } else if (ch == ')') {
                queueStatus = 0;
                enQueue(&Q, ch);
                continue;
            } else {
                error = 1;
                printf("\n  error");
                break;
            }
        }
    }
    existFirstCh = 0;
    int toPush = 1;
    int beginQueue = 0;

    while (!stackEmpty(S2)) {
        pop(&S2, &ch);
        if(ch == '.' ) {
            beginQueue = 1;
            toPush = 0;
        }
        if (toPush) {
            push(&S, ch);
        } else {
            while(beginQueue && !queueEmpty(Q)) {

                deQueue(&Q, &ch);
                if (ch == '(') {
//                    queueStatus = -1;
                    continue;
                } else if (ch == ')') {
//                    queueStatus = 0;
                    beginQueue = 0;
                    existFirstCh = 0;
                    toPush = 1;
                    continue;
                } else {
                    if (existFirstCh == 0) {
                        firstCh = ch;
                        existFirstCh = 1;
                    } else {
                        push(&S,ch);
                        push(&S,firstCh);
                    }
                }
            }
        }
    }

//	while (!stackEmpty(S)){
//		pop(&S, &ch);
//		printf("%c",ch);
//	}
//	puts("");

    while (!stackEmpty(S)) {
        pop(&S,&ch);
        switch (ch) {
            case 'A':
                printA(A);
                break;
            case 'B':
                printB(B,A);
                break;
            case 't':
                printf("天");
                break;
            case 'd':
                printf("地");
                break;
            case 's':
                printf("上");
                break;
            case 'a':
                printf("一个");
                break;
            case 'e':
                printf("鹅");
                break;
            case 'z':
                printf("追");
                break;
            case 'g':
                printf("赶");
                break;
            case 'x':
                printf("下");
                break;
            case 'n':
                printf("蛋");
                break;
            case 'i':
                printf("恨");
                break;
        }

    }
}



int main() {
    while(1)
		mowang();
    return 0;
}
