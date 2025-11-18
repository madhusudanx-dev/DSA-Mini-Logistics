#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "suppliers.h"

// storage
Supplier *root = NULL;
Supplier supplierList[MAX_SUPPLIERS];
int supplierCount = 0;

// utils
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
    (void)cap;
    printf(BLUE "%s" RESET, msg);
    scanf("%49s", buf);
    flushLine();
}

// AVL
static int height(Supplier *n) { return n ? n->height : 0; }
static int max2(int a, int b) { return a > b ? a : b; }
static Supplier *newNode(int id, const char *name, float rating)
{
    Supplier *s = (Supplier *)malloc(sizeof(Supplier));
    s->id = id;
    strcpy(s->name, name);
    s->rating = rating;
    s->left = s->right = NULL;
    s->height = 1;
    return s;
}
static Supplier *rotR(Supplier *y)
{
    Supplier *x = y->left, *t2 = x->right;
    x->right = y;
    y->left = t2;
    y->height = max2(height(y->left), height(y->right)) + 1;
    x->height = max2(height(x->left), height(x->right)) + 1;
    return x;
}
static Supplier *rotL(Supplier *x)
{
    Supplier *y = x->right, *t2 = y->left;
    y->left = x;
    x->right = t2;
    x->height = max2(height(x->left), height(x->right)) + 1;
    y->height = max2(height(y->left), height(y->right)) + 1;
    return y;
}
static int bal(Supplier *n) { return n ? height(n->left) - height(n->right) : 0; }

static Supplier *avlInsert(Supplier *node, int id, const char *name, float rating)
{
    if (!node)
        return newNode(id, name, rating);
    if (rating < node->rating)
        node->left = avlInsert(node->left, id, name, rating);
    else if (rating > node->rating)
        node->right = avlInsert(node->right, id, name, rating);
    else
        return node;

    node->height = 1 + max2(height(node->left), height(node->right));
    int b = bal(node);
    if (b > 1 && rating < node->left->rating)
        return rotR(node);
    if (b < -1 && rating > node->right->rating)
        return rotL(node);
    if (b > 1 && rating > node->left->rating)
    {
        node->left = rotL(node->left);
        return rotR(node);
    }
    if (b < -1 && rating < node->right->rating)
    {
        node->right = rotR(node->right);
        return rotL(node);
    }
    return node;
}

// lifecycle / preload
void initSuppliers(void)
{
    root = NULL;
    supplierCount = 0;
}

void loadSampleSuppliers(void)
{
    struct
    {
        int id;
        const char *name;
        float rating;
    } s[] = {
        {201, "FreshFarms", 9.2f},
        {202, "QuickMart", 8.5f},
        {203, "AgroLink", 7.8f},
        {204, "DailySupply", 8.9f},
        {205, "OrganicCo", 9.5f}};
    for (int i = 0; i < 5; i++)
    {
        root = avlInsert(root, s[i].id, s[i].name, s[i].rating);
        supplierList[supplierCount] = *newNode(s[i].id, s[i].name, s[i].rating);
        supplierCount++;
    }
}

// UI
void uiInsertSupplier(void)
{
    if (supplierCount >= MAX_SUPPLIERS)
    {
        printf(RED "[!] Supplier list full.\n" RESET);
        return;
    }
    int id;
    char name[50];
    float rating;

    if (!readInt("\nEnter Supplier ID: ", &id) || id <= 0)
    {
        printf(RED "[!] ID must be positive.\n" RESET);
        return;
    }
    readWord("Enter Supplier Name: ", name, sizeof(name));
    if (!readFloat("Enter Rating (0.0 - 10.0): ", &rating) || rating < 0 || rating > 10)
    {
        printf(RED "[!] Rating must be in 0..10.\n" RESET);
        return;
    }

    root = avlInsert(root, id, name, rating);
    supplierList[supplierCount] = *newNode(id, name, rating);
    supplierCount++;

    printf(GREEN "[+] Supplier '%s' added (Rating: %.2f)\n" RESET, name, rating);
}

void uiDisplaySuppliers(void)
{
    if (supplierCount == 0)
    {
        printf(RED "[!] No suppliers added yet.\n" RESET);
        return;
    }
    printf(BOLDWHITE "\n=== SUPPLIER LIST (Insertion Order) ===\n" RESET);
    printf(CYAN "ID    Name                Rating\n" RESET);
    printf(CYAN "-------------------------------------\n" RESET);
    for (int i = 0; i < supplierCount; i++)
    {
        printf(CYAN "%-5d %-18s %.2f\n" RESET, supplierList[i].id, supplierList[i].name, supplierList[i].rating);
    }
}

void uiRankSuppliers(void)
{
    if (supplierCount == 0)
    {
        printf(RED "[!] No suppliers to rank.\n" RESET);
        return;
    }

    Supplier tmp[MAX_SUPPLIERS];
    for (int i = 0; i < supplierCount; i++)
        tmp[i] = supplierList[i];

    // sort by rating desc (simple bubble sort for demo)
    for (int i = 0; i < supplierCount - 1; i++)
        for (int j = 0; j < supplierCount - i - 1; j++)
            if (tmp[j].rating < tmp[j + 1].rating)
            {
                Supplier t = tmp[j];
                tmp[j] = tmp[j + 1];
                tmp[j + 1] = t;
            }

    printf(BOLDWHITE "\n=== SUPPLIER RANKING (By Rating) ===\n" RESET);
    for (int i = 0; i < supplierCount; i++)
    {
        printf(GREEN "Rank %d : %-18s Rating: %.2f\n" RESET, i + 1, tmp[i].name, tmp[i].rating);
    }
}
