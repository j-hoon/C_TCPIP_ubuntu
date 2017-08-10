#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 100

void error_handling(char* message);

int main(int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int read_cnt;

	if(argc != 3) {
		printf("Usage : %s <org file> <des file>\n", argv[0]);
		exit(1);
	}

	FILE *org = fopen(argv[1], "rb");
	FILE *des = fopen(argv[2], "wb");

	if(org == NULL || des == NULL)
		error_handling("file open error.\n");

	// 파일 읽기 & 쓰기
	while(1) {
		read_cnt = fread((void*)buf, 1, BUF_SIZE, org);
		
		if(read_cnt < BUF_SIZE) {
			if(feof(org) != 0) {
				fwrite((void*)buf, 1, read_cnt, des);
				break;
			} else
				error_handling("file copy error");
		}
		fwrite((void*)buf, 1, BUF_SIZE, des);
	}
	
	fclose(org);
	fclose(des);
	return 0;

	/*
	FILE *org, *des;
	char buf;
	int cnt;

	if(argc != 3) {
		printf("Usage : %s <org file> <des file>\n", argv[0]);
		exit(1);
	}

	// 파일 읽기
	org = fopen(argv[1], "r");

	if(org == NULL)
		error_handling("file not found.\n");

	// 파일 쓰기
	des = fopen(argv[2], "w");

	while(!feof(org)) {
		buf = (char) fgetc(org);
		if(buf != EOF)
			fputc((int) buf, des);
		cnt++;
	}

	printf("file copy complete!\n");

	fclose(org);
	fclose(des);
	return 0;
	*/
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

