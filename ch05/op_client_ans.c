#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4	// 
#define OPSZ 4			// 피연산자 크기

void error_handling(char *messasge);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_adr;

	char opmsg[BUF_SIZE];	// 전송할 메세지
	int result;
	int opnd_cnt;	// 피연산자 수
	int i, j;
	int *temp_int;

	if(argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected..........");
	
	// 피연산자 수 입력 받기
	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;	// 전송할 메세지의 첫번째 인자로 피연산자 수 저장. char형으로(-128~127).
//	printf("opmsg[0]: %d\n", opmsg[0]);
//	printf("opmsg[]: %s\n", opmsg);
	
	// 피연산자 입력 받기
	for(i=0; i<opnd_cnt; i++) {
		printf("Operand %d: ", i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]);	// 입력받은 피연산자 int형으로 저장 (4Byte)
		// opnd_cnt=3 -> i=0, opmsg[1], 1~4 / i=1, opmsg[5], 5~8 / i=2, opmsg[9], 9~12
		temp_int = (int*)&opmsg[i*OPSZ+1];
		printf("%d\n", *temp_int);
	}

	// 연산자 입력 받기
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);	// 입력받은 연산자 저장
	printf("%c\n", opmsg[opnd_cnt*OPSZ+1]);
	write(sock, opmsg, opnd_cnt*OPSZ+2);	// 메시지 전송
	read(sock, &result, RLT_SIZE);	// 결과값 전송받기

	printf("Operation result: %d \n", result);
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stdout);
	fputc('\n', stdout);
	exit(1);
}
