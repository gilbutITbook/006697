/************************************/
/*        tf-idf 계산 프로그램      */
/*        tfidf.c                   */
/************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*   기호 상수                      */
#define NOFFILES 50 /* 데이터 파일 수의 상한 */
#define NOFWORDS 1000 /* 단어 수 상한 */
#define WLENGTH 32 /* 단어 길이의 상한 */
#define LENGTH 256 /* 문자열 길이의 상한 */
#define EOD -2 /* 데이터의 끝을 나타내는 기호 */

/*  단어의 속성을 표현하는 구조체        */
struct tuple {
    int freq; /* 출현 빈도 */
    char word[WLENGTH]; /* 단어 */
    double tf;
    double idf;
};

/* 함수 프로토타입 선언 */
void readfile(char *,struct tuple st[]);
/* 데이터 읽기 */
int sumfreq(struct tuple st[]);
/* 빈도 합계 계산 */
void settf(struct tuple st[], int sumoffreq);
/* settf() 함수 */
void setidf(struct tuple t[][NOFWORDS]
            , int i, int argc);
/* idf 값 계산 */
int count(char word[]
          , struct tuple t[][NOFWORDS], int argc);
/* 단어 출현 파일 수 */
void writefile(int no, struct tuple st[]);
/* 데이터 기록 */


/**********************/
/*     main() 함수    */
/**********************/
int main(int argc,char *argv[])
{
	int i;
	struct tuple t[NOFFILES][NOFWORDS];
	int sumoffreq[NOFFILES]; /* 빈도의 합계 */
    
	/* 명령행 인수 검사 */
	if (argc < 3) { /* 인수가 부족함 */
		fprintf(stderr, "사용법 tfidf "
                "(파일1) (파일2) ... \n");
		exit(1);
	}
	else if (argc > NOFFILES) { /* 파일이 너무 많음 */
		fprintf(stderr, "파일이 너무 많습니다(%d).\n "
				, argc);
		exit(1);
	}
    
    
	/* 파일에서 데이터 읽어오기 */
	for (i = 0; i < (argc - 1); ++i) {
		readfile(argv[i+1], t[i]);
	}
    
	/* 빈도 합계 계산 */
	for (i = 0; i < (argc - 1); ++i) {
		sumoffreq[i] = sumfreq(t[i]);
	}
    
	/* tf 값 계산 */
	for (i = 0; i < (argc - 1); ++i) {
		settf(t[i], sumoffreq[i]);
	}
    
	/* idf 값 계산 */
	for (i = 0; i < (argc - 1); ++i) {
		setidf(t, i, argc);
	}
    
	/* 파일에 데이터 기록하기 */
	for (i = 0; i < (argc - 1); ++i) {
		writefile(i, t[i]);
	}
    
	return 0;
}

/**********************/
/*  writefile() 함수  */
/*  데이터 기록       */
/**********************/
void writefile(int no, struct tuple st[])
{
	FILE *fp; /* 파일 포인터 */
	char filename[LENGTH]; /* 파일명 */
	int i = 0;
    
	/* 파일명은 숫자로 함 */
	sprintf(filename, "%d.txt", no);
    
	/* 파일 열기 */
	if ((fp = fopen(filename, "w")) == NULL) {
		/* 파일 열기 오류 */
		printf("%s 파일을 열 수 없습니다.\n", filename);
		exit(1);
	}
    
	/* 파일에 기록 */
	while (st[i].freq != EOD) {
		fprintf(fp, "%lf %s\n", st[i].tf*st[i].idf
                , st[i].word);
		++i;
	}
    
	/* 파일 닫기 */
	fclose(fp);
}

/**********************/
/*  setidf() 함수     */
/*  idf 값 계산       */
/**********************/
void setidf(struct tuple t[][NOFWORDS]
            , int no, int argc)
{
	int i = 0;
    
	while (t[no][i].freq != EOD) {
		t[no][i].idf = log(argc /
                           (double)count(t[no][i].word, t, argc));
		++i;
	}
}

/**********************/
/*  count() 함수      */
/*  단어 출현 파일 수 */
/**********************/
int count(char word[]
          , struct tuple t[][NOFWORDS], int argc)
{
	int cnt = 0; /* 출현 횟수 */
	int i, j;
    
	for (i = 0; i < argc - 1; ++i) {
		j = 0;
		while (t[i][j].freq != EOD) {
			if (strcmp(word, t[i][j].word) == 0) {
				++cnt;
				break;
			}
			++j;
		}
	}
	return cnt;
}


/**********************/
/*  settf() 함수      */
/*  tf 값 계산        */
/**********************/
void settf(struct tuple st[], int sumoffreq)
{
    int i = 0;
    
    while (st[i].freq != EOD) {
        st[i].tf = st[i].freq / (double)sumoffreq;
        ++i;
    }
}

/**********************/
/*  sumfreq() 함수    */
/*  빈도 합계 계산    */
/**********************/
int sumfreq(struct tuple st[])
{
	int i = 0;
	int sum = 0;
    
	while (st[i].freq != EOD) {
		sum += st[i].freq;
		++i;
	}
    
	return sum;
}

/**********************/
/*  readfile() 함수   */
/*  데이터 읽어오기   */
/**********************/
void readfile(char *filename, struct tuple st[])
{
	int i = 0;
	FILE *fp; /* 파일 포인터 */
	char buffer[LENGTH]; /* 입력 버퍼 */
    
	/* 파일 열기 */
	if ((fp = fopen(filename, "r")) == NULL) {
		/* 파일 열기 오류 */
		printf("%s 파일을 열 수 없습니다.\n", filename);
		exit(1);
	}
    
	/* 파일로부터 읽어오기 */
	while (fgets(buffer, LENGTH, fp) != NULL) {
		if (sscanf(buffer, "%d %s", &(st[i].freq)
                   , st[i].word) != 0)
        /* 변환할 수 있었음 */
            if (strlen(st[i].word) != 0)
                ++i; /* 다음 위치로 */
		if (i >= (NOFWORDS - 1)) { /* 단어 수의 상한을 넘음 */
			fprintf(stderr, "단어 수가 너무 많습니다"
                    "(%s).\n", filename);
			break; /* 읽기 중단 */
		}
	}
	/* 파일 끝 기호 */
	st[i].freq = EOD;
    
	/* 파일 닫기 */
	fclose(fp);
}
