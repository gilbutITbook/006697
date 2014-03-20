/************************************/
/*  데이터 전처리 프로그램          */
/*           prep.c                 */
/*  입력된 수치의 증감을 조사한다.  */
/************************************/

#include <stdio.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define TRUE 1  /* 참 */
#define FALSE 0 /* 거짓 */

/*   함수 프로토타입 선언   */
void eval(double data, double olddata);
/* 값의 비교와 결과 출력 */

/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	char linebuf[LIMITL]; /* 입력 버퍼 */
	double data; /* 입력된 수치 */
	double olddata = 0.0; /* 이전 입력 값 */
	int firstdata = TRUE; /* 선두 데이터 판별 */
    
	while (fgets(linebuf, LIMITL, stdin) != NULL) {
        /* 값 취득 */
        if (sscanf(linebuf, "%lf", &data) <= 0)
            break; /* 수치로 변환할 수 없음 */
        /* 이전 값과 비교 */
        if (firstdata == TRUE) firstdata = FALSE;
        else eval(data, olddata);
        
        /* 값 갱신 */
        olddata = data;
	}
    
	return 0;
}


/****************************/
/*       eval() 함수        */
/*    값의 비교와 결과 출력 */
/****************************/
void eval(double data, double olddata)
{
	if (data >= olddata)
		printf("1\n"); /* 증가 또는 변화 없음 */
	else 
		printf("0\n"); /* 감소 */
}

