#include "DatabaseLoader.h"
using namespace std;

void DatabaseLoader::loadProductsFromDb(sqlite3* db, Storage& storage, ProductManagement& productManager) {
    const char* selectFrom = R"(
            SELECT id, name, description, price, amount, seller_id, category_id, added_date
            FROM products;
        )";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectFrom, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double price = sqlite3_column_double(stmt, 3);
            int amount = sqlite3_column_int(stmt, 4);
            int seller_id = sqlite3_column_int(stmt, 5);
            int category_id = sqlite3_column_int(stmt, 6);
            std::string added_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

            auto product = std::make_unique<Product>(id, name, description, price, amount, seller_id, category_id, added_date);
            productManager.addProduct(storage, std::move(product));
        }
    } else {
        std::cerr << "Ошибка: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void DatabaseLoader::addToTable(Storage& storage, sqlite3* db, int seller_id) {
    std::string name;
    std::string description;
    double price;
    int amount;
    int category_id;

    std::cout << "Введите название продукта: ";
    std::cin >> name;
    std::cout << "Введите описание продукта: ";
    std::cin.ignore();
    std::getline(std::cin, description);
    std::cout << "Введите цену: ";
    std::cin >> price;
    std::cout << "Введите количество: ";
    std::cin >> amount;
    std::cout << "Введите ID категории: ";
    std::cin >> category_id;

    // Получение текущего времени
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Форматирование времени в строку
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    std::string added_date = ss.str();

    std::string sqlInsert = std::format(
            "INSERT INTO products (name, description, price, seller_id, category_id, amount, added_date) VALUES ('{}', '{}', {}, {}, {}, {}, '{}');",
            name, description, price, seller_id, category_id, amount, added_date
    );

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sqlInsert.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка добавления продукта в базу данных: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Продукт успешно добавлен в базу данных." << std::endl;
    }
}