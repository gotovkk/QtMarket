#include "SellerAuth.h"
#include "SessionManager.h"
#include "../../include/ui/sellermenu.h"

bool SellerAuth::registerSeller(sqlite3 *db, const std::string &username, const std::string &password,
                                const std::string &email, const std::string &phone_num, int &seller_id) const {

    const char *sqlCheckTable = "SELECT name FROM sqlite_master WHERE type='table' AND name='sellers';";
    sqlite3_stmt *stmtCheckTable;

    if (sqlite3_prepare_v2(db, sqlCheckTable, -1, &stmtCheckTable, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmtCheckTable) != SQLITE_ROW) {
            const char *sqlCreateTable = "CREATE TABLE sellers ("
                                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "username TEXT NOT NULL UNIQUE, "
                                         "password_hash TEXT NOT NULL, "
                                         "email TEXT NOT NULL, "
                                         "phone_num TEXT NOT NULL, "
                                         "reg_date DATETIME DEFAULT CURRENT_TIMESTAMP"
                                         ");";

            char *errMsg = nullptr;
            if (int rc = sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, &errMsg); rc != SQLITE_OK) {
                qDebug() << "Ошибка при создании таблицы sellers: " << errMsg;
                sqlite3_free(errMsg);
                sqlite3_finalize(stmtCheckTable);
                return false;
            }
        }
    } else {
        qDebug() << "Ошибка проверки существования таблицы: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmtCheckTable);
        return false;
    }
    sqlite3_finalize(stmtCheckTable);

    std::string sqlSelect = "SELECT COUNT(*) FROM sellers WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            qDebug() << "Пользователь с таким логином уже существует!";
            sqlite3_finalize(stmt);
            return false;
        }
    } else {
        qDebug() << "Ошибка подготовки SQL-запроса SELECT: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    std::string sqlInsert = "INSERT INTO sellers (username, password_hash, email, phone_num) VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phone_num.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            qDebug() << "Ошибка регистрации пользователя!";
            sqlite3_finalize(stmt);
            return false;
        }

        seller_id = static_cast<int>(sqlite3_last_insert_rowid(db));
        sqlite3_finalize(stmt);

    } else {
        qDebug() << "Ошибка подготовки SQL-запроса INSERT: " << sqlite3_errmsg(db);
        return false;
    }

    sqlite3_finalize(stmt);
    qDebug() << "Регистрация успешна! seller_id: " << seller_id;
    return true;
}


bool SellerAuth::login(sqlite3 *db, const std::string &username, const std::string &password) const {
    std::string sqlSelect = "SELECT id, password_hash FROM sellers WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса: " << sqlite3_errmsg(db);
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка при подготовке SQL-запроса.");
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    bool loginSuccess = false;

    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW) {
        const char *storedPassword = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        if (storedPassword) {
            int sellerId = sqlite3_column_int(stmt, 0);

            qDebug() << "Извлеченный пароль: " << storedPassword;
            if (std::string(storedPassword) == password) {
                int sellerId = sqlite3_column_int(stmt, 0);

                SessionManager::setCurrentUserId(sellerId);

                qDebug() << "Пользователь успешно вошел. ID продавца: " << sellerId;

                sqlite3_finalize(stmt);

                return true;
            } else {
                qDebug() << "Неправильный пароль!";
                QMessageBox::warning(nullptr, "Ошибка", "Неправильный пароль!");
            }
        } else {
            qDebug() << "Ошибка: пароль не найден!";
            QMessageBox::warning(nullptr, "Ошибка", "Пароль не найден!");
        }
    } else if (stepResult == SQLITE_DONE) {
        qDebug() << "Пользователь с таким именем не найден.";
        QMessageBox::warning(nullptr, "Ошибка", "Пользователь не найден!");
    } else {
        qDebug() << "Ошибка при выполнении запроса: " << sqlite3_errmsg(db);
        QMessageBox::warning(nullptr, "Ошибка", "Ошибка при выполнении запроса.");
    }

    sqlite3_finalize(stmt);

    if (!loginSuccess) {
        SessionManager::logout();
    }

    return loginSuccess;
}