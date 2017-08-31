/*
 * IPC 기법을 확인하는 차원에서, 두 프로세스 사이에서 총 3회에 걸쳐서 문자열을
 * 한 번씩 주고받는 예제를 작성해 보자. 물론 두 프로세스는 부모, 자식의 관계로
 * 형성이 되며, 주고 받을 문자열의 종류는 프로그램상에서 임의로 결정한다.
 */
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str[3][20] = {"Hello Process!", "Nice to meet you.", "Bye Bye~"};
	char buf[BUF_SIZE];
	pid_t pid;
	int i;

	pipe(fds1), pipe(fds2);
	pid = fork();

	if(pid == 0) {	// 자식 프로세스
		for(i=0; i<3; i++) {
			write(fds1[1], str[i], sizeof(str[i]));	// 부모 프로세스로 문자열 전송
			read(fds2[0], buf, BUF_SIZE);				// 부모 프로세스로부터 문자열 받기
			printf("message from server: %s\n", buf);
		}
	} else {				// 부모 프로세스
		for(i=0; i<3; i++) {
			read(fds1[0], buf, BUF_SIZE);				// 자식 프로세스로부터 문자열 받기
			printf("message from client: %s\n", buf);
			write(fds2[1], str[i], sizeof(str[i]));	// 자식 프로세스로 문자열 전송
		}
	}

	return 0;
}
