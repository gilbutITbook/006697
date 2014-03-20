/************************************/
/*      프레임 표현                 */
/*      frame.c                     */
/*                                  */
/*      A는 B를 가지고 있습니까?    */
/************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TEXTLENGTH 4096 /* 텍스트 최대 길이 */
#define DATAFILENAME "data.txt" /* 데이터 파일 */
#define YES 1
#define NO  0

/*     노드를 표현하는 구조체     */
struct node {
	char label[TEXTLENGTH]; /* 노드의 라벨 */
	char isa[TEXTLENGTH]; /* isa 링크 */
	int  isalink; /* isa 링크 접속 번호 */
	char has[TEXTLENGTH]; /* has 프레임 */
	char nothas[TEXTLENGTH]; /* not-has 프레임 */
	char does[TEXTLENGTH]; /* do 프레임 */
	char how[TEXTLENGTH]; /* how 프레임 */
};

/* 프레임 네트워크 */
struct node snet[LIMITL];
int snetpointer = 0; /* snet의 크기 저장 */

/*   함수 프로토타입 선언   */
void initnet(); /* 네트워크 초기화 */
void readdata(); /* 데이터 읽기  */
void setisalink(int ptr); /* 링크 생성 */
int searchbh(char* B, int ptr);
/* isalink 탐색과 has 슬롯 검사 */
char* fgetl(char *text, int maxline, FILE *fp);
/*  파일 읽기  */

/* 디버그용 함수 */
void printsnet();

/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	char A[TEXTLENGTH], B[TEXTLENGTH]; /* 입력 문자열 */
	int ptr;
	int result; /* 검색 결과 */
    
	/* 프레임 네트워크 초기화 */
	initnet();
    //	printsnet();
    
	/* 탐색의 본체 */
	printf("'A는 B를 가지고 있습니까?'"
           "라는 질문을 다룹니다.\n");
	printf("A와 B를 줄바꿈으로 구분하여 입력해주세요.\n");
	while (fgets(A, TEXTLENGTH, stdin) != NULL) {
		if (fgets(B, TEXTLENGTH, stdin) != NULL) {
			if ((strlen(A) <= 1) || strlen(B) <= 1)
            /* 빈 행 */ break;
			A[strlen(A)-1] = '\0';
			B[strlen(B)-1] = '\0';
		}
		printf("질문: %s은(는) %s을(를) 가지고 있습니까?\n", A, B);
		/* A를 찾음 */
		for (ptr = 0; ptr <= snetpointer; ++ptr)
			if (strcmp(snet[ptr].label, A) == 0)
				break;
		if (ptr > snetpointer) {
			printf("%s이(가) 무엇인지 이해할 수 없습니다.\n", A);
			break;
		}
		/* isa 링크를 더듬어 B를 찾음 */
		printf("  (생각 중...)\n");
		result = searchbh(B, ptr);
		printf("답변: %s은(는) %s", A, B);
		if (result == YES)
			printf("을(를) 가지고 있습니다.\n");
		else
			printf("을(를) 가지고 있지 않습니다.\n");
        
		printf("A와 B를 줄바꿈으로 구분하여 입력해주세요.\n");
	}
    
	return 0;
}

/***********************************/
/*       searchbh() 함수           */
/*    isalink 탐색과 has 슬롯 검사 */
/***********************************/
int searchbh(char* B, int ptr)
{
	printf("(%s을(를) 조사합니다)\n", snet[ptr].label);
    
	/* has 슬롯 검사 */
	if (strcmp(B, snet[ptr].has) == 0)
		return YES;
	/* not-has 슬롯 검사 */
	if (strcmp(B, snet[ptr].nothas) == 0)
		return NO;
	else if (snet[snet[ptr].isalink].isalink != -1) /* 루트가 아님 */
		return searchbh(B, snet[ptr].isalink);
	else return NO;
}

/**************************/
/*    setisalink() 함수   */
/*    링크 생성           */
/**************************/
void setisalink(int ptr)
{
	int i;
    
	for (i = 0; i <= snetpointer; ++i) {
		if ((i != ptr) && (strcmp(snet[ptr].isa
                                  , snet[i].label) == 0)) {
			snet[ptr].isalink = i;
		}
	}
}

/**************************/
/*    initnet() 함수      */
/*    네트워크 초기화     */
/**************************/
void initnet()
{
	int i;
    
	/* 데이터 읽기 */
	snet[snetpointer].isalink = -1; /* 루트 설정 */
	readdata();
	/* 링크 생성 */
	for (i = 0; i <= snetpointer; ++i) {
		setisalink(i);
	}
}

/**************************/
/*     fgetl() 함수       */
/*     파일 읽기          */
/**************************/
char* fgetl(char *text, int maxline, FILE *fp)
{
	while (fgets(text, maxline, fp) != NULL) {
		if (strlen(text) > 1) /* 빈 행이 아님 */
			return text;
	}
	return NULL;
}

/**************************/
/*    readdata() 함수     */
/*    데이터 읽기         */
/**************************/
void readdata()
{
	char text[TEXTLENGTH],
    ltext[TEXTLENGTH]; /* 입력 문자열 */
	FILE *fp; /* 파일 포인터 */
    
	/* 파일 열기 */
	if ((fp = fopen(DATAFILENAME, "r")) == NULL) {
		/* 파일 열기 실패 */
		printf("%s를 열수 없습니다.\n", DATAFILENAME);
		exit(1);
	}
	/* 파일에서 데이터 읽기 */
	while ((fgetl(text, TEXTLENGTH, fp) != NULL)
           && (fgetl(ltext, TEXTLENGTH, fp) != NULL)) {
		/* 슬롯 설정 */
		if (strncmp(text, "label", strlen("label")) == 0) {
			++snetpointer;
			strncpy(snet[snetpointer].label
                    , ltext, strlen(ltext) - 1);
		}
		else if (strncmp(text, "isa", strlen("isa")) == 0) {
			strncpy(snet[snetpointer].isa
                    , ltext, strlen(ltext) - 1);
		}
		else if (strncmp(text, "has", strlen("has")) == 0) {
			strncpy(snet[snetpointer].has
                    , ltext, strlen(ltext) - 1);
		}
		else if (strncmp(text, "nothas",
                         strlen("nothas")) == 0) {
			strncpy(snet[snetpointer].nothas
                    , ltext, strlen(ltext) - 1);
		}
		else if (strncmp(text, "does", strlen("does")) == 0) {
			strncpy(snet[snetpointer].does
                    , ltext, strlen(ltext) - 1);
		}
		else if (strncmp(text, "how", strlen("how")) == 0) {
			strncpy(snet[snetpointer].how
                    , ltext, strlen(ltext) - 1);
		}
	}
	/* 파일 닫기 */
	fclose(fp);
    
}

/**************************/
/*   printsnet() 함수     */
/*   debug function       */
/**************************/
void printsnet()
{
	int i;
	for (i = 0; i <= snetpointer; ++i) {
		printf("node %d\n", i);
		printf("label : %s\n", snet[i].label);
		printf("isa   : %s\n", snet[i].isa);
		printf("isalnk: %d\n", snet[i].isalink);
		printf("has   : %s\n", snet[i].has);
		printf("nothas: %s\n", snet[i].nothas);
		printf("does  : %s\n", snet[i].does);
		printf("how   : %s\n\n", snet[i].how);
	}
}
