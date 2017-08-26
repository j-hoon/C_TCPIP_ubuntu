#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig == SIGALRM)
		puts("Time out!");
	alarm(2);
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction act;
	act.sa_handler = timeout;		// 시그널 핸들러 등록
	sigemptyset(&act.sa_mask);	// 시그널 마스크 초기화
	act.sa_flags = 0;						// 시그널 플래그 초기화
	sigaction(SIGALRM, &act, 0);	// SIGALRM 시그널 등록

	alarm(2);

	for(i=0; i<3; i++) {
		puts("wait...");
		sleep(100);
	}

	return 0;
}
