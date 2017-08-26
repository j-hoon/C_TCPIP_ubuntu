/*
 * 상호간에 한번씩 메시지를 주고받는 형태로 대화를 진행.
 * 주고받는 대화는 콘솔상에 출력되어야 한다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;

	if(argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while(1) {
		fputs("  --Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		printf("Me: %s", message);
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		
		// 문자열 전송
		sendto(sock, message, strlen(message), 0, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*) &from_adr, &adr_sz);	// 문자열 수신

		message[str_len-1] = '\0';	// 개행문자 제거
		fputs("Server: ", stdout);
		fputs(message, stdout);
	}

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
