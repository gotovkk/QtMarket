#ifndef SELLER_H  // Проверка, был ли определён SELLER_H
#define SELLER_H  // Определение SELLER_H

#include <iostream>
#include <string>
#include "User.h"
#include "../../source/management/ProductManagement.h"

class Seller : public User, public ProductManagement {
private:
    int sellerId = NULL;

public:
    Seller(
            const std::string& username,
            const std::string& password,
            int sellerId
    ) : User(username, password), sellerId(sellerId) {}


    bool login(const std::string& passwordInput) const override {
        return password == passwordInput;
    }
    void logout() override {
        std::cout << "Продавец " << User::getUsername() << " вышел." << std::endl;
    }



    void addProduct(
            Storage& storage,
            std::unique_ptr<Product> product
    ) const override {
        for (const auto& existProduct : storage.productsList()) {
            if (existProduct->getName() == product->getName() && existProduct->getSellerId() == product->getSellerId()) {
                int newAmount = existProduct->getAmount() + product->getAmount();
                existProduct->setAmount(newAmount);
                return;
            }
        }
        storage.productsList().push_back(std::move(product));

    }

    bool removeProduct(
            Storage& storage,
            int sellerId
    ) const override {
        std::string name;
        std::cout << "Введите название продукта для удаления: ";
        std::cin >> name;
        auto& products = storage.productsList();
        for (auto iter = products.begin(); iter != products.end(); iter++) {
            if ((*iter)->getName() == name && (*iter)->getSellerId() == sellerId) {
                products.erase(iter);
                return true;
            }
        }
        std::cout << "Товар не найден " << std::endl;
        return false;
    }

    void updateProduct(
            Storage& storage,
            int sellerId
    ) const override {
        std::string name;
        std::cout << "Введите название продукта для обновления: ";
        std::cin >> name;

        std::cout << "Выберите поле для обновления:\n"
                  << "1 - Цвет\n"
                  << "2 - Цена\n"
                  << "3 - Количество\n"
                  << "Ваш выбор: ";

        int fieldChoice;
        std::cin >> fieldChoice;

        std::variant<double, std::string> value;
        switch (fieldChoice) {
            case 1: {
                std::string color;
                std::cout << "Введите новый цвет: ";
                std::cin >> color;
                value = color;
                break;
            }
            case 2: {
                double price;
                std::cout << "Введите новую цену: ";
                std::cin >> price;
                value = price;
                break;
            }
            case 3: {
                double amount;
                std::cout << "Введите новое количество: ";
                std::cin >> amount;
                value = amount;
                break;
            }
            default:
                std::cout << "Некорректный выбор. Попробуйте снова." << std::endl;
                return;
        }

        for (const auto& product : storage.productsList()) {
            if (product->getName() == name && product->getSellerId() == sellerId) {
                switch (fieldChoice) {
                    case 1:
                        product->setPrice(std::get<double>(value));
                        break;
                    case 2:
                        product->setAmount(static_cast<int>(std::get<double>(value)));
                        break;
                    default:
                        std::cout << "Некорректное поле для обновления." << std::endl;
                        return;
                }

                std::cout << "Продукт обновлен успешно." << std::endl;
                return;
            }
        }

        std::cout << "Продукт с названием \"" << name << "\" не найден или не принадлежит текущему продавцу." << std::endl;
    }


    int getSellerId() const;

};

#endif // SELLER_H
