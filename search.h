#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>
#include "inventory.h"
#include "suppliers.h"

typedef struct SearchCriteria {
    bool hasCategory;
    bool hasPriceMin;
    bool hasPriceMax;
    bool onlyInStock;
    char sortBy;  // 'p' = price, 'n' = name
    char category[MAX_CATEGORY_LEN];
    double priceMin;
    double priceMax;
} SearchCriteria;

void search_build_and_execute(struct Inventory* inv, struct SuppliersDB* sdb, SearchCriteria criteria);

#endif
