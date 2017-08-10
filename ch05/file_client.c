/*
 * 사용자로부터 전송 받을 파일의 이름을 입력 받는다.
 * 해당 이름의 파일전송을 서버에게 요청한다.
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
	int sock, fd;
	struct sockaddr_in serv_addr;
	char message[MSG_SIZE];
	int msg_len, read_cnt;

	if(argc != 4) {
		printf("Usage: %s <IP> <PORT> <FILE_NAME>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	// 서버로 파일 이름 전송
	printf("argv[3]: %s, sizeof(argv[3]): %d\n", argv[3], (int) strlen(argv[3]));
	write(sock, argv[3], strlen(argv[3]));

	// 파일 수신
	fd = open(argv[3], O_CREAT|O_WRONLY|O_TRUNC, 0666);	// 파일 생성
	while((read_cnt = read(sock, message, MSG_SIZE)) != 0) {
		fputs(message, stdout);
		// 파일 쓰기
		write(fd, message, read_cnt);
		// 버퍼 초기화
		memset(message, 0, sizeof(message));
	}
	
	close(fd);
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
