#pragma once

#include <iostream>
#include "../../sqlite/sqlite3.h"
#include <string>
#include <QDebug>
#include "../manager/ErrorManager.h"

class SellerAuth {

public:
    bool registerSeller(
            sqlite3 *db,
            const std::string &username,
            const std::string &password,
            const std::string &email,
            const std::string &phone_num,
            int &seller_id
    ) const;

    bool login(sqlite3 *db, const std::string &username, const std::string_view &password) const;

private:

};

