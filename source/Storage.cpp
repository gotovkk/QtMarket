#pragma once

#include "../include/Storage.h"

using namespace std;

void Storage::allProductsList() const {
    for (const auto &product: products) {
        cout << "----------------------" << endl;
        product->printInfo();
    }
    cout << "----------------------" << endl;
}

void showProducts(const Storage &storage) {
    cout << "Список предметов:" << endl;
    storage.allProductsList();
}

void Storage::findLowStockProducts(Storage &storage) const {
    int limit;
    cout << "Введите лимит для низкого запаса: ";
    cin >> limit;

    cout << "Товары с низким запасом:" << endl;
    for (const auto &product: storage.productsList()) {
        if (product->getAmount() <= limit) {
            product->printInfo();
        }
    }
}


void amountChange(const Storage &storage, std::string_view name) {
    int operationChoice;
    std::cout << "Выберите операцию (1 - добавить, 2 - уменьшить): ";
    std::cin >> operationChoice;

    int amount;
    std::cout << "Введите количество для обновления: ";
    std::cin >> amount;

    auto &products = storage.productsList();
    for (const auto &product: products) {
        if (product->getName() == name) {
            if (operationChoice == 1) {
                *product += amount;
            } else if (operationChoice == 2) {
                *product -= amount;
            }
            break;
        }
    }
}

Product const *Storage::findProductByName(std::string_view productName) const {
    for (const auto &product: products) {
        if (product->getName() == productName) {
            return product.get();
        }
    }
    return nullptr;
}


void productsComprassion(std::string &firstProductName, std::string &secondProductName, const Product *&first,
                         Storage &storage, const Product *&second) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введите имя первого продукта для сравнения: ";
    std::getline(std::cin, firstProductName);

    std::cout << "Введите имя второго продукта для сравнения: ";
    std::getline(std::cin, secondProductName);

    first = storage.findProductByName(firstProductName);
    second = storage.findProductByName(secondProductName);

    if (first && second) {
        comprasionCheck(first, second);
    } else {
        if (!first) {
            std::cout << "Продукт \"" << firstProductName << "\" не найден." << std::endl;
        }
        if (!second) {
            std::cout << "Продукт \"" << secondProductName << "\" не найден." << std::endl;
        }
    }
}
