#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../sqlite/sqlite3.h"


class Product {
    friend bool operator==(const Product& lhs, const Product& rhs);

private:
    int id;
    std::string name;
    std::string description;
    double price;
    int amount;
    int seller_id;
    int category_id;
    std::string added_date;

public:
    Product(int id, const std::string& name, const std::string& description, double price, int amount, int seller_id,
            int category_id, const std::string& added_date)
            : id(id), name(name), description(description), price(price), amount(amount),
              seller_id(seller_id), category_id(category_id), added_date(added_date) {}

    double getPrice() const;
    int getAmount() const;
    std::string getName() const;
    std::string getDescription() const;
    int getSellerId() const;
    int getCategoryId() const;
    int getId() const;
    std::string getAddedDate() const;
    void setPrice(double newPrice);
    void setSellerId(int newId);
    void setAmount(int newAmount);
    void printInfo() const;

    Product& operator+=(int amount);
    Product& operator-=(int amount);

    friend bool operator==(const Product& lhs, const Product& rhs) {
        bool isEqual = true;

        std::vector<std::pair<std::string, bool>> comparison = {
                {"ID", lhs.id == rhs.id},
                {"Название", lhs.name == rhs.name},
                {"Описание", lhs.description == rhs.description},
                {"Цена", lhs.price == rhs.price},
                {"ID продавца", lhs.seller_id == rhs.seller_id},
                {"ID категории", lhs.category_id == rhs.category_id},
                {"Количество", lhs.amount == rhs.amount},
        };

        std::cout << "Сравнение характеристик продуктов:\n";

        for (const auto& [field, result] : comparison) {
            if (result) {
                std::cout << field << ": совпадает" << std::endl;
            } else {
                if (field == "Цена" || field == "Количество") {
                    std::cout << field << ": " << lhs.name << " (" << lhs.price << ") != " << rhs.name << " (" << rhs.price << ")" << std::endl;
                } else {
                    std::cout << field << ": " << lhs.name << " != " << rhs.name << std::endl;
                }
                isEqual = false;
            }
        }

        return isEqual;
    }
};

void comprasionCheck(const Product* first, const Product* second);
