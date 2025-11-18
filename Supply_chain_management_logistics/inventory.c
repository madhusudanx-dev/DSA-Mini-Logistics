#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"

Product inventory[MAX_PRODUCTS];
int productCount = 0;
int lowStockThreshold = 10;

// -------- util ----------
static void flushLine(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

static int readInt(const char *msg, int *out)
{
    printf(BLUE "%s" RESET, msg);
    if (scanf("%d", out) != 1)
    {
        printf(RED "[!] Invalid number.\n" RESET);
        flushLine();
        return 0;
    }
    flushLine();
    return 1;
}
static int readFloat(const char *msg, float *out)
{
    printf(BLUE "%s" RESET, msg);
    if (scanf("%f", out) != 1)
    {
        printf(RED "[!] Invalid number.\n" RESET);
        flushLine();
        return 0;
    }
    flushLine();
    return 1;
}
static void readWord(const char *msg, char *buf, size_t cap)
{
    (void)cap; // simple word input
    printf(BLUE "%s" RESET, msg);
    scanf("%49s", buf);
    flushLine();
}

// case-insensitive equals
static int ciEqual(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}
// case-insensitive contains
static int ciContains(const char *text, const char *sub)
{
    size_t n = strlen(text), m = strlen(sub);
    if (m == 0)
        return 1;
    for (size_t i = 0; i + m <= n; i++)
    {
        size_t j = 0;
        while (j < m && tolower((unsigned char)text[i + j]) == tolower((unsigned char)sub[j]))
            j++;
        if (j == m)
            return 1;
    }
    return 0;
}

// -------- helpers ----------
int invFindIndexById(int id)
{
    for (int i = 0; i < productCount; i++)
        if (inventory[i].id == id)
            return i;
    return -1;
}

// -------- lifecycle ----------
void initInventory(void) { productCount = 0; }

void loadSampleInventory(void)
{
    struct
    {
        int id;
        const char *name, *cat;
        int qty;
        float price;
    } s[] = {
        {101, "Apple", "Fruits", 40, 90}, {102, "Banana", "Fruits", 80, 30}, {103, "Tomato", "Vegetables", 50, 25}, {104, "Potato", "Vegetables", 60, 20}, {105, "Milk", "Dairy", 30, 50}, {106, "Cheese", "Dairy", 15, 120}, {107, "Soap", "Household", 45, 25}, {108, "Shampoo", "Household", 25, 80}, {109, "Rice", "Grains", 100, 60}, {110, "Wheat", "Grains", 90, 55}};
    for (int i = 0; i < 10; i++)
    {
        inventory[i].id = s[i].id;
        strcpy(inventory[i].name, s[i].name);
        strcpy(inventory[i].category, s[i].cat);
        inventory[i].quantity = s[i].qty;
        inventory[i].price = s[i].price;
    }
    productCount = 10;
}

// -------- UI actions ----------
void uiAddProduct(void)
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf(RED "[!] Inventory full.\n" RESET);
        return;
    }

    Product p;
    if (!readInt("\nEnter Product ID: ", &p.id) || p.id <= 0)
    {
        printf(RED "[!] ID must be positive.\n" RESET);
        return;
    }
    if (invFindIndexById(p.id) != -1)
    {
        printf(RED "[!] Product ID already exists.\n" RESET);
        return;
    }

    readWord("Enter Product Name: ", p.name, sizeof(p.name));
    readWord("Enter Category: ", p.category, sizeof(p.category));

    if (!readInt("Enter Quantity: ", &p.quantity) || p.quantity < 0)
    {
        printf(RED "[!] Quantity cannot be negative.\n" RESET);
        return;
    }
    if (!readFloat("Enter Price (Rs.): ", &p.price) || p.price <= 0)
    {
        printf(RED "[!] Price must be positive.\n" RESET);
        return;
    }

    inventory[productCount++] = p;
    printf(GREEN "[+] Product '%s' added successfully!\n" RESET, p.name);
}

void uiUpdateStock(void)
{
    int id;
    if (!readInt("\nEnter Product ID to update: ", &id))
        return;
    int idx = invFindIndexById(id);
    if (idx == -1)
    {
        printf(RED "[!] Product ID not found.\n" RESET);
        return;
    }

    int q;
    if (!readInt("Enter new Quantity: ", &q) || q < 0)
    {
        printf(RED "[!] Quantity cannot be negative.\n" RESET);
        return;
    }
    inventory[idx].quantity = q;
    printf(YELLOW "[*] '%s' updated to Qty %d\n" RESET, inventory[idx].name, q);
}

void uiDisplayInventory(void)
{
    if (productCount == 0)
    {
        printf(RED "[!] No products in inventory.\n" RESET);
        return;
    }
    printf(BOLDWHITE "\n=== INVENTORY LIST ===\n" RESET);
    printf(MAGENTA "ID   Name               Category     Qty   Price(Rs.)\n" RESET);
    printf(MAGENTA "----------------------------------------------------\n" RESET);
    for (int i = 0; i < productCount; i++)
    {
        printf(CYAN "%-4d %-18s %-12s %-5d %-9.2f\n" RESET,
               inventory[i].id, inventory[i].name, inventory[i].category,
               inventory[i].quantity, inventory[i].price);
    }
}

void uiCheckLowStock(void)
{
    printf(MAGENTA "\nLow stock items (Qty < %d):\n" RESET, lowStockThreshold);
    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (inventory[i].quantity < lowStockThreshold)
        {
            if (!found)
                printf(MAGENTA "---------------------------------\n" RESET);
            found = 1;
            printf(RED "%-18s Qty: %d\n" RESET, inventory[i].name, inventory[i].quantity);
        }
    }
    if (!found)
        printf(GREEN "All items sufficiently stocked.\n" RESET);
}

void uiSetLowStockThreshold(void)
{
    int t;
    if (!readInt("\nEnter new low stock threshold: ", &t) || t < 1)
    {
        printf(RED "[!] Invalid threshold.\n" RESET);
        return;
    }
    lowStockThreshold = t;
    printf(YELLOW "[*] Threshold set to %d\n" RESET, t);
}
