#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "common.h"
#include "inventory.h"
#include "orders.h"
#include "search.h"
#include "suppliers.h"

#define VERSION "3.0.0"

void print_line() { printf("-----------------------------------------------------------\n"); }
void big_line()   { printf("===========================================================\n"); }

// ✅ Utility for safe input
int read_menu_choice(int min, int max) {
    printf("> ");
    return safe_read_int_range(min, max);
}

// INVENTORY MENU
void show_inventory_menu(Inventory* inv) {
    int choice;
    do {
        printf("\n\n");
        big_line();
        printf("              INVENTORY MANAGEMENT\n");
        big_line();
        inventory_print_category_summary(inv);
        print_line();

        printf("\nOptions:\n");
        printf("  1. Add Product\n");
        printf("  2. Update Stock\n");
        printf("  3. Remove Product\n");
        printf("  4. Show Low Stock Alerts\n");
        printf("  5. Undo Last Action\n");
        printf("  0. Back\n");
        choice = read_menu_choice(0, 5);

        if (choice == 1) {
            Product p = {0};
            printf("\nEnter New Product Details:\n");
            printf("  ID (1–9999): "); p.id = safe_read_int_range(1, 9999);
            printf("  Name: "); scanf(" %63[^\n]", p.name);
            printf("  Category: "); scanf(" %31[^\n]", p.category);
            printf("  Supplier ID: "); p.supplierId = safe_read_int_range(1, 9999);
            printf("  Price: "); p.price = safe_read_double_range(0, 100000);
            printf("  Stock: "); p.stock = safe_read_int_range(0, 100000);

            if (inventory_add_product(inv, p))
                printf("\n✅ Product added successfully!\n");
            else
                printf("\n❌ Failed to add product!\n");
        }
        else if (choice == 2) {
            printf("\nEnter Product ID to update: ");
            int id = safe_read_int_range(1, 9999);
            Product* prod = inventory_get_product(inv, id);
            if (!prod) {
                printf("\n❌ Product ID not found!\n");
                continue;
            }
            printf("Enter new stock (0–100000): ");
            int stock = safe_read_int_range(0, 100000);
            inventory_update_stock(inv, id, stock);
            printf("\n✅ Stock updated successfully!\n");
        }
        else if (choice == 3) {
            printf("\nEnter Product ID to remove: ");
            int id = safe_read_int_range(1, 9999);
            if (inventory_remove_product(inv, id))
                printf("\n✅ Product removed successfully!\n");
            else
                printf("\n❌ Product not found!\n");
        }
        else if (choice == 4) {
            printf("\nShow products with stock less than: ");
            int threshold = safe_read_int_range(0, 100000);
            printf("Maximum alerts to show: ");
            int max = safe_read_int_range(1, 1000);
            inventory_pop_low_stock_alerts(inv, threshold, max);
        }
        else if (choice == 5) {
            inventory_undo_last(inv);
        }

    } while (choice != 0);
}

// SIMPLE SEARCH - Shows products immediately!
void show_search_menu(Inventory* inv, SuppliersDB* sdb) {
    int choice;
    do {
        printf("\n\n");
        big_line();
        printf("                    PRODUCT SEARCH\n");
        big_line();
        
        // Show ALL products immediately - no questions asked!
        inventory_print_all(inv);
        print_line();

        printf("\nQuick Filters (click to apply):\n");
        printf("  1. Show Only In-Stock Products\n");
        printf("  2. Sort by Price (Low to High)\n");
        printf("  3. Sort by Price (High to Low)\n");
        printf("  4. Sort by Name (A-Z)\n");
        printf("  5. Filter by Category\n");
        printf("  6. Filter by Price Range\n");
        printf("  0. Back to Main Menu\n");
        choice = read_menu_choice(0, 6);

        SearchCriteria criteria;
        memset(&criteria, 0, sizeof(criteria));
        
        if (choice == 1) {
            // Show only in-stock products
            criteria.onlyInStock = true;
            search_build_and_execute(inv, sdb, criteria);
        } 
        else if (choice == 2) {
            // Sort by price low to high
            criteria.sortBy = 'p';
            search_build_and_execute(inv, sdb, criteria);
        } 
        else if (choice == 3) {
            // Sort by price high to low - we'll handle this in search.c
            criteria.sortBy = 'P'; // Use uppercase for reverse
            search_build_and_execute(inv, sdb, criteria);
        } 
        else if (choice == 4) {
            // Sort by name
            criteria.sortBy = 'n';
            search_build_and_execute(inv, sdb, criteria);
        } 
        else if (choice == 5) {
            // Filter by category
            printf("\nEnter category name (or press Enter to see all): ");
            char input[32];
            scanf(" %31[^\n]", input);
            if (strlen(input) > 0) {
                strncpy(criteria.category, input, MAX_CATEGORY_LEN - 1);
                criteria.hasCategory = true;
            }
            search_build_and_execute(inv, sdb, criteria);
        } 
        else if (choice == 6) {
            // Filter by price range
            printf("\nEnter minimum price (or 0 for no minimum): ");
            double min = safe_read_double_range(0, 100000);
            if (min > 0) {
                criteria.priceMin = min;
                criteria.hasPriceMin = true;
            }
            printf("Enter maximum price (or 0 for no maximum): ");
            double max = safe_read_double_range(0, 100000);
            if (max > 0) {
                criteria.priceMax = max;
                criteria.hasPriceMax = true;
            }
            search_build_and_execute(inv, sdb, criteria);
        }

    } while (choice != 0);
}

