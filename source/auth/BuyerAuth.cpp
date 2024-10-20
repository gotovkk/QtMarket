#include "BuyerAuth.h"

bool BuyerAuth::registerBuyer(
        sqlite3 *db, const std::string &name, const std::string &password,
        const std::string &email, int &buyer_id) const {

    const char *sqlCheckTable = "SELECT name FROM sqlite_master WHERE type='table' AND name='buyers';";
    sqlite3_stmt *stmtCheckTable;

    if (sqlite3_prepare_v2(db, sqlCheckTable, -1, &stmtCheckTable, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmtCheckTable) != SQLITE_ROW) {
            const char *sqlCreateTable =
                    "CREATE TABLE buyers ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL UNIQUE, "
                    "password_hash TEXT NOT NULL, "
                    "email TEXT NOT NULL, "
                    "reg_date DATETIME DEFAULT CURRENT_TIMESTAMP"
                    ");";

            char *errMsg = nullptr;
            if (int rc = sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, &errMsg); rc != SQLITE_OK) {
                qDebug() << "Ошибка при создании таблицы buyers: " << errMsg;
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

    std::string sqlSelect = "SELECT COUNT(*) FROM buyers WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            qDebug() << "Покупатель с таким именем уже существует!";
            sqlite3_finalize(stmt);
            return false;
        }
    } else {
        qDebug() << "Ошибка подготовки SQL-запроса SELECT: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    std::string sqlInsert = "INSERT INTO buyers (name, password_hash, email) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            qDebug() << "Ошибка регистрации покупателя!";
            sqlite3_finalize(stmt);
            return false;
        }

        buyer_id = static_cast<int>(sqlite3_last_insert_rowid(db));
    } else {
        qDebug() << "Ошибка подготовки SQL-запроса INSERT: " << sqlite3_errmsg(db);
        return false;
    }

    sqlite3_finalize(stmt);
    qDebug() << "Регистрация успешна! buyer_id: " << buyer_id;
    return true;
}

bool BuyerAuth::login(sqlite3 *db, const std::string &name, const std::string &password) const {
    std::string sqlSelect = "SELECT password_hash FROM buyers WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса: " << sqlite3_errmsg(db);
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка при подготовке SQL-запроса.");
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    bool loginSuccess = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *storedPassword = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        if (storedPassword) {
            if (std::string(storedPassword) == password) {
                qDebug() << "Вход выполнен успешно!";
                loginSuccess = true;
            } else {
                qDebug() << "Неправильный пароль!";
                QMessageBox::warning(nullptr, "Ошибка", "Неправильный пароль!");
            }
        } else {
            qDebug() << "Ошибка: пароль не найден!";
            QMessageBox::warning(nullptr, "Ошибка", "Пароль не найден!");
        }
    } else {
        qDebug() << "Покупатель не найден: " << sqlite3_errmsg(db);
        QMessageBox::warning(nullptr, "Ошибка", "Покупатель не найден!");
    }

    sqlite3_finalize(stmt);
    return loginSuccess;
}