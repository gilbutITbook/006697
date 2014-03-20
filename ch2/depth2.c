/************************************/
/*         깊이 우선 탐색 프로그램  */
/*         depth.c                  */
/************************************/

#include <stdio.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TRUE 1 /* 참(논리 값) */
#define START 1 /* 시작 노드 번호 */
#define GOAL 999 /* 목표 노드 번호 */

/*    노드를 표현하는 구조체    */
struct node{
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
/* 리스트 상태 출력 */
void printlist();
/* 노드 전개 */
void expand(int id);
/* 오픈 리스트의 선두 요소 삭제 */
void removefirst();
/* ID 중복 검사 */
int check(int id);
/* 목표 노드까지의 경로 출력 */
void printroute(int id);
/* 오픈 리스트에서 중복 제거 */
void checkopenlist(int id);
/* 오픈 리스트의 선두에 빈 요소 추가 */
void addopenlist();


/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	struct node currentnode;
    
	/* 초기화 */
	initlist();
	printlist();
	/* 탐색의 본체 */
	while (TRUE) {
		if (openlistep == 0) {
			/* 목표 노드를 찾을 수 없음 */
			printf("목표 노드는 발견되지 않았습니다.\n");
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
		/* 오픈 리스트의 선두 요소를 꺼냄 */
		currentnode = openlist[0];
		removefirst(); /* 오픈 리스트의 선두에서 삭제 */
		/* 오픈 리스트의 선두 요소를 전개 */
		expand(currentnode.nodeid);
        
		/* 클로즈드 리스트로 이동 */
		closedlist[closedlistep++] = currentnode;
        
		printlist();
        
	}
    
	return 0;
}

/********************************/
/*    printroute() 함수         */
/*    목표 노드까지의 경로 출력 */
/********************************/
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
	/* 깊이 우선 탐색에서는 불필요 */
    /*	for (i = 0; i < openlistep; ++i)
     if (openlist[i].nodeid == id)
     res = TRUE; */
	/* 클로즈드 리스트와의 중복 검사 */
	for (i = 0; i < closedlistep; ++i)
		if (closedlist[i].nodeid == id)
			res = TRUE;
	return res;
}

/***************************/
/* checkopenlist() 함수    */
/* 오픈 리스트의 중복 삭제 */
/***************************/
void checkopenlist(int id)
{
	int i, j;
    
	for (i = 1; i < openlistep; ++i)
		if (openlist[i].nodeid == id) {
			for (j = i; j < openlistep; ++j)
				openlist[j] = openlist[j+1];
			--openlistep;
			--i;
		}
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
/*    expand() 함수   */
/*    노드 전개       */
/**********************/
void expand(int id)
{
    /* 접속 관계 정보 */
    int tree[][5] = {
        {1, 2, 3, 4},
        {2, 10, 11},
        {3, 12, 13, 14},
        {4, 15, 16, 17},
        {11, 21},
        {12, 22},
        {14, 23, 24},
        {15, 4, 10, 999},
        {16, 25},
        {17, 26, 27},
        {0} /* 정보의 끝 */
    };
	int i = 0;
	int j;
    
	while (tree[i][0] != 0) {
		if (tree[i][0] == id) {
			/* 전개 */
			for (j = 1; tree[i][j] != 0; ++j) {
				if (check(tree[i][j]) != TRUE) { /* 중복 검사 */
					/* 오픈 리스트의 선두에 빈 요소 추가 */
					addopenlist();
					/* 오픈 리스트의 선두에 추가 */
					openlist[0].nodeid = tree[i][j];
					openlist[0].parentid = id;
				}
				/* 오픈 리스트에 중복이 있으면 삭제 */
				/* (깊이 우선 탐색의 고유한 처리) */
				checkopenlist(tree[i][j]);
			}
			break;
		}
		++i;
	}
    
}

/*************************************/
/*   addopenlist() 함수              */
/* 오픈 리스트의 선두에 빈 요소 추가 */
/*************************************/
void addopenlist()
{
	int i;
	for (i = openlistep; i > 0; --i)
		openlist[i] = openlist[i-1];
	++openlistep;
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
		printf("%d[%d],", openlist[i].nodeid,
               openlist[i].parentid);
	printf("\n");
    
	/* 클로즈드 리스트 출력 */
	printf("클로즈드 리스트 ");
	for (i = 0; i < closedlistep; ++i)
		printf("%d[%d],", closedlist[i].nodeid,
               closedlist[i].parentid);
	printf("\n");
}
