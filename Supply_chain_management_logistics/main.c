#include <stdio.h>
#include "inventory.h"
#include "orders.h"
#include "search.h"
#include "suppliers.h"

// ask if user wants to repeat action
static int wantAnother(void)
{
    char a;
    printf(YELLOW "\nDo another operation? (y/n): " RESET);
    if (scanf(" %c", &a) != 1)
        return 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    return (a == 'y' || a == 'Y');
}

static void inventoryMenu(void)
{
    // overview on entry
    printf(BOLDWHITE "\n--- INVENTORY OVERVIEW ---\n" RESET);
    uiDisplayInventory();

    while (1)
    {
        printf(BOLDWHITE "\n=== INVENTORY MENU ===\n" RESET);
        printf(CYAN
               "1. Add Product\n"
               "2. Update Stock\n"
               "3. Display Inventory\n"
               "4. Low Stock Alert\n"
               "5. Set Low Stock Threshold\n"
               "6. Back\n" RESET);
        printf(YELLOW "Enter choice: " RESET);

        int ch;
        if (scanf("%d", &ch) != 1)
        {
            printf(RED "[!] Invalid choice.\n" RESET);
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        if (ch == 1)
        {
            do
            {
                uiAddProduct();
            } while (wantAnother());
        }
        else if (ch == 2)
        {
            do
            {
                uiUpdateStock();
            } while (wantAnother());
        }
        else if (ch == 3)
        {
            uiDisplayInventory();
        }
        else if (ch == 4)
        {
            uiCheckLowStock();
        }
        else if (ch == 5)
        {
            uiSetLowStockThreshold();
        }
        else if (ch == 6)
        {
            return;
        }
        else
            printf(RED "[!] Invalid choice.\n" RESET);
    }
}

static void ordersMenu(void)
{
    while (1)
    {
        printf(BOLDWHITE "\n=== ORDER MENU ===\n" RESET);
        printf(CYAN
               "1. Add Order\n"
               "2. Process Orders\n"
               "3. Display Orders\n"
               "4. Back\n" RESET);
        printf(YELLOW "Enter choice: " RESET);

        int ch;
        if (scanf("%d", &ch) != 1)
        {
            printf(RED "[!] Invalid choice.\n" RESET);
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        if (ch == 1)
        {
            do
            {
                uiEnqueueOrder();
            } while (wantAnother());
        }
        else if (ch == 2)
        {
            uiProcessOrders();
        }
        else if (ch == 3)
        {
            uiDisplayOrders();
        }
        else if (ch == 4)
        {
            return;
        }
        else
            printf(RED "[!] Invalid choice.\n" RESET);
    }
}

static void suppliersMenu(void)
{
    while (1)
    {
        printf(BOLDWHITE "\n=== SUPPLIER MENU ===\n" RESET);
        printf(CYAN
               "1. Add Supplier\n"
               "2. View Suppliers\n"
               "3. Supplier Ranking\n"
               "4. Back\n" RESET);
        printf(YELLOW "Enter choice: " RESET);

        int ch;
        if (scanf("%d", &ch) != 1)
        {
            printf(RED "[!] Invalid choice.\n" RESET);
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        if (ch == 1)
        {
            do
            {
                uiInsertSupplier();
            } while (wantAnother());
        }
        else if (ch == 2)
        {
            uiDisplaySuppliers();
        }
        else if (ch == 3)
        {
            uiRankSuppliers();
        }
        else if (ch == 4)
        {
            return;
        }
        else
            printf(RED "[!] Invalid choice.\n" RESET);
    }
}

int main(void)
{
    initInventory();
    initOrders();
    initSuppliers();

    // preload demo data
    loadSampleInventory();
    loadSampleSuppliers();

    while (1)
    {
        printf(BOLDWHITE "\n===== SUPPLY CHAIN MANAGEMENT SYSTEM =====\n" RESET);
        printf(CYAN
               "1. Manage Inventory\n"
               "2. Orders\n"
               "3. Product Search\n"
               "4. Suppliers\n"
               "5. Exit\n" RESET);
        printf(YELLOW "Enter choice: " RESET);

        int ch;
        if (scanf("%d", &ch) != 1)
        {
            printf(RED "[!] Invalid choice.\n" RESET);
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        if (ch == 1)
            inventoryMenu();
        else if (ch == 2)
            ordersMenu();
        else if (ch == 3)
            searchMenu();
        else if (ch == 4)
            suppliersMenu();
        else if (ch == 5)
        {
            printf(GREEN "\nThank You. Goodbye!\n" RESET);
            break;
        }
        else
            printf(RED "[!] Invalid choice.\n" RESET);
    }
    return 0;
}