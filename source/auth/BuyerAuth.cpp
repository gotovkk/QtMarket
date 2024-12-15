#include "BuyerAuth.h"
#include "SessionManager.h"

bool BuyerAuth::registerBuyer(sqlite3 *db, const std::string &name, const std::string &password, const std::string &email,
                              int &buyer_id) const {
    SqlErrorHandler errorHandler(db);

    try {
        const char *sqlCheckTable = "SELECT name FROM sqlite_master WHERE type='table' AND name='buyers';";
        sqlite3_stmt *stmtCheckTable;

        if (sqlite3_prepare_v2(db, sqlCheckTable, -1, &stmtCheckTable, nullptr) != SQLITE_OK) {
            errorHandler.handleError("Ошибка проверки существования таблицы buyers");
        }

        if (sqlite3_step(stmtCheckTable) != SQLITE_ROW) {
            const char *sqlCreateTable = "CREATE TABLE buyers ("
                                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "name TEXT NOT NULL UNIQUE, "
                                         "password_hash TEXT NOT NULL, "
                                         "email TEXT NOT NULL, "
                                         "reg_date DATETIME DEFAULT CURRENT_TIMESTAMP"
                                         ");";

            char *errMsg = nullptr;
            if (sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
                std::string err = "Ошибка при создании таблицы buyers: ";
                err += errMsg;
                sqlite3_free(errMsg);
                sqlite3_finalize(stmtCheckTable);
                throw SQLException(err);
            }
        }
        sqlite3_finalize(stmtCheckTable);

        std::string sqlSelect = "SELECT COUNT(*) FROM buyers WHERE name = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            errorHandler.handleError("Ошибка подготовки SQL-запроса SELECT");
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            throw SQLException("Покупатель с таким именем уже существует!");
        }
        sqlite3_finalize(stmt);

        std::string sqlInsert = "INSERT INTO buyers (name, password_hash, email) VALUES (?, ?, ?);";

        if (sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            errorHandler.handleError("Ошибка подготовки SQL-запроса INSERT");
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            errorHandler.handleError("Ошибка регистрации покупателя");
        }

        buyer_id = static_cast<int>(sqlite3_last_insert_rowid(db));
        sqlite3_finalize(stmt);

        qDebug() << "Регистрация успешна! buyer_id: " << buyer_id;
        return true;

    } catch (const SQLException &ex) {
        qDebug() << "Ошибка SQL: " << ex.what();
        return false;
    } catch (const std::exception &ex) {
        qDebug() << "Общая ошибка: " << ex.what();
        return false;
    }
}

bool BuyerAuth::login(sqlite3 *db, const std::string &name, const std::string &password) const {
    SqlErrorHandler errorHandler(db);

    try {
        std::string sqlSelect = "SELECT id, password_hash FROM buyers WHERE name = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            errorHandler.handleError("Ошибка при подготовке SQL-запроса");
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *storedPassword = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            if (!storedPassword || std::string(storedPassword) != password) {
                sqlite3_finalize(stmt);
                throw UnauthorizedException("Неправильный пароль или покупатель не найден");
            }

            int buyerId = sqlite3_column_int(stmt, 0);

            SessionManager::setCurrentUserId(buyerId);

            sqlite3_finalize(stmt);

            qDebug() << "Вход выполнен успешно!";

            return true;
        } else {
            sqlite3_finalize(stmt);
            throw UnauthorizedException("Покупатель не найден");
        }

    } catch (const UnauthorizedException &ex) {
        qDebug() << "Ошибка авторизации: " << ex.what();
        QMessageBox::warning(nullptr, "Ошибка авторизации", QString::fromStdString(ex.what()));
        return false;
    } catch (const SQLException &ex) {
        qDebug() << "Ошибка SQL: " << ex.what();
        QMessageBox::critical(nullptr, "Ошибка базы данных", QString::fromStdString(ex.what()));
        return false;
    } catch (const std::exception &ex) {
        qDebug() << "Общая ошибка: " << ex.what();
        QMessageBox::critical(nullptr, "Ошибка", QString::fromStdString(ex.what()));
        return false;
    }
}
