#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>

#define MAX_PRODUCTS 1000
#define MAX_NAME_LEN 64
#define MAX_CATEGORY_LEN 32

typedef struct Product {
    int id;
    char name[MAX_NAME_LEN];
    char category[MAX_CATEGORY_LEN];
    int supplierId;
    double price;
    int stock;
} Product;

typedef struct Inventory Inventory;

// Inventory management
Inventory* inventory_create(void);
void inventory_destroy(Inventory* inv);

bool inventory_add_product(Inventory* inv, Product p);
bool inventory_remove_product(Inventory* inv, int productId);
bool inventory_update_stock(Inventory* inv, int productId, int newStock);

Product* inventory_get_product(Inventory* inv, int productId);
void inventory_print_all(Inventory* inv);

int inventory_pop_low_stock_alerts(Inventory* inv, int threshold, int maxCount);
bool inventory_undo_last(Inventory* inv);
void inventory_heap_refresh_all(Inventory* inv);
void inventory_print_category_summary(struct Inventory* inv);

#endif
