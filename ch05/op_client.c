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
	int sock;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	int cnt_op = 0;
	int *digit;
	char operand;
	int i;
	char *buf_str;

	int buf_operand = 0;
	int ret_fscanf = 0;


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
	
	// 피연산자 수 입력받기
	while(1) {	
		fputs("Operand count: ", stdout);
		
		if(fscanf(stdin, "%10d", &cnt_op) == 0 || cnt_op <= 1) {	// 정수가 아닌 값을 입력하면 fscanf 값은 0 || 2 이상의 정수 / 10자리 이하 정수
			fputs("2 이상의 10자리 이하 정수를 입력해 주세요.\n", stdout);
			while(getchar() != '\n');
		} else {
//			printf("cnt_op: %d\n", cnt_op);
			sprintf(message, "%d", cnt_op);	// 정수를 문자열로 변환
//			printf("message: %s\n", message);
			while(getchar() != '\n');

			write(sock, message, strlen(message));
			break;
		}
	}

	digit = (int*) malloc(sizeof(int*) * cnt_op);

	// 피연산자 입력받기
	for(i=0; i<cnt_op; i++) {
		while(1) {
			printf("Operand %d: ", i+1);

			if(fscanf(stdin, "%10d", &digit[i]) == 0) {	// 정수가 아닌 값을 입력하면 fscanf 값은 0 / 10자리 이하 정수
				fputs("10자리 이하의 정수를 입력해 주세요.\n", stdout);
				while(getchar() != '\n');
			} else {
				sprintf(message, "%d", digit[i]);	// 정수를 문자열로 변환
//				printf("message: %s\n", message);				
				while(getchar() != '\n');

				write(sock, message, strlen(message));
				break;
			}
		}
	}

	// 연산자 입력받기
	while(1) {
		fputs("Operator: ", stdout);

		if(fscanf(stdin, "%c", &operand) != 1 || !(operand == '+' || operand == '-' || operand == '*' || operand == '/')) {	// fscanf 오류 검출 및 +,-,*,/ 문자 확인
			fputs("연산자 +, -, *, / 중 하나를 선택해 입력해 주세요.\n", stdout);
//			printf("ret_fscanf: %d\n", ret_fscanf);
//			printf("oper: %c\n", operand);
			while(getchar() != '\n');
		} else {
			sprintf(message, "%c", operand);	// 문자를 문자열로 변환
//			printf("message: %s, strlen(message): %d\n", message, strlen(message));
//			printf("message[0]: [%c], message[1]: [%c], message[2]: [%c]\n", message[0], message[1], message[2]);
//			printf("message: %s\n", message);
//			printf("ret_fscanf: %d\n", ret_fscanf);
			while(getchar() != '\n');

			write(sock, message, strlen(message));
			break;
		}
	}

	// 연산 결과 받기
	read(sock, message, BUF_SIZE-1);
	printf("Operation Result: %s\n", message);

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
