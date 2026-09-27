#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "product.h"
#include "report.h"

void print_summary(Product *list, int count)
{
	float profit, spent = 0, receive = 0;
	
	for (int i = 0; i < count; i++)
	{
		spent += list[i].initial_quantity * list[i].purchase_price;
		receive += (list[i].initial_quantity - list[i].quantity) * list[i].sale_price;
	}
	
	profit = receive - spent;
	
	printf("\nПотрачено: %.2f \nПолучено: %.2f \nПрибыль: %.2f\n\n", spent, receive, profit);
}

void print_list(const Product *list, int count)
{
	if (!count) printf("Список пуст.\n");
	printf("\nназвание  id  количество  цена покупки  цена продажи   дата прибытия  дата продажи");
	
	for (int i = 0; i < count; i++)
    {
        printf("\n%s       %d      %d           %g             %g          %s", list[i].name, list[i].id, list[i].quantity, list[i].purchase_price, list[i].sale_price, list[i].arrive_date);

        if (list[i].is_sold == 1) printf("       %s", list[i].exite_date);
        else printf("       --.--.----");

        printf("\n");
    }
    
    printf("\n\n");
}
