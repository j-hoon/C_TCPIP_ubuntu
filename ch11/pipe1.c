#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();

	if(pid == 0) {	// 자식 프로세스
		write(fds[1], str, sizeof(str));
	} else {				// 부모 프로세스
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	return 0;
}
