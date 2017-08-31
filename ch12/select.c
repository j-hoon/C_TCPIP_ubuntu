#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);		// reads 변수의 모든 비르르 0으로 초기화.
	FD_SET(0, &reads);	// 0 is standard input(console)	// fd 0 은 표준입력

	// 이 위치에서 타임아웃을 설정하면 안됨, select 함수호출 이후에는 타임아웃이 발생하기까지 남았던 시간으로 변경되기 때문.
	/*
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	*/

	while(1) {
		temps = reads;	// select 함수호출이 끝나면 변화가 생긴 fd 외 나머지 비트들은 0으로 초기화되므로, 미리 복사.
		timeout.tv_sec = 5;	// 타임아웃 설정을 반복해야함.
		timeout.tv_usec = 0;
		result = select(1, &temps, 0, 0, &timeout);
		if(result == -1) {				// select 함수 에러
			puts("select() error!");
			break;
		} else if(result == 0) {	// fd 변화가 없을 경우(타임아웃)
			puts("Time-out!");
		} else {									// fd 변화가 있을 경우
			if(FD_ISSET(0, &temps)) {	// 변화된 fd가 표준 입력인지 확인
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}

	return 0;
}
