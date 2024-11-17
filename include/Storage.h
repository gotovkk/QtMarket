#pragma once

#include <vector>
#include <string>
#include <variant>
#include <format>
#include <memory>
#include "Product.h"
#include "../sqlite/sqlite3.h"


enum class Field {
    Color, Price, Weight, Amount
};

class Storage {
private:
    std::vector<std::unique_ptr<Product>> products;
public:
    const std::vector<std::unique_ptr<Product>> &productsList() const {
        return products;
    }

    std::vector<std::unique_ptr<Product>> &productsList() {
        return products;
    }

    void allProductsList() const;

    void findLowStockProducts(Storage &storage) const;

    Product const *findProductByName(std::string_view productName) const;

};

void amountChange(const Storage &storage, std::string_view name);

void showProducts(const Storage &storage);

void productsComprassion(std::string &firstProductName, std::string &secondProductName, const Product *&first,
                         Storage &storage, const Product *&second);


