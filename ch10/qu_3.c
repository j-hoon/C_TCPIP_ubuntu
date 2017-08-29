/*
 * 자식 프로세스가 생성되면 부모 프로세스의 모든 것을 복사하는데,
 * 이때 복사의 대상으로는 소켓의 파일 디스크립터도 포함이 된다.
 * 그렇다면 복사된 파일 디스크립터의 정수 값은 원본 파일 디스크립터의
 * 정수 값과 동일한지 확인하기 위한 프로그램을 작성해 보자.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);

	pid = fork();

	if(pid == 0)
		printf("Child sock fd: [%d] \n", sockfd);
	else
		printf("Child sock fd: [%d] \n", sockfd);

	return 0;
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in adr;
	socklen_t adr_sz;
	pid_t pid;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(atoi("9190"));

	pid = fork();	// 자식 프로세스 생성

	if(pid == 0) {	// 자식 프로세스 분기
		printf("Child Proc PID: %d\n", pid);
		printf("Child Proc File Descrpitor: %d\n", sock);
	} else {				// 부모 프로세스 분기
		printf("Parent Proc PID: %d\n", pid);
		printf("Parent Proc File Descrpitor: %d\n", sock);
	}

	return 0;
}
*/
