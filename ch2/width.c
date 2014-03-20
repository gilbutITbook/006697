/************************************/
/*      너비 우선 탐색 프로그램     */
/*      width.c                     */
/************************************/

#include <stdio.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TRUE 1     /* 참(논리 값) */
#define START 1    /* 시작 노드 번호 */
#define GOAL 999   /* 목표 노드 번호 */

/*     노드를 표현하는 구조체     */
struct node {
	int nodeid; /* 노드 번호 */
	int parentid; /* 상위 노드 번호 */
};
/* 오픈 리스트와 클로즈드 리스트 */
struct node openlist[LIMITL];
int openlistep = 0;
struct node closedlist[LIMITL];
int closedlistep = 0;

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
/* ID 중복 검사 */
int check(int id);
/* 목표 노드까지의 경로 출력 */
void printroute(int id);


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
        
		printlist();
        
	}
    
	return 0;
}
/************************************/
/*  printroute() 함수               */
/*  목표 노드에 이르는 경로를 출력  */
/************************************/
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
    /* 시작이 아니면 출력을 속행 */
		printroute(closedlist[i].parentid);
	printf("\n");
    
}

/**************************/
/*     check() 함수       */
/*     ID 중복 검사       */
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


/***********************/
/*  movetofirst() 함수 */
/*  선두 요소 이동     */
/***********************/
void movetofirst()
{
	/* 선두 요소를 클로즈드 리스트로 이동 */
	closedlist[closedlistep++] = openlist[0];
	/* 오픈 리스트 선두 요소 삭제 */
	removefirst();
}



/**********************/
/*   expand() 함수    */
/*   노드 전개        */
/**********************/
void expand(int id)
{
	/* 접속 관계 정보 */
	int tree[][5] = {
		{1, 2, 3, 5},
		{2, 4},
		{3, 6, 7},
		{5, 4, 8},
		{6, 5, 7},
		{8, 7, 999},
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
					openlist[openlistep++].parentid = id;
				}
			}
			break;
		}
		++i;
	}
    
}


/**********************/
/*  initlist() 함수   */
/*  리스트 초기화     */
/**********************/
void initlist()
{
	/* 오픈 리스트 초기화 */
	openlist[0].nodeid = START;
	openlist[0].parentid = 0;
	++openlistep;
}

/**********************/
/*  printlist() 함수  */
/* 리스트의 상태 출력 */
/**********************/
void printlist()
{
	int i;
    
	/* 오픈 리스트 출력 */
	printf("\n오픈 리스트  ");
	for (i = 0; i < openlistep; ++i)
		printf("%d[%d],", openlist[i].nodeid,
               openlist[i].parentid);
	printf("\n");
    
	/* 클로즈드 리스트 출력 */
	printf("클로즈드 리스트  ");
	for (i = 0; i < closedlistep; ++i)
		printf("%d[%d],", closedlist[i].nodeid,
               closedlist[i].parentid);
	printf("\n");
    
    
    
}
