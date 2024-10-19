#include "ErrorManager.h"

void ErrorManager::reportError(const std::string& msg, sqlite3* db) {
    std::cerr << msg << ": " << (db ? sqlite3_errmsg(db) : "неизвестная ошибка") << std::endl;
}

void ErrorManager::loginError()
{
    std::cerr << "Ошибка входа. Проверьте введенный логин или пароль.\n" << std::endl;
}

void ErrorManager::choiceError()
{
    std::cerr << "Неверный выбор, попробуйте снова.\n" << std::endl;

}