// SUPPLIER MENU
void show_suppliers_menu(SuppliersDB* sdb) {
    int choice;
    do {
        printf("\n\n");
        big_line();
        printf("                  SUPPLIER RANKINGS\n");
        big_line();
        suppliers_print_ranked(sdb);
        print_line();

        printf("\nOptions:\n");
        printf("  1. Add New Supplier\n");
        printf("  2. Remove Supplier\n");
        printf("  3. Find Supplier by ID\n");
        printf("  4. Show High-Rated Suppliers\n");
        printf("  0. Back\n");
        choice = read_menu_choice(0, 4);

        if (choice == 1) {
            Supplier s = {0};
            printf("\nEnter Supplier Details:\n");
            printf("  ID (1–9999): "); s.id = safe_read_int_range(1, 9999);
            printf("  Name: "); scanf(" %63[^\n]", s.name);
            printf("  Quality (0–10): "); s.ratings.quality = safe_read_double_range(0, 10);
            printf("  Delivery Time (0–10): "); s.ratings.deliveryTime = safe_read_double_range(0, 10);
            printf("  Price (0–10): "); s.ratings.price = safe_read_double_range(0, 10);
            printf("  Reliability (0–10): "); s.ratings.reliability = safe_read_double_range(0, 10);
            printf("  Customer Service (0–10): "); s.ratings.customerService = safe_read_double_range(0, 10);

            suppliers_insert(sdb, s);
            printf("\n✅ Supplier added successfully!\n");
        }
        else if (choice == 2) {
            printf("\nEnter Supplier ID to remove: ");
            int id = safe_read_int_range(1, 9999);
            if (suppliers_delete(sdb, id))
                printf("\n✅ Supplier removed.\n");
            else
                printf("\n❌ Supplier not found.\n");
        }
        else if (choice == 3) {
            printf("\nEnter Supplier ID to find: ");
            int id = safe_read_int_range(1, 9999);
            Supplier* f = suppliers_find_by_id(sdb, id);
            if (f)
                printf("\n✅ %s (Score: %.2f)\n", f->name, supplier_overall_score(&f->ratings));
            else
                printf("\n❌ Supplier not found.\n");
        }
        else if (choice == 4) {
            double minRating = DBL_MAX, maxRating = -DBL_MAX;
            for (int i = 1; i <= 9999; i++) {
                Supplier* s = suppliers_find_by_id(sdb, i);
                if (s) {
                    double sc = supplier_overall_score(&s->ratings);
                    if (sc < minRating) minRating = sc;
                    if (sc > maxRating) maxRating = sc;
                }
            }
            if (minRating == DBL_MAX) {
                printf("\nNo suppliers found.\n");
                continue;
            }
            printf("\nEnter minimum rating (%.2f–%.2f): ", minRating, maxRating);
            double minOverall = safe_read_double_range(minRating, maxRating);
            suppliers_print_min_rating(sdb, minOverall);
        }

    } while (choice != 0);
}

