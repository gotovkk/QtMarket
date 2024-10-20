#include "ui_buyermenu.h"
#include "../../include/ui/buyermenu.h"
#include <iostream>

BuyerMenu::BuyerMenu(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::BuyerMenu) {
    ui->setupUi(this);

    QWidget *container = new QWidget();
    layout = new QVBoxLayout(container);

    container->setLayout(layout);

    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);

    setupDatabase();
    loadProducts(db);
}

BuyerMenu::~BuyerMenu() {
    delete ui;
}

void BuyerMenu::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных(3): " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта(3).";
    }
}

void BuyerMenu::on_logOutButton_clicked() {
    this->close();
    emit secondWindow();
}

void BuyerMenu::loadProducts(sqlite3 *db) {
    std::string sqlSelect = "SELECT id, name, description, price, amount, seller_id, category_id, added_date FROM products;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса: " << sqlite3_errmsg(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int price = sqlite3_column_int(stmt, 3);
        int amount = sqlite3_column_int(stmt, 4);
        int seller_id = sqlite3_column_int(stmt, 5);
        int category_id = sqlite3_column_int(stmt, 6);
        std::string added_date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));

        Product product(id, name, description, price, amount, seller_id, category_id, added_date);

        auto *productWidget = new ProductItemWidget(product);
        layout->addWidget(productWidget);
    }

    sqlite3_finalize(stmt);
}