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
        if (idx != -1) {
            // Show perishable warning in order queue
            if (isPerishableCategory(inventory[idx].category)) {
                printf(YELLOW "%-12d %-20s %-8d  PERISHABLE\n" RESET,
                       inventory[idx].id, inventory[idx].name, q[i].quantity);
            } else {
                printf(CYAN "%-12d %-20s %-8d\n" RESET,
                       inventory[idx].id, inventory[idx].name, q[i].quantity);
            }
        }
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
    displayProductsBrief();

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

    // Check if ordering perishable item
    if (isPerishableCategory(inventory[idx].category)) {
        printf(YELLOW "\n  PERISHABLE ITEM: %s\n" RESET, inventory[idx].name);
        printf(YELLOW "💡 Recommendation: Order smaller quantities more frequently\n" RESET);
        printf(YELLOW "   Suggested maximum: 30 units per order\n" RESET);
    }

    printf(BLUE "Enter Quantity (1-999): " RESET);
    if (scanf("%d", &qty) != 1 || qty <= 0 || qty > 999)
    {
        printf(RED "[!] Quantity must be between 1-999.\n" RESET);
        flushLine();
        return;
    }
    flushLine();

    // Extra warning for large perishable orders
    if (isPerishableCategory(inventory[idx].category) && qty > 30) {
        printf(YELLOW "\n  WARNING: Large order for perishable item!\n" RESET);
        printf(YELLOW "   Ordering %d units may lead to spoilage\n" RESET, qty);
        printf(YELLOW "   Consider splitting into smaller, more frequent orders\n" RESET);
        
        char confirm;
        printf(YELLOW "   Continue anyway? (y/n): " RESET);
        scanf(" %c", &confirm);
        flushLine();
        if (confirm != 'y' && confirm != 'Y') {
            printf(YELLOW "[*] Order cancelled\n" RESET);
            return;
        }
    }

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
            // Special message for perishable items
            if (isPerishableCategory(inventory[idx].category)) {
                printf(YELLOW " Processed %s x %d (Remaining: %d) ⚠️ PERISHABLE\n" RESET,
                       inventory[idx].name, o.quantity, inventory[idx].quantity);
            } else {
                printf(GREEN " Processed %s x %d (Remaining: %d)\n" RESET,
                       inventory[idx].name, o.quantity, inventory[idx].quantity);
            }
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
    
    // Show updated inventory after processing
    displayProductsBrief();
}