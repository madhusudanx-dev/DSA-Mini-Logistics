#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "search.h"
#include "inventory.h"

// helper for case-insensitive
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
static int ciContains(const char *text, const char *sub)
{
    size_t n = strlen(text), m = strlen(sub);
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

void uiSearchByName(void)
{
    char key[50];
    printf(BLUE "\nEnter product name (substring ok): " RESET);
    scanf("%49s", key);
    int found = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (ciContains(inventory[i].name, key))
        {
            if (!found)
            {
                printf(BOLDWHITE "\nResults:\n" RESET);
                printf(MAGENTA "Name               Category     Qty   Price(Rs.)\n" RESET);
                printf(MAGENTA "------------------------------------------------\n" RESET);
            }
            found = 1;
            printf(CYAN "%-18s %-12s %-5d %-9.2f\n" RESET,
                   inventory[i].name, inventory[i].category,
                   inventory[i].quantity, inventory[i].price);
        }
    }
    if (!found)
        printf(RED "[!] No matching products.\n" RESET);
}

void uiSearchByPriceRange(void)
{
    float mn, mx;
    printf(BLUE "\nEnter min price: " RESET);
    scanf("%f", &mn);
    printf(BLUE "Enter max price: " RESET);
    scanf("%f", &mx);
    if (mn < 0 || mx <= 0 || mn > mx)
    {
        printf(RED "[!] Invalid range.\n" RESET);
        return;
    }
    int f = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (inventory[i].price >= mn && inventory[i].price <= mx)
        {
            if (!f)
            {
                printf(BOLDWHITE "\nProducts Rs.%.2f–%.2f:\n" RESET, mn, mx);
                printf(MAGENTA "Name               Category     Qty   Price(Rs.)\n" RESET);
                printf(MAGENTA "------------------------------------------------\n" RESET);
            }
            f = 1;
            printf(CYAN "%-18s %-12s %-5d %-9.2f\n" RESET,
                   inventory[i].name, inventory[i].category,
                   inventory[i].quantity, inventory[i].price);
        }
    }
    if (!f)
        printf(RED "[!] None found.\n" RESET);
}

void uiSearchByCategory(void)
{
    char cat[30];
    printf(BLUE "\nEnter category: " RESET);
    scanf("%29s", cat);
    int f = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (ciEqual(inventory[i].category, cat))
        {
            if (!f)
            {
                printf(BOLDWHITE "\nProducts in '%s':\n" RESET, cat);
                printf(MAGENTA "Name               Qty   Price(Rs.)\n" RESET);
                printf(MAGENTA "-----------------------------------\n" RESET);
            }
            f = 1;
            printf(CYAN "%-18s %-5d %-9.2f\n" RESET,
                   inventory[i].name, inventory[i].quantity, inventory[i].price);
        }
    }
    if (!f)
        printf(RED "[!] No items in '%s'.\n" RESET, cat);
}
void searchMenu(void)
{
    while (1)
    {
        printf(BOLDWHITE "\n=== PRODUCT SEARCH MENU ===\n" RESET);
        printf(CYAN
               "1. Search by Name\n"
               "2. Search by Price Range\n"
               "3. Search by Category\n"
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
            uiSearchByName();
        else if (ch == 2)
            uiSearchByPriceRange();
        else if (ch == 3)
            uiSearchByCategory();
        else if (ch == 4)
            return;
        else
            printf(RED "[!] Invalid choice.\n" RESET);
    }
}
