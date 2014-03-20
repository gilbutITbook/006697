/************************************/
/*           유전 알고리즘          */
/*             ga.c                 */
/************************************/

#include <stdio.h>
#include <stdlib.h>

/*   기호 상수                    */
#define LIMITL 256 /* 배열 최대 영역 */
#define BUFSIZE 256 /* 입력 버퍼 사이즈 */
#define DIM 5 /* 입력 수 */
#define LINES 32 /* 유전자좌의 수 */
#define POOLSIZE 30 /* 풀 사이즈 */
#define LOOPLIMIT 50 /* 반복 세대 수 */
#define SEED 32767 /* 난수의 시드 */
#define MRATE 0.03 /* 돌연변이율 */

/*   함수 프로토타입 선언   */
int initldata(int d[][DIM+1]);
/* 학습 데이터 초기화 */
int binrand(); /* 2가 난수 */
double frand(double fmax); /* 실수 난수 */
void initpool(int pool[][LINES]);
/* 유전자 풀 초기화 */
void evalpool(int pool[][LINES],
              double poolval[], int ldata[][DIM+1],
              int no_of_ldata); /* 풀의 평가 값 계산 */
double eval(int gene[],
            int ldata[][DIM+1], int no_of_ldata);
/* 유전자의 평가 값 계산 */
void printpool(int p[][LINES], double pv[]);
/* 풀의 상태 표시 */
void mating(int p[][LINES], double pv[],
            int dp[][LINES]);
/* 교차 */
void mutation(int dp[][LINES]); /* 돌연변이 */
int reverse(int val); /* 0/1 역전 */
void evaldpool(int dpool[][LINES],
               double dpoolval[], int ldata[][DIM+1],
               int no_of_ldata); /* dpool의 평가 값 계산 */
void selection(int p[][LINES], double pv[],
               int dp[][LINES], double dpv[]); /* 선택 */
double average(double pv[], int n);
/* 평균값 계산 */
int nrand(int n); /* n 미만의 정수 난수 생성 */
void singlemating(int p[][LINES],
                  double pv[], int dp[][LINES], int point,
                  double sumofpoolval); /* 교차를 1회 실행 */
void mating(int p[][LINES], double pv[],
            int dp[][LINES]); /* 교차와 증식 */
int roulette(double pv[], double sumofpoolval);
/* 룰렛 선택 */


/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	int ldata[LIMITL][DIM+1]; /* 학습 데이터 */
	int no_of_ldata; /* 학습 데이터의 개수 */
	int pool[POOLSIZE][LINES]; /* 유전자 풀 */
	double poolval[POOLSIZE]; /* 유전자의 평가 값 */
	int dpool[POOLSIZE*2][LINES]; /* 증식 중인 풀 */
	double dpoolval[POOLSIZE*2]; /* 증식 중인 평가 값 */
	int generation; /* 세대 수 */
    
	/* 난수 초기화 */
	srand(SEED);
    
	/* 학습 데이터 초기화 */
	no_of_ldata = initldata(ldata);
    
	/* 유전자 풀 초기화 */
	initpool(pool);
	evalpool(pool, poolval,
             ldata, no_of_ldata); /* 평가 값 계산 */
	printpool(pool,poolval); /* 풀의 상태 표시 */
	printf("\n");
    
	/* 유전 알고리즘 반복 */
	for (generation = 0; generation < LOOPLIMIT; ++generation)
	{
		/* 교차와 증식 */
		mating(pool, poolval, dpool);
        
		/* 돌연변이 */
		mutation(dpool);
        
		/* 선택 */
		evaldpool(dpool, dpoolval, ldata, no_of_ldata);
		selection(pool, poolval, dpool, dpoolval);
        
		/* 처리 후의 풀 상태 표시 */
		printf("%d  %lf", generation + 1, poolval[0]);
		printf(" %lf\n", average(poolval, POOLSIZE));
		printpool(pool, poolval); /*풀의 상태  표시*/
		printf("\n");
	}
    
	return 0;
}

