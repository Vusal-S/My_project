#include "io_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_line(char *buffer, int size)
{
	char c;
	fgets(buffer, size, stdin);
	if (strchr(buffer, '\n') == NULL)	while ((c = getchar()) != '\n' && c != EOF);
	buffer[strcspn(buffer, "\n")] = '\0';
}

int read_int(const char *prompt)
{
	char input[100];
	
	printf("\nВведите %s: ", prompt);
	read_line(input, 100);
	
	return atoi(input);
}

float read_float(const char *prompt)
{
	char input[100];
	
	printf("\nВведите %s: ", prompt);
	read_line(input, 100);
	
	return atof(input);
}
