#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[])
{
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;

	pid = fork();	// 프로세스 생성(복제)
	if(pid == 0)	// 자식 프로세스 분기
		gval += 2, lval += 2;
	else	// 부모 프로세스 분기
		gval -= 2, lval -= 2;
	
	if(pid == 0)
		printf("Child Proc: [%d, %d] \n", gval, lval);
	else
		printf("Parent Proc: [%d, %d] \n", gval, lval);

	return 0;
}
