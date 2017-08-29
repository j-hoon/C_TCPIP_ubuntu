/*
 * SIGINT에 대한 핸들러를 등록하지 않은 상태에서 Ctrl+C 키가 입력되면,
 * 운영체제가 지정해 놓은 디폴트 이벤트 핸들러에 의해서 프로그램이 종료되어 버린다.
 * 그러나 Ctrl+C 키에 대한 핸들러를 직접 등록하면 프로그램은 종료되지 않고
 * 프로그래머가 지정한 이벤트 핸들러가 대신 호출된다. 그렇다면 일반적인 프로그램에서
 * 다음과 같이 동작하도록 이벤트 핸들러 등록을 위한 코드를 구성해 보자.
 * 		"Ctrl+C 키가 눌리면, 정말로 프로그램을 종료할 것인지 묻고,
 * 		이에 대한 대답으로 'Y'가 입력되면 프로그램을 종료한다."
 * 그리고 간단히 문자열을 1초당 한번 정도 반복 출력하는 프로그램을 작성해서
 * 위의 이벤트 핸들러 등록 코드를 적용시켜 보자.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void ctrl_handler(int sig)
{
	char ex;

	fputs("Do you want exit(Y to exit)? ", stdout);
	scanf("%c", &ex);

	if(ex == 'y' || ex == 'Y')
		exit(1);
}

int main(int argc, char *argv[])
{
	struct sigaction act;

	act.sa_handler = ctrl_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);

	while(1) {
		sleep(1);
		puts("Have a nice day~");
	}

	return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 30

void keycontrol(int sig)
{
	char message[BUF_SIZE];

	if(sig == SIGINT) {
		fputs("\n정말 프로그램을 종료하시겠습니까?(Y to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "Y\n") || !strcmp(message, "y\n"))
			exit(0);		
	}
}

int main(int argc, char *argv[])
{
	int i;
	signal(SIGINT, keycontrol);

	for(i=1; i<=100; i++) {
		printf("Hello! (%d)\n", i);
		sleep(1);
	}

	return 0;
}
*/
