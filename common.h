#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

typedef struct SupplierRatings {
    double quality;
    double deliveryTime;
    double price;
    double reliability;
    double customerService;
} SupplierRatings;

double supplier_overall_score(const SupplierRatings *r);
void trim_newline(char *s);
int safe_read_int();
double safe_read_double();

// ✅ new declarations
int safe_read_int_range(int min, int max);
double safe_read_double_range(double min, double max);

#endif
