#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include "User.h"
#include "../../source/management/ProductManagement.h"

class Admin : public User, public ProductManagement {
private:
    int sellerId = NULL;
public:
    Admin(
            const std::string& username,
            const std::string& password,
            int sellerId
    ) : User(username, password), sellerId(sellerId) {}

    [[nodiscard]] bool login(const std::string& passwordInput) const override {
        return password == passwordInput;
    }

    void logout() override {
        std::cout << "Администратор " << getUsername() << " вышел." << std::endl;
    }

    [[maybe_unused]] void approveSeller(
            [[maybe_unused]] const Storage& storage,
            [[maybe_unused]] int sellerId
    ) const;



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
        std::cout << "Введите название продукта для удаления от имени администратора: ";
        std::cin >> name;
        auto& products = storage.productsList();
        auto iter = std::ranges::find_if(products.begin(), products.end(),
                                 [&name, sellerId](const auto& product) {
                                     return product->getName() == name && product->getSellerId() == sellerId;
                                 });

        if (iter != products.end()) {
            products.erase(iter);
            return true;
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

};

#endif // ADMIN_H
