/************************************/
/*         최상 우선 탐색 프로그램  */
/*         best.c                   */
/************************************/

#include <stdio.h>
#include <stdlib.h> /* qsort */
/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TRUE 1 /* 참(논리 값) */
#define START 1 /* 시작 노드 번호 */
#define GOAL 999 /* 목표 노드 번호 */

/*     노드를 표현하는 구조체      */
struct node {
	int nodeid; /* 노드 번호 */
	int parentid; /* 상위 노드 번호 */
	double value; /* 평가의 지정 값 */
};
/* 오픈 리스트와 클로즈드 리스트 */
struct node openlist[LIMITL];
int openlistep = 0;
struct node closedlist[LIMITL];
int closedlistep = 0;
/* 각 노드의 평가 추정 값 */
double h[] = {0, 5.7, 4.5, 2.8, 4.1, 2, 4, 4, 0};

/*   함수 프로토타입 선언   */
/* 리스트 초기화 */
void initlist();
/* 리스트의 상태 출력 */
void printlist();
/* 노드 전개 */
void expand(int id);
/* 선두 요소를 클로즈드 리스트로 이동 */
void movetofirst();
/* 오픈 리스트의 선두 요소 삭제 */
void removefirst();
/* ID 중복 확인 */
int check(int id);
/* 목표 노드까지의 경로 출력 */
void printroute(int id);
/* 오픈 리스트의 오름차순 정렬 */
void sortopenlist();
/* 서브 함수 - 노드 비교 함수 */
int cmp(const void *a, const void *b);


/**********************/
/*     main() 함수    */
/**********************/
int main()
{
    
	/* 초기화 */
	initlist();
	printlist();
	/* 탐색의 본체 */
	while (TRUE) {
		if (openlistep == 0) {
			/* 목표 노드를 발견하지 못함 */
			printf("목표 노드를 찾을 수 없습니다.\n");
			break;
		}
		/* 오픈 리스트의 선두 요소가 목표 노드면 종료 */
		if (openlist[0].nodeid == GOAL) {
			printf("\n목표 노드를 찾았습니다.\n");
			printf("%d[%d]", openlist[0].nodeid,
                   openlist[0].parentid);
			printroute(openlist[0].parentid);
			break;
		}
		/* 오픈 리스트의 선두 요소를 전개 */
		expand(openlist[0].nodeid);
        
		/* 선두 요소를 클로즈드 리스트로 이동 */
		movetofirst(); /* 클로즈드 리스트로 이동 */
        
		/* 오픈 리스트를 오름차순으로 정렬 */
		sortopenlist();
        
		printlist();
        
	}
    
	return 0;
}
/********************************/
/*  sortopenlist() 함수         */
/*  오픈 리스트의 오름차순 정렬 */
/********************************/
void sortopenlist()
{
	qsort(openlist, openlistep, sizeof(struct node), cmp);
}
/* 서브 함수 - 노드 비교 함수 */
int cmp(const void *a, const void *b)
{
	struct node *x, *y;
	x = (struct node *)a;
	y = (struct node *)b;
	if ((x->value) < (y->value)) return -1;
	else if ((x->value) > (y->value)) return 1;
	else return 0;
}

/*********************************/
/*     printroute() 함수         */
/*     목표 노드까지의 경로 출력 */
/*********************************/
void printroute(int id)
{
	int i;
	/* 노드 번호가 id인 노드를 찾음 */
	for (i = 0; i < closedlistep; ++i)
		if (closedlist[i].nodeid == id) {
			printf("<-%d[%d]", closedlist[i].nodeid,
                   closedlist[i].parentid);
			break;
		}
	/* 다음 노드 출력 */
	if (closedlist[i].parentid != 0)
    /* 시작 노드가 아니면 출력을 속행 */
		printroute(closedlist[i].parentid);
	printf("\n");
    
}

/**************************/
/*      check() 함수      */
/*      ID 중복 검사      */
/**************************/
int check(int id)
{
	int i;
	int res = 0;
    
	/* 오픈 리스트와의 중복 검사 */
	for (i = 0; i < openlistep; ++i)
		if (openlist[i].nodeid == id)
            res = TRUE;
	/* 클로즈드 리스트와의 중복 검사 */
	for (i = 0; i < closedlistep; ++i)
		if (closedlist[i].nodeid == id)
			res = TRUE;
	return res;
}


/**********************************/
/*   removefirst() 함수           */
/*   오픈 리스트의 선두 요소 삭제 */
/**********************************/
void removefirst()
{
	int i;
	for (i = 0; i < openlistep; ++i)
		openlist[i] = openlist[i+1];
	--openlistep;
}


/**********************/
/*   movefirst() 함수 */
/*   선두 요소 이동   */
/**********************/
void movetofirst()
{
	/* 선두 요소를 클로즈드 리스트로 이동 */
	closedlist[closedlistep++] = openlist[0];
	/* 오픈 리스트의 선두 요소 삭제 */
	removefirst();
}



/**********************/
/*    expand() 함수   */
/*    노드 전개       */
/**********************/
void expand(int id)
{
	/* 접속 관계 정보 */
	int tree[][5] = {
		{1, 2, 4},
		{2, 3, 6},
		{3, 5, 999},
		{4, 3},
		{5, 7, 999},
		{0} /* 정보의 끝 */
	};
	int i = 0;
	int j;
    
	while (tree[i][0] != 0) {
		if (tree[i][0] == id) {
			/* 전개 */
			for (j = 1; tree[i][j] != 0; ++j) {
				if (check(tree[i][j]) != TRUE) { /* 중복 검사 */
					openlist[openlistep].nodeid = tree[i][j];
					openlist[openlistep].parentid = id;
					openlist[openlistep++].value = h[tree[i][j]];
				}
			}
			break;
		}
		++i;
	}
    
}


/**********************/
/*   initlist() 함수  */
/*   리스트 초기화    */
/**********************/
void initlist()
{
	/* 오픈 리스트 초기화 */
	openlist[0].nodeid = START;
	openlist[0].parentid = 0;
	++openlistep;
}

/***********************/
/*  printlist() 함수   */
/*  리스트의 상태 출력 */
/***********************/
void printlist()
{
	int i;
    
	/* 오픈 리스트 출력 */
	printf("\n오픈 리스트  ");
	for (i = 0; i < openlistep; ++i)
		printf("%d[%d,%.1lf],", openlist[i].nodeid,
               openlist[i].parentid,
               openlist[i].value);
	printf("\n");
    
	/* 클로즈드 리스트 출력 */
	printf("클로즈드 리스트 ");
	for (i = 0; i < closedlistep; ++i)
		printf("%d[%d],", closedlist[i].nodeid,
               closedlist[i].parentid);
	printf("\n");
    
    
    
}
