#pragma once
#include "User.h"
#include "../../source/management/CartManagement.h"

class Buyer : public User, public CartManagement
{
public:
    Buyer(const std::string& username, const std::string& password, int buyerId)
            : User(username, password), buyerId(buyerId) {}

    [[nodiscard]] bool login(const std::string& passwordInput) const override {
        return password == passwordInput;
    }

    void logout() override {
        std::cout << "Покупатель" << username << " вышел." << std::endl;
    }
private:
    int buyerId;
};

