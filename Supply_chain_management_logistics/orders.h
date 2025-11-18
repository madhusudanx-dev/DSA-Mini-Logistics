#ifndef ORDERS_H
#define ORDERS_H

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define BOLDWHITE "\x1b[1;37m"

#define MAX_ORDERS 200

typedef struct {
    int productID;
    int quantity;
} Order;

void initOrders(void);
void uiEnqueueOrder(void);
void uiProcessOrders(void);
void uiDisplayOrders(void);

#endif
