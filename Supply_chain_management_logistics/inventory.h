#ifndef INVENTORY_H
#define INVENTORY_H

// minimal color macros (kept in each header to satisfy "4 headers" constraint)
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define BOLDWHITE "\x1b[1;37m"

#define MAX_PRODUCTS 200
#define MAX_NAME_LEN 50
#define MAX_CATEGORY_LEN 30

typedef struct {
    int   id;
    char  name[MAX_NAME_LEN];
    char  category[MAX_CATEGORY_LEN];
    int   quantity;
    float price;
} Product;

extern Product inventory[];
extern int productCount;
extern int lowStockThreshold;

// lifecycle / preload
void initInventory(void);
void loadSampleInventory(void);

// UI actions (handle prompts + validation internally)
void uiAddProduct(void);
void uiUpdateStock(void);
void uiDisplayInventory(void);
void uiCheckLowStock(void);
void uiSetLowStockThreshold(void);

// helpers usable across modules
int invFindIndexById(int id);

// new helper function
void displayProductsBrief(void);

// perishable categories check
int isPerishableCategory(const char* category);

#endif