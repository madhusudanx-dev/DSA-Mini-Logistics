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

// List of perishable categories
const char* perishableCategories[] = {
    "fruits", "fruit", "vegetables", "vegetable", "dairy", 
    "milk", "cheese", "yogurt", "bread", "bakery"
};
const int perishableCategoriesCount = 10;

// Check if category is perishable
int isPerishableCategory(const char* category) {
    for (int i = 0; i < perishableCategoriesCount; i++) {
        if (ciEqual(category, perishableCategories[i])) {
            return 1;
        }
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

// Display products in brief format
void displayProductsBrief(void)
{
    if (productCount == 0)
    {
        printf(RED "[!] No products in inventory.\n" RESET);
        return;
    }
    printf(BOLDWHITE "\n=== AVAILABLE PRODUCTS ===\n" RESET);
    printf(MAGENTA "ID   Name               Category     Qty   Price(Rs.)\n" RESET);
    printf(MAGENTA "----------------------------------------------------\n" RESET);
    for (int i = 0; i < productCount; i++)
    {
        printf(CYAN "%-4d %-18s %-12s %-5d %-9.2f\n" RESET,
               inventory[i].id, inventory[i].name, inventory[i].category,
               inventory[i].quantity, inventory[i].price);
    }
}

// -------- UI actions ----------
void uiAddProduct(void)
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf(RED "[!] Inventory full.\n" RESET);
        return;
    }

    // Show existing products first
    displayProductsBrief();

    Product p;
    if (!readInt("\nEnter Product ID (1-999): ", &p.id) || p.id <= 0 || p.id > 999)
    {
        printf(RED "[!] ID must be between 1-999.\n" RESET);
        return;
    }
    if (invFindIndexById(p.id) != -1)
    {
        printf(RED "[!] Product ID already exists.\n" RESET);
        return;
    }

    readWord("Enter Product Name: ", p.name, sizeof(p.name));
    readWord("Enter Category: ", p.category, sizeof(p.category));

    // Check if category is perishable and show warning
    if (isPerishableCategory(p.category)) {
        printf(YELLOW "\n WARNING: This is a PERISHABLE item!\n" RESET);
        printf(YELLOW "💡 Recommendation: Add smaller quantities to minimize waste\n" RESET);
        printf(YELLOW "   Consider ordering 20-50 units maximum\n" RESET);
    }

    if (!readInt("Enter Quantity (0-999): ", &p.quantity) || p.quantity < 0 || p.quantity > 999)
    {
        printf(RED "[!] Quantity must be between 0-999.\n" RESET);
        return;
    }
    
    // Additional warning for large quantities of perishable items
    if (isPerishableCategory(p.category) && p.quantity > 50) {
        printf(YELLOW "\n WARNING: High quantity for perishable item!\n" RESET);
        printf(YELLOW "   You're adding %d units - this may lead to spoilage\n" RESET, p.quantity);
        printf(YELLOW "   Consider reducing quantity to avoid losses\n" RESET);
        
        char confirm;
        printf(YELLOW "   Continue anyway? (y/n): " RESET);
        scanf(" %c", &confirm);
        flushLine();
        if (confirm != 'y' && confirm != 'Y') {
            printf(YELLOW "[*] Operation cancelled\n" RESET);
            return;
        }
    }
    
    if (!readFloat("Enter Price (Rs.) (1-9999): ", &p.price) || p.price <= 0 || p.price > 9999)
    {
        printf(RED "[!] Price must be between 1-9999.\n" RESET);
        return;
    }

    inventory[productCount++] = p;
    printf(GREEN "[+] Product '%s' added successfully!\n" RESET, p.name);
    
    // Show updated product list
    displayProductsBrief();
}

void uiUpdateStock(void)
{
    // Show products first
    displayProductsBrief();
    
    int id;
    if (!readInt("\nEnter Product ID to update: ", &id))
        return;
    int idx = invFindIndexById(id);
    if (idx == -1)
    {
        printf(RED "[!] Product ID not found.\n" RESET);
        return;
    }

    // Check if updating a perishable item
    if (isPerishableCategory(inventory[idx].category)) {
        printf(YELLOW "\n  PERISHABLE ITEM: %s (%s)\n" RESET, 
               inventory[idx].name, inventory[idx].category);
        printf(YELLOW "💡 Keep quantities low to avoid spoilage\n" RESET);
    }

    int q;
    if (!readInt("Enter new Quantity (0-999): ", &q) || q < 0 || q > 999)
    {
        printf(RED "[!] Quantity must be between 0-999.\n" RESET);
        return;
    }
    
    // Warning for high quantities of perishable items
    if (isPerishableCategory(inventory[idx].category) && q > 50) {
        printf(YELLOW "\n  WARNING: High quantity for perishable item!\n" RESET);
        printf(YELLOW "   Setting quantity to %d may lead to waste\n" RESET, q);
        printf(YELLOW "   Recommended maximum: 50 units\n" RESET);
        
        char confirm;
        printf(YELLOW "   Continue anyway? (y/n): " RESET);
        scanf(" %c", &confirm);
        flushLine();
        if (confirm != 'y' && confirm != 'Y') {
            printf(YELLOW "[*] Operation cancelled\n" RESET);
            return;
        }
    }
    
    inventory[idx].quantity = q;
    printf(YELLOW "[*] '%s' updated to Qty %d\n" RESET, inventory[idx].name, q);
    
    // Show updated product list
    displayProductsBrief();
}

void uiDisplayInventory(void)
{
    displayProductsBrief();
    
    // Show perishable items summary
    printf(YELLOW "\n=== PERISHABLE ITEMS SUMMARY ===\n" RESET);
    int perishableCount = 0;
    for (int i = 0; i < productCount; i++) {
        if (isPerishableCategory(inventory[i].category)) {
            if (perishableCount == 0) {
                printf(YELLOW "ID   Name               Category     Qty   Status\n" RESET);
                printf(YELLOW "-------------------------------------------------\n" RESET);
            }
            perishableCount++;
            const char* status = (inventory[i].quantity > 50) ? "HIGH RISK" : 
                                (inventory[i].quantity > 30) ? "MEDIUM RISK" : "LOW RISK";
            printf(YELLOW "%-4d %-18s %-12s %-5d %s\n" RESET,
                   inventory[i].id, inventory[i].name, inventory[i].category,
                   inventory[i].quantity, status);
        }
    }
    if (perishableCount == 0) {
        printf(GREEN "No perishable items in inventory\n" RESET);
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
            // Add special icon for perishable low stock items
            if (isPerishableCategory(inventory[i].category)) {
                printf(RED "  %-18s Qty: %d (PERISHABLE - URGENT!)\n" RESET, 
                       inventory[i].name, inventory[i].quantity);
            } else {
                printf(RED "%-18s Qty: %d\n" RESET, inventory[i].name, inventory[i].quantity);
            }
        }
    }
    if (!found)
        printf(GREEN "All items sufficiently stocked.\n" RESET);
}

void uiSetLowStockThreshold(void)
{
    int t;
    if (!readInt("\nEnter new low stock threshold (1-100): ", &t) || t < 1 || t > 100)
    {
        printf(RED "[!] Threshold must be between 1-100.\n" RESET);
        return;
    }
    lowStockThreshold = t;
    printf(YELLOW "[*] Threshold set to %d\n" RESET, t);
    
    // Special note for perishable items
    printf(YELLOW " Note: For perishable items, consider keeping threshold higher\n" RESET);
    printf(YELLOW "   to ensure fresh stock and minimize waste\n" RESET);
}