#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct
{
	int id;
	char name[100];
	int quantity;
	int initial_quantity;
	float purchase_price;
	float sale_price;
	char arrive_date[11];
	char exite_date[11];
	int is_sold;
} Product;

int add_product(Product **list, int *count, int *capacity);
int sell_product(Product *list, int count);
int delete_product(Product *list, int *count);

#endif
