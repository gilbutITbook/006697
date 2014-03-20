/************************************/
/*       n-gram 작성                */
/*        ngram.c                   */
/************************************/

#include <stdio.h>
#define N 5 /* n-gram의 길이 n */

/* 함수 프로토타입 선언 */
void initngram(char ngram[]);
/* n-gram 초기화 */
void putngram(char chr, char ngram[]);
/* n-gram 출력 */

/**********************/
/*     main() 함수      */
/**********************/
int main()
{
	char chr; /* 입력 문자 */
	char ngram[N+1]; /* n-gram */
    
	/* n-gram 초기화 */
	initngram(ngram);
    
	/* n-gram 출력 */
	while ((chr = getchar()) != EOF) {
		putngram(chr, ngram); /* 출력 */
	}
    
	return 0;
}
/**********************/
/*  putngram() 함수   */
/*  n-gram 출력       */
/**********************/
void putngram(char chr, char ngram[])
{
	int i;
    
	for (i = 0; i < N - 1; ++i)
		ngram[i] = ngram[i+1];
	ngram[N-1] = chr;
    
	printf("%s\n", ngram);
}

/**********************/
/*  initngram() 함수  */
/*  n-gram 초기화     */
/**********************/
void initngram(char ngram[])
{
	int i;
    
	for (i = 0; i < N; ++i)
		ngram[i] = ' ';
    
	ngram[N] = '\0'; /* 문자열의 끝 */
}
