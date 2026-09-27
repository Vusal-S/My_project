#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "product.h"
#include "storage.h"


int save_to_file(const char *filename, Product *list, int count)
{
	FILE *fin = fopen(filename, "wb");
	if (!fin) return 1;
	
	if (fwrite(list, sizeof(Product), count, fin) != (size_t)count) return 2;
	printf("Данные успешно сохранены.\n\n");
	
	fclose(fin);
	return 0;
}

int load_from_file(const char *filename, Product **list, int *count, int *capacity)
{
	FILE *fout = fopen(filename, "rb");
	if (!fout) return 1;
	
	fseek(fout, 0, SEEK_END);
	*count = ftell(fout) / sizeof(Product);
	fseek(fout, 0, SEEK_SET);
	
	if ((*count) >= (*capacity))
	{
		(*capacity) = 2 * (*count);
		if (!(*list = (Product*)realloc(*list, sizeof(Product) * (*capacity)))) return 2;
	}
	
	fread(*list, sizeof(Product), (size_t)(*count), fout);
	
	fclose(fout);
	return 0;
}
