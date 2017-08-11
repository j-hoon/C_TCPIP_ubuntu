#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	char buf[4];
	int msg_len = 0;
	int idx = 0, read_len = 0;

	if(argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	// 서버로부터 문자열 길이 수신
	read(sock, buf, 4);
	msg_len = atoi(buf);
	printf("Message Length from server: %d \n", msg_len);
	
	// 수신한 문자열 길이 재전송
	write(sock, buf, 4);

	// 메시지 수신
	read_len = read(sock, message, msg_len);
	printf("Read Length from server: %d \n", read_len);
	printf("Message from server: %s \n", message);

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}