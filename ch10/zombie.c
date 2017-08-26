#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid = fork();

	if(pid == 0) {	// 자식 프로세스 분기
		puts("Hi I'm a child process");
	} else {				// 부모 프로세스 분기
		printf("Child Process ID: %d \n", pid);
		sleep(30);
	}

	if(pid == 0)	// 자식 프로세스 분기
		puts("End child process");
	else					// 부모 프로세스 분기
		puts("End parent process");

	return 0;
}
