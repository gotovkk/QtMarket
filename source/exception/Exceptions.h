#pragma once
#include <stdexcept>
#include <QString>
#include "../../sqlite/sqlite3.h"

class Exceptions : public std::runtime_error {
public:
    explicit Exceptions(const QString &message) : std::runtime_error(message.toStdString()) {}
};

class UnauthorizedException : public std::runtime_error {
public:
    explicit UnauthorizedException(const std::string &message) : std::runtime_error(message) {}
};


class SQLException : public std::runtime_error {
public:
    explicit SQLException(const std::string &message) : std::runtime_error(message) {}
};

class SqlErrorHandler {
public:
    explicit SqlErrorHandler(sqlite3 *database) : db(database) {}

    void handleError(const std::string &errorMessage) const {
        std::string fullMessage = errorMessage + ": " + sqlite3_errmsg(db);
        throw std::runtime_error(fullMessage);
    }

private:
    sqlite3 *db;
};
