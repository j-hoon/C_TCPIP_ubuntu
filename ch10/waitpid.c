#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid = fork();

	if(pid == 0) {	// 자식 프로세스 분기
		sleep(15);
		return 24;
	} else {				// 부모 프로세스 분기
		while(!waitpid(-1, &status, WNOHANG)) {	// 임의의 자식 프로세스가 종료되기를 기다림(-1), 종료된 자식 프로세스가 없으면 0 반환.
			sleep(3);
			puts("sleep 3sec.");
		}

		if(WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}

	return 0;
}
