#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io_utils.h"
#include "product.h"
#include "storage.h"
#include "report.h"

int main(void)
{
	const char *filename = "data.dat";
	int run = 1, count = 0, capacity = 2, choice, error;
	
	Product *list = (Product*)malloc(sizeof(Product) * 2);
	if (!list) return 1;
	
	if ((error = load_from_file(filename, &list, &count, &capacity)) == 1) printf("Ошибка, файла нет.\n");
	else if (error == 2) {printf("Ошибка при загрузке."); return 2;}
		
	while(run)
	{
		printf("1. Добавить товар \n2. Показать все товары \n3. Сохранить даные \n4. Продать товар \n5. Удалить товар \n6. Показать отчёт \n0. Выход\n\n");
		choice = read_int("ваш выбор");
		
		switch (choice)
		{
			case 0:
				run = 0;
				break;
				
			case 1:
				if (add_product(&list, &count, &capacity)) printf("Ошибка при добавлении товара.\n\n");
				break;
				
			case 2:
				print_list(list, count);
				break;
				
			case 3:
				if (save_to_file(filename, list, count)) printf("Ошибка при сохранении.\n\n");
				break;
				
			case 4:
				if ((error = sell_product(list, count)) == 1) printf("Ошибка, товара с таким id не найден.\n\n");
				else if (error == 2) printf("Ошибка, количество на продажу превышает клличество в наличие.\n\n");
				break;
				
			case 5:
				if ((error = delete_product(list, &count)) == 1) printf("Ошибка, товара с таким id не найден.\n\n");
				break;
				
			case 6:
				print_summary(list, count);
				break;
				
			default:
				printf("Ошибка, введите снова\n\n");
				break;
		}
	}
	
	free(list);
	return 0;
}
