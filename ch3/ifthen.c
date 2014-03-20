/************************************/
/*           규칙 기반 추론         */
/*           ifthen.c               */
/*                                  */
/************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TEXTLENGTH 4096 /* 텍스트 최대 길이 */
#define DATAFILENAME "ifdata.txt" /* 데이터 파일 */
#define YES 1
#define NO  0

/*     규칙을 표현하는 구조체     */
struct ifthen {
	char cond1[TEXTLENGTH]; /* 조건 1 */
	char cond2[TEXTLENGTH]; /* 조건 2 */
	char cond3[TEXTLENGTH]; /* 조건 3 */
	char res[TEXTLENGTH];   /* 결론   */
};

/* 규칙 집합 */
struct ifthen rulebase[LIMITL];
int rulepointer = 0; /* 규칙 집합의 크기 저장 */

/* 워킹 메모리 */
char wm[LIMITL][TEXTLENGTH];
int wmpointer = 0;

/*   함수 프로토타입 선언   */
void initrule(); /* 규칙 기반 초기화 */
void readdata(); /*  데이터 읽기  */
void setisalink(int ptr); /* 링크 생성 */
int searchb(char* B, int ptr); /* isalink 탐색 */
char* fgetl(char *text, int maxline, FILE *fp);
/*  파일 읽기    */
void printrule(); /* 규칙 기반 출력 */
int match(int i); /* 규칙 i와 WM의 매칭 */
int singlematch(char *singlerule);
/* 단독 규칙과 WM의 매칭 */
void putwm(); /* ＷＭ의 상태 출력 */

/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	char text[TEXTLENGTH]; /* 입력 문자열 */
	int i;
	int result; /* 검색 결과 */
	int contflag; /* 속행 플래그 */
	int check[LIMITL] = {NO}; /* 발화한 규칙의 기록 */
    
	/* 규칙 초기화 */
	initrule();
	printrule();
    
	/* 탐색의 본체 */
	printf("'Ａ，Ｂ，...의 특징을 가진 것은 무엇입니까?'"
           "라는 질문을 다룹니다.\n");
	printf("Ａ，Ｂ，...을 줄바꿈으로 구분하여 입력해주세요.\n");
	while (fgets(text, TEXTLENGTH, stdin) != NULL) {
		if (strlen(text) <= 1) break; /* 빈 행 */
		strncpy(wm[wmpointer++], text, strlen(text) - 1);
	}
	printf("아래의 특징을 가진 것을 조사합니다.\n");
	putwm();
    
	/* 규칙 기반의 검색 */
	while (1) {
		contflag = NO;
		/* 규칙을 차례대로 조사함 */
		for (i = 0; i < rulepointer; ++i) {
			/* 조건부와 일치하면 결론부를 ＷＭ에 복사 */
			if ((match(i) == YES) && (check[i] == NO)) {
				contflag = YES;
				check[i] = YES;
				strcpy(wm[wmpointer++], rulebase[i].res);
				printf("WM의 상태\n");
				putwm();
			}
		}
        
        
		/* 목표를 발견하면 종료 */
		if (strchr(wm[wmpointer-1], '<') != NULL) {
			printf(">%s을(를) 발견했습니다.\n", wm[wmpointer-1]);
			break;
		}
		else if (contflag == NO) {
			printf("목표를 발견하지 못했습니다.\n");
			break;
		}
	}
    
	return 0;
}

/**************************/
/*     putwm() 함수       */
/*     ＷＭ의 상태를 출력 */
/**************************/
void putwm()
{
	int i;
    
	for (i = 0; i < wmpointer; ++i)
		printf("  %s\n", wm[i]);
}

/**************************/
/*  singlematch() 함수    */
/*  단독 규칙과 WM의 매칭 */
/**************************/
int singlematch(char *singlerule)
{
	int i;
	int result = NO;
    
	if (singlerule[0] == '*') return YES;
    
	for (i = 0; i < wmpointer; ++i) {
		if (strcmp(singlerule, wm[i]) == 0) {
			result = YES;
			break;
		}
	}
    
	return result;
}

/**************************/
/*   match() 함수         */
/*   규칙 i와 WM의 매칭   */
/**************************/
int match(int i)
{
	if ((singlematch(rulebase[i].cond1) == YES)
        && (singlematch(rulebase[i].cond2) == YES)
        && (singlematch(rulebase[i].cond3) == YES))
		return YES; /* 일치함 */
    
	else return NO; /* 일치하지 않음 */
}

/**************************/
/*   initrule() 함수      */
/*   규칙 기반 초기화     */
/**************************/
void initrule()
{
	/* 데이터 읽기 */
	readdata();
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
/*   readdata() 함수      */
/*   데이터 읽기          */
/**************************/
void readdata()
{
	char c1text[TEXTLENGTH],
    c2text[TEXTLENGTH],
    c3text[TEXTLENGTH],
    rtext[TEXTLENGTH]; /* 입력 문자열 */
	FILE *fp; /* 파일 포인터 */
    
	/* 파일 열기 */
	if ((fp = fopen(DATAFILENAME, "r")) == NULL) {
		/* 파일 열기 실패 */
		printf("%s 파일을 열 수 없습니다.\n", DATAFILENAME);
		exit(1);
    }
	/* 파일에서 읽기 */
	while ((fgetl(c1text, TEXTLENGTH, fp) != NULL)
           && (fgetl(c2text, TEXTLENGTH, fp) != NULL)
           && (fgetl(c3text, TEXTLENGTH, fp) != NULL)
           && (fgetl(rtext, TEXTLENGTH, fp) != NULL)) {
		/* 조건 대입 */
		strncpy(rulebase[rulepointer].cond1
                , c1text, strlen(c1text) - 1);
		strncpy(rulebase[rulepointer].cond2
                , c2text, strlen(c2text) - 1);
		strncpy(rulebase[rulepointer].cond3
                , c3text, strlen(c3text) - 1);
		/* 결과 대입 */
		strncpy(rulebase[rulepointer].res
                , rtext, strlen(rtext) - 1);
		++rulepointer;
	}
	/* 파일 닫기 */
	fclose(fp);
    
}

/**************************/
/*  printrule() 함수      */
/*  debug function        */
/**************************/
void printrule()
{
	int i;
	for (i = 0; i < rulepointer; ++i) {
		printf("rule %d\n", i);
		printf("cond1 : %s\n", rulebase[i].cond1);
		printf("cond2 : %s\n", rulebase[i].cond2);
		printf("cond3 : %s\n", rulebase[i].cond3);
		printf("res   : %s\n\n", rulebase[i].res);
	}
}
