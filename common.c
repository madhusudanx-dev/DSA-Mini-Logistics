#include "common.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Removes newline from end of string
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

// Flushes leftover stdin garbage
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ✅ Safe integer input (no range)
int safe_read_int() {
    int x;
    char buf[100];
    while (1) {
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        int valid = 1;
        for (int i = 0; buf[i] && buf[i] != '\n'; i++) {
            if (!isdigit(buf[i]) && !(i == 0 && buf[i] == '-')) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("❌ Invalid input! Enter an integer: ");
            continue;
        }

        if (sscanf(buf, "%d", &x) == 1) return x;

        printf("❌ Invalid integer! Try again: ");
    }
}

// ✅ Safe double input (no range)
double safe_read_double() {
    double x;
    char buf[100];
    while (1) {
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        int valid = 1, dotCount = 0;
        for (int i = 0; buf[i] && buf[i] != '\n'; i++) {
            if (!isdigit(buf[i]) && buf[i] != '.' && !(i == 0 && buf[i] == '-')) {
                valid = 0; break;
            }
            if (buf[i] == '.') dotCount++;
            if (dotCount > 1) { valid = 0; break; }
        }

        if (!valid) {
            printf("❌ Invalid input! Enter a number: ");
            continue;
        }

        if (sscanf(buf, "%lf", &x) == 1) return x;

        printf("❌ Invalid number! Try again: ");
    }
}

// ✅ NEW — Safe integer input with range check
int safe_read_int_range(int min, int max) {
    int val;
    while (1) {
        val = safe_read_int();
        if (val < min || val > max) {
            printf("⚠️  Enter a value between %d and %d: ", min, max);
            continue;
        }
        return val;
    }
}

// ✅ NEW — Safe double input with range check
double safe_read_double_range(double min, double max) {
    double val;
    while (1) {
        val = safe_read_double();
        if (val < min || val > max) {
            printf("⚠️  Enter a value between %.2f and %.2f: ", min, max);
            continue;
        }
        return val;
    }
}

// ✅ Supplier score calculator (unchanged)
double supplier_overall_score(const SupplierRatings *r) {
    return (r->quality + r->deliveryTime + r->price + r->reliability + r->customerService) / 5.0;
}
