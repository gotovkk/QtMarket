#pragma once
#include "../../sqlite/sqlite3.h"
#include "../exception/Exceptions.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>


class BuyerAuth {
public:
    bool registerBuyer(sqlite3 *db,
        const std::string &name,
        const std::string &password,
        const std::string &email,
        int &buyer_id) const;

    bool login(
            sqlite3 *db,
            const std::string &name,
            const std::string &password
    ) const;
};