/**********************/
/*  average() 함수    */
/*   평균값 계산      */
/**********************/
double average(double pv[], int n)
{
	int i;
	double sum = 0;
    
	/* pv[]의 평균값 계산 */
	for (i = 0; i < n; ++i)
		sum += pv[i];
    
	return sum / n;
}

/**********************/
/*  selection() 함수  */
/*      선택          */
/**********************/
void selection(int p[][LINES], double pv[],
               int dp[][LINES], double dpv[])
{
	int i, j, k;
	double maxval;
	int maxpos;
	/* p[][]에 dp[][]의 상위를 대입 */
	for (i = 0; i < POOLSIZE; ++i) {
		maxval = dpv[0];
		maxpos = 0;
		for (j = 0; j < POOLSIZE * 2; ++j) {
			if (dpv[j] > maxval) {
				maxval = dpv[j];
				maxpos = j;
			}
		}
		/* 평가 최고인 유전자를 p로 이동 */
		for (k = 0; k < LINES; ++k) p[i][k] = dp[maxpos][k];
		pv[i] = maxval;
		dpv[maxpos] = 0; /* 최저치로 수정 */
	}
}

/**********************/
/*  mutation() 함수   */
/*    돌연변이        */
/**********************/
void mutation(int dp[][LINES])
{
	int i, j;
    
	for (i = 0; i < POOLSIZE * 2; ++i)
		for (j = 0; j < LINES; ++j)
			if (frand(1.0) < MRATE)
				dp[i][j] = reverse(dp[i][j]);
}

/**********************/
/*   reverse() 함수   */
/*     0/1 역전       */
/**********************/
int reverse(int val)
{
	if (val == 0) return 1;
	return 0;
}

/**********************/
/*  roulette() 함수   */
/*  룰렛 함수         */
/**********************/
int roulette(double pv[], double sumofpoolval)
{
	double sum = 0; /* 평가 값의 부분 합 */
	int i = 0; /* 유전자 번호 */
	double limit; /* 룰렛의 적중값 */
    
	/* 룰렛의 적중값을 설정 */
	limit = frand(sumofpoolval);
	/* 룰렛을 돌림 */
	while (sum < limit)
		sum += pv[i++];
	--i; /* 1을 뺌 */
    
	return i;
}

/***********************/
/* singlemating() 함수 */
/* 교차를 1회 시행     */
/***********************/
void singlemating(int p[][LINES],
                  double pv[], int dp[][LINES], int point,
                  double sumofpoolval)
{
	int i;
	int a, b; /* 룰렛으로 선택할 부모의 번호 */
	int crosspoint; /* 교차점 */
    
	/* 룰렛을 이용해 부모를 선택 */
	a = roulette(pv, sumofpoolval);
	while ((b = roulette(pv, sumofpoolval)) == a);
    
	/* 교차점 결정 */
	crosspoint = nrand(LINES);
    
	/* 교차를 통해 유전자 작성 */
	for (i = 0; i < LINES; ++i) {
		if (i < crosspoint) {
			dp[2*point][i] = p[a][i];
			dp[2*point+1][i] = p[b][i];
		}
		else {
			dp[2*point][i] = p[b][i];
			dp[2*point+1][i] = p[a][i];
		}
	}
}

/**********************/
/*  mating() 함수     */
/*  교차와 증식       */
/**********************/
void mating(int p[][LINES], double pv[],
            int dp[][LINES])
{
	int i;
	double sumofpoolval = 0; /* 평가치의 총합 */
    
	/* 평가 값의 총합 계산 */
	for (i = 0; i < POOLSIZE; ++i)
		sumofpoolval += pv[i];
    
	for (i = 0; i < POOLSIZE; ++i) {
		/* 부모를 선택해 교차를 1회 실행 */
		singlemating(p, pv, dp, i, sumofpoolval);
	}
}

