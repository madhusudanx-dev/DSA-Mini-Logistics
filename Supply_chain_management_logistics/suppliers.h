#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define BOLDWHITE "\x1b[1;37m"

#define MAX_SUPPLIERS 200

typedef struct Supplier{
    int id;
    char name[50];
    float rating; // 0..10
    struct Supplier *left, *right;
    int height;
} Supplier;

extern Supplier supplierList[];
extern int supplierCount;

void initSuppliers(void);
void loadSampleSuppliers(void);

void uiInsertSupplier(void);
void uiDisplaySuppliers(void); // insertion order (unsorted)
void uiRankSuppliers(void);    // ranking view (sorted by rating desc)

#endif
