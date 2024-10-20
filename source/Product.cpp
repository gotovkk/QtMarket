#include "../include/Product.h"

double Product::getPrice() const { return price; }
int Product::getAmount() const { return amount; }
std::string Product::getName() const { return name; }
std::string Product::getDescription() const { return description; }
int Product::getSellerId() const { return seller_id; }
int Product::getCategoryId() const { return category_id; }
int Product::getId() const { return id; }
std::string Product::getAddedDate() const { return added_date; }

void Product::setPrice(double newPrice) { price = newPrice; }
void Product::setSellerId(int newId) { seller_id = newId; }
void Product::setAmount(int newAmount) { amount = newAmount; }

void Product::printInfo() const {
    std::cout << "ID: " << id << "\n"
              << "Название: " << name << "\n"
              << "Описание: " << description << "\n"
              << "Цена: " << price << "\n"
              << "Количество: " << amount << "\n"
              << "ID продавца: " << seller_id << "\n"
              << "ID категории: " << category_id << "\n"
              << "Дата добавления: " << added_date << std::endl;
}

Product& Product::operator+=(int amountToAdd) {
    this->amount += amountToAdd;
    return *this;
}

Product& Product::operator-=(int amountToSubtract) {
    if (this->amount >= amountToSubtract) {
        this->amount -= amountToSubtract;
    }
    else {
        this->amount = 0;
    }
    return *this;
}

void comprasionCheck(const Product* first, const Product* second)
{
    if (first && second) {
        std::cout << "Результат сравнения товаров:\n";
        if (*first == *second) {
            std::cout << "Продукты совпадают" << std::endl;
        }
        else {
            std::cout << "Продукты имеют различие" << std::endl;
        }
    }
    else {
        std::cout << "Один или оба продукта не найдены\n";
    }
}