/**********************/
/*  printpool() 함수  */
/*  풀의 상태 표시    */
/**********************/
void printpool(int p[][LINES], double pv[])
{
	int i, j;
    
	for (i = 0; i < POOLSIZE; ++i) {
		for (j = 0; j < LINES; ++j)
			printf("%d ", p[i][j]);
		printf(" : %lf\n", pv[i]);
	}
}

/**************************/
/*  eval() 함수           */
/*  유전자의 평가 값 계산 */
/**************************/
double eval(int gene[],
            int ldata[][DIM+1], int no_of_ldata)
{
	int i, j;
	int pos; /* 유전자좌의 위치 */
	int n; /* 2의 멱승 */
	int hit = 0; /* 정해 수 */
    
	for (i = 0; i < no_of_ldata; ++i) {
		/* 유전자좌 결정 */
		pos = 0; n = 1;
		for (j = DIM - 1; j >= 0; --j) {
			pos += n * ldata[i][j];
			n *= 2;
		}
		/* 학습 데이터와의 비교 */
		if (ldata[i][DIM] == gene[pos]) ++hit;
	}
    
	return (double)hit / no_of_ldata;
}

/*************************/
/*  evaldpool() 함수     */
/*  dpool의 평가 값 계산 */
/*************************/
void evaldpool(int dpool[][LINES],
               double dpoolval[], int ldata[][DIM+1],
               int no_of_ldata)
{
	int i;
    
	for (i = 0; i < POOLSIZE * 2; ++i) {
		/* 각 유전자에 대해 평가 값 계산 */
		dpoolval[i] = eval(dpool[i], ldata, no_of_ldata);
	}
}

/**********************/
/*  evalpool() 함수   */
/*  풀의 평가 값 계산 */
/**********************/
void evalpool(int pool[][LINES],
              double poolval[], int ldata[][DIM+1],
              int no_of_ldata)
{
	int i;
    
	for (i = 0; i < POOLSIZE; ++i) {
		/* 각 유전자에 대해 평가 값 계산 */
		poolval[i] = eval(pool[i], ldata, no_of_ldata);
	}
}

/**********************/
/*  initpool() 함수   */
/*  유전자 풀 초기화  */
/**********************/
void initpool(int pool[][LINES])
{
	int i, j;
    
	for (i = 0; i < POOLSIZE; ++i)
		for (j = 0;j < LINES; ++j)
			pool[i][j] = binrand();
}

/**********************/
/*   frand() 함수     */
/*     실수 난수      */
/**********************/
double frand(double fmax)
{
	return (double)rand() / RAND_MAX * fmax;
}

/**************************/
/*   nrand() 함수         */
/*  n 미만 정수 난수 생성 */
/**************************/
int nrand(int n)
{
	unsigned int r;
    
	while ((r = rand()) == RAND_MAX);
	return (double)r / RAND_MAX * n;
}

/**********************/
/*  binrand() 함수    */
/*     2가 난수       */
/**********************/
int binrand()
{
	if ((double)rand() / RAND_MAX > 0.5)
		return 1;
	else
		return 0;
}

/***********************/
/*  initldata() 함수   */
/*  학습 데이터 초기화 */
/***********************/
int initldata(int d[][DIM+1])
{
	char linebuf[BUFSIZE]; /* 입력 버퍼 */
	int i = 0; /* 학습 데이터의 개수 */
    
	while (fgets(linebuf, BUFSIZE, stdin) != NULL) {
        /* 행 읽기 가능 */
		if (sscanf(linebuf, "%d %d %d %d %d %d",
                   &d[i][0], &d[i][1], &d[i][2],
                   &d[i][3], &d[i][4], &d[i][5]) <= 0)
        /* 변환할 수 없는 경우 */
			break; /* 행 읽기 중단 */
		++i;
	}
    
	return i;
}
