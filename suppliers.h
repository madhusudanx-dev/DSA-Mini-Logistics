#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include <stdbool.h>
#include "common.h"

typedef struct Supplier {
    int id;
    char name[64];
    SupplierRatings ratings;
} Supplier;

typedef struct SuppliersDB SuppliersDB;

// AVL Tree supplier functions
SuppliersDB* suppliers_create(void);
void suppliers_destroy(SuppliersDB* db);

bool suppliers_insert(SuppliersDB* db, Supplier s);
bool suppliers_delete(SuppliersDB* db, int supplierId);
Supplier* suppliers_find_by_id(SuppliersDB* db, int supplierId);

void suppliers_print_ranked(SuppliersDB* db);
void suppliers_print_min_rating(SuppliersDB* db, double minOverall);

#endif
