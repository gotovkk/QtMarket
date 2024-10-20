#pragma once
#include <string>
#include <iostream>
#include "../Storage.h"

#include <string>

class User {
protected:
    std::string username;
    std::string password;

public:
    User() : username(""), password("") {}

    User(const std::string& username, const std::string& password)
            : username(username), password(password) {}

    virtual bool login(const std::string& passwordInput) const = 0;
    virtual void logout() = 0;

    const std::string& getUsername() const {
        return username;
    }

    virtual ~User() = default;
};