// ORDERS MENU
void show_orders_menu(OrdersQueue* oq, Inventory* inv) {
    int choice;
    do {
        printf("\n\n");
        big_line();
        printf("                    ORDER MANAGEMENT\n");
        big_line();
        orders_print(oq);
        print_line();

        printf("\nOptions:\n");
        printf("  1. Place New Order\n");
        printf("  2. Process Next Order\n");
        printf("  3. Process All Orders\n");
        printf("  0. Back\n");
        choice = read_menu_choice(0, 3);

        if (choice == 1) {
            Order o = {0};
            printf("\nEnter Order Details:\n");
            printf("  Order ID: "); o.id = safe_read_int_range(1, 9999);
            printf("  Customer Name: "); scanf(" %63[^\n]", o.customer);
            printf("  Number of items (1–%d): ", MAX_ORDER_ITEMS);
            o.numItems = safe_read_int_range(1, MAX_ORDER_ITEMS);

            for (int i = 0; i < o.numItems; i++) {
                printf("\n  Item %d\n", i + 1);
                printf("    Product ID: "); o.items[i].productId = safe_read_int_range(1, 9999);
                Product* p = inventory_get_product(inv, o.items[i].productId);
                if (!p) {
                    printf("❌ Product not found! Try again.\n");
                    i--;
                    continue;
                }
                printf("    Quantity: "); o.items[i].quantity = safe_read_int_range(1, 1000);
            }
            orders_enqueue(oq, o);
            printf("\n✅ Order added successfully!\n");
        }
        else if (choice == 2) {
            orders_process_next(oq, inv);
        }
        else if (choice == 3) {
            int count = 0;
            while (orders_count(oq) > 0) {
                if (orders_process_next(oq, inv)) count++;
            }
            printf("\n✅ Processed %d orders.\n", count);
        }

    } while (choice != 0);
}

// HARD CODED SAMPLE DATA
void add_sample_data(Inventory* inv, SuppliersDB* sdb, OrdersQueue* oq) {
    Product products[] = {
        {101, "Laptop", "Electronics", 1, 999.99, 15},
        {102, "Mouse", "Electronics", 1, 25.50, 40},
        {103, "Notebook", "Stationery", 2, 4.99, 100},
        {104, "Pen", "Stationery", 2, 1.99, 200},
        {105, "Coffee", "Beverages", 3, 12.99, 50},
        {106, "Tea", "Beverages", 3, 10.99, 60},
        {107, "Monitor", "Electronics", 1, 180.00, 30},
        {108, "Desk Chair", "Furniture", 4, 89.99, 20},
        {109, "Printer", "Electronics", 1, 149.99, 25},
        {110, "Paper Pack", "Stationery", 2, 8.99, 150}
    };
    for (int i = 0; i < 10; i++) inventory_add_product(inv, products[i]);

    Supplier suppliers[] = {
        {1, "Tech Supplies Inc", {8.5, 7.0, 9.0, 8.0, 7.5}},
        {2, "Office World", {9.0, 8.5, 7.5, 9.5, 8.0}},
        {3, "Beverage Co", {8.0, 9.0, 8.5, 7.5, 8.5}},
        {4, "Home Comforts", {7.5, 8.0, 8.0, 7.0, 7.5}},
        {5, "Eco Friendly Ltd", {8.0, 8.0, 8.5, 8.5, 9.0}},
        {6, "Printify", {7.0, 7.5, 8.0, 7.5, 7.0}},
        {7, "FoodMart", {9.5, 8.0, 9.0, 8.5, 9.5}}
    };
    for (int i = 0; i < 7; i++) suppliers_insert(sdb, suppliers[i]);

    Order orders[] = {
        {1, "John Doe", 2, {{101, 1}, {102, 2}}},
        {2, "Jane Smith", 1, {{103, 5}}},
        {3, "Ravi Kumar", 1, {{107, 1}}},
        {4, "Anita R.", 2, {{108, 1}, {104, 3}}},
        {5, "Michael Chen", 1, {{110, 2}}}
    };
    for (int i = 0; i < 5; i++) orders_enqueue(oq, orders[i]);
}

// MAIN
int main() {
    Inventory* inv = inventory_create();
    SuppliersDB* sdb = suppliers_create();
    OrdersQueue* oq = orders_create();
    add_sample_data(inv, sdb, oq);

    int choice;
    do {
        printf("\n\n");
        big_line();
        printf("       SUPPLY CHAIN MANAGEMENT SYSTEM (v%s)\n", VERSION);
        big_line();
        printf("\nMain Menu:\n");
        printf("  1. Inventory Management\n");
        printf("  2. Search Products\n");
        printf("  3. Supplier Rankings\n");
        printf("  4. Order Management\n");
        printf("  0. Exit\n");
        choice = read_menu_choice(0, 4);

        if (choice == 1) show_inventory_menu(inv);
        else if (choice == 2) show_search_menu(inv, sdb);
        else if (choice == 3) show_suppliers_menu(sdb);
        else if (choice == 4) show_orders_menu(oq, inv);

    } while (choice != 0);

    printf("\nThank you for using Supply Chain Manager!\n\n");
    orders_destroy(oq);
    suppliers_destroy(sdb);
    inventory_destroy(inv);
    return 0;
}
