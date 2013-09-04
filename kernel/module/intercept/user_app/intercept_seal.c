#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#define NR_seal 361
#define NR_is_sealed 362

int main(int argc, char *argv[]) {
	char ch, *file_name = "/data/local/example.txt";
	FILE *fp;

	printf("seal : %i\n", syscall(NR_seal));

	fp = fopen(file_name, "r"); // read mode

	if (fp == NULL) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	//printf("The contents of %s file are :\n", file_name);

	//ch = fgetc(fp);
    //printf(">> %c", ch);

	fclose(fp);
	return 0;
}
