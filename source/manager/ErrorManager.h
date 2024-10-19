#pragma once
#include "../../sqlite/sqlite3.h"
#include <iostream>

class ErrorManager
{
public:
    static void reportError(const std::string& msg, sqlite3* db);
    static void loginError();
    static void choiceError();

};

