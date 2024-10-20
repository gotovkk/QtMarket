#pragma once
#include "../../sqlite/sqlite3.h"
#include "../../include/Storage.h"
#include "../management/ProductManagement.h"
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>


class DatabaseLoader {
public:
    void loadProductsFromDb(sqlite3 *db, Storage &storage, ProductManagement &productManager);

    void addToTable(
            Storage &storage, sqlite3 *db, int seller_id,
            const std::string &name, const std::string &description,
            double price, int amount, int category_id
    );
};
