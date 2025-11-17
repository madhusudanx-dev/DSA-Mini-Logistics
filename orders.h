#ifndef ORDERS_H
#define ORDERS_H

#include <stdbool.h>
#include "inventory.h"

#define MAX_ORDER_ITEMS 10

typedef struct OrderItem {
    int productId;
    int quantity;
} OrderItem;

typedef struct Order {
    int id;
    char customer[64];
    int numItems;
    OrderItem items[MAX_ORDER_ITEMS];
} Order;

typedef struct OrdersQueue OrdersQueue;

// Queue operations
OrdersQueue* orders_create(void);
void orders_destroy(OrdersQueue* q);

bool orders_enqueue(OrdersQueue* q, Order o);
bool orders_dequeue(OrdersQueue* q, Order* out);
int orders_count(OrdersQueue* q);
void orders_print(OrdersQueue* q);
bool orders_process_next(OrdersQueue* q, Inventory* inv);

#endif
