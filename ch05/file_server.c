/*
 * 파일이 존재할 경우 파일을 전송하고, 존재하지 않을 경우 연결을 종료한다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define MSG_SIZE 100

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[MSG_SIZE];
	int fd;
	int read_len, read_cnt;

	if(argc != 2) {
		printf("Usage: %s <PORT>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept() error");

	// 파일 이름 수신
	read_len = read(clnt_sock, message, MSG_SIZE);
	printf("read_len = %d\n", read_len);
	printf("File Name: %s\n", message);

	// 파일 확인
	fd = open(message, O_RDONLY);
	if(fd == -1) {
		error_handling("File not exists.");
	}

	// 파일 읽기 및 전송
	while((read_cnt = read(fd, message, MSG_SIZE)) != 0) {
		fputs(message, stdout);

		// 파일 전송
		write(clnt_sock, message, read_cnt);

		// 버퍼 초기화
		memset(message, 0, sizeof(message));
	}

	close(fd);
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
