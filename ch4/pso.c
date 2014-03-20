/*************************************/
/*     입자 군집 최적화법 프로그램   */
/*          pso.c                    */
/*                                   */
/*************************************/

#include <stdio.h>
#include <stdlib.h>

/*   기호 상수                    */
#define NOPS 10 /* 입자의 개수 */
#define LIMITL 256 /* 배열 최대 영역 */
#define ILIMIT 100 /* 반복 횟수 */
#define SEED 32767 /* 난수 초기화 */
#define W 0.7 /* 관성 상수 */
#define C1 1.4 /* 로컬 질량 */
#define C2 1.4 /* 글로벌 질량 */

/*     평면 좌표를 표현하는 구조체     */
struct point {
	double x;
	double y;
};

/*     입자를 표현하는 구조체     */
struct particle {
	struct point pos; /* 위치 */
	double value; /* 평가 값 */
	struct point v; /* 속도 */
	struct point bestpos; /* 최적 위치 */
	double bestval; /* 최적 평가 값*/
};


/*   함수 프로토타입 선언   */
void initps(struct particle ps[]);
/* 입자 군집 초기화 */
void printps(struct particle ps[]);
/* 입자 군집 표시 */
double frand(); /* 실수 난수 */
double calcval(double x, double y); /* 평가 값 계산 */
void optimize(struct particle ps[]);
/* 입자 군집의 위치 갱신 */
void setgbest(struct particle ps[]);
/* 군집의 최적 위치 저장 */

/* 전역 변수 */
struct point gbestpos; /* 군집 내의 최적 위치 */
double gbestval; /* 군집 내의 최적 평가 값 */


/**********************/
/*     main() 함수    */
/**********************/
int main()
{
	struct particle ps[LIMITL]; /* 입자 군집 */
	int i; /* 반복 횟수 제어 */
    
	/* 난수 초기화 */
	srand(SEED);
    
	/* 입자 군집 초기화 */
	initps(ps);
	printps(ps);
    
	/* 최적화의 본체 */
	for (i = 0; i < ILIMIT; ++i) {
		optimize(ps);
		printf("%d회째\n",i);
		printps(ps);
	}
    
	return 0;
}

/****************************/
/*    optimize() 함수       */
/*    입자 군집의 위치 갱신 */
/****************************/
void optimize(struct particle ps[])
{
	int i;
	double r1, r2; /* 난수 저장 */
    
	for (i = 0; i < NOPS; ++i) {
		/* 난수 설정 */
		r1 = frand();
		r2 = frand();
		/* 속도 갱신 */
		ps[i].v.x = W * ps[i].v.x
        + C1 * r1 * (ps[i].bestpos.x - ps[i].pos.x)
        + C2 * r2 * (gbestpos.x - ps[i].pos.x);
        
		ps[i].v.y = W * ps[i].v.y
        + C1 * r1 * (ps[i].bestpos.y - ps[i].pos.y)
        + C2 * r2 * (gbestpos.y - ps[i].pos.y);
        
		/* 위치 갱신 */
		ps[i].pos.x += ps[i].v.x;
		ps[i].pos.y += ps[i].v.y;
        
		/* 최적 값 갱신 */
		ps[i].value = calcval(ps[i].pos.x, ps[i].pos.y);
		if (ps[i].value < ps[i].bestval) {
			ps[i].bestval = ps[i].value;
			ps[i].bestpos = ps[i].pos;
		}
	}
	/* 군집 최적 값 갱신 */
	setgbest(ps);
}


/**************************/
/*    calcbal() 함수      */
/*    평가 값 계산        */
/**************************/
double calcval(double x, double y)
{
	return x * x + y * y + 1;
}

/**************************/
/*    setgbest() 함수     */
/*    군집 최적 위치 저장 */
/**************************/
void setgbest(struct particle ps[])
{
	int i;
	double besti;
	double x, y;
    
	besti = ps[0].value;
	x = ps[0].pos.x;
	y = ps[0].pos.y;
    
	for (i = 0; i < NOPS; ++i)
    /* 현재의 최적 평가 값 탐색 */
		if (ps[i].value < besti) {
			besti = ps[i].value;
			x = ps[i].pos.x;
			y = ps[i].pos.y;
		}
    
	/* 평가 값이 과거보다 좋으면 갱신 */
	if (besti < gbestval) {
		gbestval = besti;
		gbestpos.x = x;
		gbestpos.y = y;
	}
}

/**************************/
/*    initps() 함수       */
/*    입자 군집 초기      */
/**************************/
void initps(struct particle ps[])
{
	int i;
	double x, y;
    
	for (i = 0; i < NOPS; ++i) {
		/* 위치 */
		x = ps[i].pos.x = frand() * 2 - 1.0;
		y = ps[i].pos.y = frand() * 2 - 1.0;
		/* 평가 값 */
		ps[i].value = calcval(x, y);
		/* 속도 */
		ps[i].v.x = frand() * 2 - 1.0;
		ps[i].v.y = frand() * 2 - 1.0;
		/* 최적 위치 */
		ps[i].bestpos.x = ps[i].pos.x;
		ps[i].bestpos.y = ps[i].pos.y;
		/* 최적 평가 값 */
		ps[i].bestval = ps[i].value;
	}
	/* 군집의 최적 위치 저장 */
	gbestval = ps[0].value;
	gbestpos.x = ps[0].pos.x;
	gbestpos.y = ps[0].pos.y;
	setgbest(ps);
}


/**************************/
/*    printps() 함수      */
/*    입자 군집 표시      */
/**************************/
void printps(struct particle ps[])
{
	int i;
    
	for (i = 0; i < NOPS; ++i) {
		printf("%d ", i);
		printf("%lf %lf ", ps[i].pos.x, ps[i].pos.y);
		printf("%lf ", ps[i].value);
		printf("%lf %lf ", ps[i].v.x, ps[i].v.y);
		printf("%lf %lf ",
               ps[i].bestpos.x, ps[i].bestpos.y);
		printf("%lf\n", ps[i].bestval);
	}
	printf("%lf %lf %lf\n", gbestpos.x, gbestpos.y, gbestval);
}

/*************************/
/*     frand() 함수      */
/*     실수 난수         */
/*************************/
double frand(void)
{
	double result;
	while ((result = (double)rand() / RAND_MAX) >= 1);
	return result;
}
