#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int fd_org, fd_des;
	int read_cnt;
	char buf[BUF_SIZE];

	if(argc != 3) {
		printf("Usage : %s <org_file> <des_file> \n", argv[0]);
		exit(1);
	}

	fd_org = open(argv[1], O_RDONLY);
	fd_des = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0666);

	if(fd_org == -1 || fd_des == -1)
		error_handling("open() error!");
	
	// 파일 읽기 & 쓰기
	while((read_cnt = read(fd_org, buf, BUF_SIZE)) != 0)
		write(fd_des, buf, read_cnt);

	close(fd_org);
	close(fd_des);
	return 0;

	/*
	int fd_org, fd_des;
	char buf[BUF_SIZE];
	int str_len;

	if(argc != 3) {
		printf("Usage : %s <org_file> <des_file> \n", argv[0]);
		exit(1);
	}
	
	// 파일 읽기
	fd_org = open(argv[1], O_RDONLY);
	if(fd_org == -1)
		error_handling("open_org() error!");

	printf("file descriptor (org): %d \n", fd_org);

	str_len = read(fd_org, buf, sizeof(buf)-1);
	if(str_len == -1)
		error_handling("read() error!");

	printf("file data: %s", buf);
	printf("file length: %d \n", str_len);

	// 파일 쓰기
	fd_des = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0666);
	if(fd_des == -1)
		error_handling("open_des() error!");

	printf("file descriptor (des): %d \n", fd_des);

	if(write(fd_des, buf, str_len) == -1)
		error_handling("write() error!");

	printf("file copy complete!\n");

	close(fd_org);
	close(fd_des);
	*/
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

