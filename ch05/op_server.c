#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 2048

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	int *operand;
	int cnt_op = 0, result;
	char operator;

	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);
	if(clnt_sock == -1)
		error_handling("accpet() error");
	else
		fputs("Connected..........\n", stdout);

	// 피연산자 수 받기
	read(clnt_sock, message, BUF_SIZE);
	cnt_op = atoi(message);
	printf("Operand count: %d\n", cnt_op);
	operand = (int*) malloc(sizeof(int*) * cnt_op);

	// 피연산자 받기
	for(i=0; i<cnt_op; i++) {
		str_len = read(clnt_sock, message, BUF_SIZE);
		operand[i] = atoi(message);
		printf("operand[%d]: %d\n", i, operand[i]);
	}
	
	// 연산자 받기
	memset(message, 0, sizeof(message));
	str_len = read(clnt_sock, message, BUF_SIZE);
	operator = message[0];
	printf("Operator: %c\n", operator);

	// 받은 피연산자와 연산자로 연산
	result = operand[0];
	for(i=1; i<cnt_op; i++) {
		switch(operator) {
			case '+' :
				result += operand[i];
				break;
			case '-' :
				result -= operand[i];
				break;
			case '*' :
				result *= operand[i];
				break;
			case '/' :
				result /= operand[i];
				break;
		}
	}

	printf("Operation Result: %d\n", result);

	// 연산 결과 보내기
	sprintf(message, "%d", result);
	write(clnt_sock, message, strlen(message));

/*
	while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
//		fputs(message, stdout);	
//		fputc('\n', stdout);
//
		printf("message: %s, strlen(message): %d, str_len: %d\n", message, strlen(message), str_len);
		memset(message, 0, sizeof(message));
	}
*/
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
