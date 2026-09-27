#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io_utils.h"
#include "product.h"

int add_product(Product **list, int *count, int *capacity)
{
	char name[100], arrive_date[11]; 
	
	if ((*count) == (*capacity))
	{
		(*capacity) *= 2;
		if (!(*list = (Product*)realloc(*list, sizeof(Product) * (*capacity)))) return 1;
	}
	
	if (*count != 0)
		(*list)[*count].id = (*list)[*count - 1].id + 1;
	else
		(*list)[*count].id = 1;
	
	printf("\nВведите название товара: ");
	read_line(name, sizeof(name));
	strcpy((*list)[*count].name, name);
	
	(*list)[*count].quantity = read_int("количество");
	(*list)[*count].initial_quantity = (*list)[*count].quantity;
	
	(*list)[*count].purchase_price = read_float("цену покупки");
	
	(*list)[*count].sale_price = read_float("цену продажи");
	
	printf("\nВведите дату прибытия: ");
	read_line(arrive_date, sizeof(arrive_date));
	strcpy((*list)[*count].arrive_date, arrive_date);
	
	strcpy((*list)[*count].exite_date, "--.--.----");
	
	(*list)[*count].is_sold = 0;
	
	(*count)++;
	
	printf("\nТовар добавлен.\n\n");
	
	return 0;
}

int sell_product(Product *list, int count)
{
	int id = read_int("id товара");
	int quantity_to_sell = read_int("количество на продажу");
	char exite_date[11];
	
	int i;
	for (i = 0; i < count; i++)
		if (list[i].id == id) break;
		
	if (i == count) return 1;
	if (quantity_to_sell > list[i].quantity) return 2;
	
	list[i].quantity -= quantity_to_sell;
	if (list[i].quantity == 0)
	{
		list[i].is_sold = 1;
		printf("Введите дату продажи: ");
		read_line(exite_date, sizeof(exite_date));
		strcpy(list[i].exite_date, exite_date);
	}
	
	printf("\n\n");
	
	return 0;
}

int delete_product(Product *list, int *count)
{
	int j, id = read_int("id товара");
	
	for (j = 0; j < *count; j++)
		if (list[j].id == id) break;
		
	if (j == *count) return 1;
	
	for (int i = j; i < *count - 1; i++)
		list[i] = list[i + 1];
	
	printf("Товар успешно удалён.\n\n");
	(*count)--;
	
	return 0;
}
