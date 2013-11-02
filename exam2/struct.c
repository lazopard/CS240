#include <stdio.h>

struct simple {
	short s1;
	double d1;
	short s2;
	int i1;
	short s3;
	short s4;
	int i2;
	double d2;
};

struct tiny {
	char c1;
	char c2;
	char c3;
};

struct complexx {
	struct simple s1;
	char c1;
	struct tiny t1;
	struct tiny t2;
	struct simple s2;
};

struct ultra_complex {
	struct complexx c1;
	double d1;
	int i1;
	struct simple s1;
	struct tiny t1;
	char ch1;
	struct tiny t2;
	struct complexx c2;
	char ch2;
};

int main(void) {
	printf("size of simple is %zu\n",sizeof(struct simple)); // 36
	printf("size of tiny is %zu\n",sizeof(struct tiny)); // 4
	printf("size of complexx is %zu\n",sizeof(struct complexx)); // 48 + 36 = 84
	printf("size of ultra_complex is %zu\n",sizeof(struct ultra_complex)); // 272
	return 0;
}

