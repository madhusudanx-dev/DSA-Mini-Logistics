#include <stdio.h>
#include "orders.h"
#include "inventory.h"

static Order q[MAX_ORDERS];
static int front = 0, rear = 0;

static int qEmpty(void) { return front == rear; }
static int qFull(void) { return (rear + 1) % MAX_ORDERS == front; }
static void flushLine(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void initOrders(void) { front = rear = 0; }

// Optional: show pending orders
void uiDisplayOrders(void)
{
    if (qEmpty())
    {
        printf(RED "\n[!] No orders currently in queue.\n" RESET);
        return;
    }

    printf(BOLDWHITE "\n=== CURRENT ORDER QUEUE ===\n" RESET);
    printf(CYAN "Product ID   Product Name        Quantity\n" RESET);
    printf(CYAN "------------------------------------------\n" RESET);

    int i = front;
    while (i != rear)
    {
        int idx = invFindIndexById(q[i].productID);
        if (idx != -1)
            printf(CYAN "%-12d %-20s %-8d\n" RESET,
                   inventory[idx].id, inventory[idx].name, q[i].quantity);
        else
            printf(RED "%-12d [Deleted Product] %-8d\n" RESET,
                   q[i].productID, q[i].quantity);
        i = (i + 1) % MAX_ORDERS;
    }
}

//  Updated: show available products before adding
void uiEnqueueOrder(void)
{
    // Show available products from inventory
    printf(BOLDWHITE "\n=== AVAILABLE PRODUCTS ===\n" RESET);
    if (productCount == 0)
    {
        printf(RED "[!] No products available in inventory.\n" RESET);
        return;
    }

    printf(CYAN "ID    Name               Category     Qty   Price(Rs.)\n" RESET);
    printf(CYAN "----------------------------------------------------\n" RESET);
    for (int i = 0; i < productCount; i++)
    {
        printf(CYAN "%-4d %-18s %-12s %-5d %-9.2f\n" RESET,
               inventory[i].id, inventory[i].name, inventory[i].category,
               inventory[i].quantity, inventory[i].price);
    }

    // Now prompt to add order
    if (qFull())
    {
        printf(RED "\n[!] Order queue is full.\n" RESET);
        return;
    }

    int pid, qty;
    printf(BLUE "\nEnter Product ID to order: " RESET);
    if (scanf("%d", &pid) != 1)
    {
        printf(RED "[!] Invalid Product ID.\n" RESET);
        flushLine();
        return;
    }
    flushLine();

    int idx = invFindIndexById(pid);
    if (idx == -1)
    {
        printf(RED "[!] Product ID not found in inventory.\n" RESET);
        return;
    }

    printf(BLUE "Enter Quantity: " RESET);
    if (scanf("%d", &qty) != 1 || qty <= 0)
    {
        printf(RED "[!] Invalid quantity.\n" RESET);
        flushLine();
        return;
    }
    flushLine();

    if (qty > inventory[idx].quantity)
    {
        printf(RED "[!] Not enough stock! Only %d available.\n" RESET, inventory[idx].quantity);
        return;
    }

    q[rear].productID = pid;
    q[rear].quantity = qty;
    rear = (rear + 1) % MAX_ORDERS;

    printf(GREEN "\n[+] Queued Order: %s x %d (Rs. %.2f each)\n" RESET,
           inventory[idx].name, qty, inventory[idx].price);

    // Optional: show current queue
    uiDisplayOrders();
}

void uiProcessOrders(void)
{
    if (qEmpty())
    {
        printf(RED "\n[!] No orders to process.\n" RESET);
        return;
    }

    printf(BOLDWHITE "\n=== PROCESSING ORDERS ===\n" RESET);
    int processed = 0, failed = 0;

    while (!qEmpty())
    {
        Order o = q[front];
        front = (front + 1) % MAX_ORDERS;

        int idx = invFindIndexById(o.productID);
        if (idx == -1)
        {
            printf(RED "Skipped: Product ID %d no longer exists.\n" RESET, o.productID);
            failed++;
            continue;
        }

        if (inventory[idx].quantity >= o.quantity)
        {
            inventory[idx].quantity -= o.quantity;
            printf(GREEN " Processed %s x %d (Remaining: %d)\n" RESET,
                   inventory[idx].name, o.quantity, inventory[idx].quantity);
            processed++;
        }
        else
        {
            printf(RED "Failed: %s only has %d in stock, need %d\n" RESET,
                   inventory[idx].name, inventory[idx].quantity, o.quantity);
            failed++;
        }
    }

    printf(YELLOW "\n[*] Summary: %d processed, %d failed.\n" RESET, processed, failed);
}
