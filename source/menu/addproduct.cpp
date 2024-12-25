#include "ui_addproduct.h"
#include "../../include/ui/addproduct.h"
#include "../auth/SessionManager.h"

#include <iostream>


AddProduct::AddProduct(QWidget *parent) : QWidget(parent), ui(new Ui::AddProduct) {
    ui->setupUi(this);
    setupDatabase();
    ui->productLabel->setVisible(false);
}

AddProduct::~AddProduct() {
    delete ui;
}

void AddProduct::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта.";
    }
}

void AddProduct::on_backButton_clicked() {
    this->close();
    emit addProductWindow();
}

void AddProduct::on_addProductButton_clicked() {
    QString name = ui->lineEditName->text();
    QString description = ui->lineEditDescription->text();
    double price = ui->lineEditPrice->text().toDouble();
    int amount = ui->lineEditAmount->text().toInt();
    int category_id = ui->lineEditCategory->text().toInt();
    int seller_id = SessionManager::getCurrentUserId();

    if (currentProductId == -1) {
        databaseLoader.addToTable(storage, db, seller_id,
                                  name.toStdString(), description.toStdString(),
                                  price, amount, category_id);
        qDebug() << "Новый продукт добавлен.";
    } else {
        std::string sqlUpdate = "UPDATE products SET name = ?, description = ?, price = ?, amount = ?, category_id = ? WHERE id = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sqlUpdate.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            qDebug() << "Ошибка при подготовке SQL-запроса для обновления: " << sqlite3_errmsg(db);
            return;
        }

        sqlite3_bind_text(stmt, 1, name.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, description.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, price);
        sqlite3_bind_int(stmt, 4, amount);
        sqlite3_bind_int(stmt, 5, category_id);
        sqlite3_bind_int(stmt, 6, currentProductId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            qDebug() << "Продукт с ID " << currentProductId << " успешно обновлен.";
        } else {
            qDebug() << "Ошибка при обновлении продукта: " << sqlite3_errmsg(db);
        }

        sqlite3_finalize(stmt);
    }

    ui->lineEditName->clear();
    ui->lineEditDescription->clear();
    ui->lineEditPrice->clear();
    ui->lineEditAmount->clear();
    ui->lineEditCategory->clear();

    ui->addProductButton->setText("Add Product");
    ui->productLabel->setVisible(true);

    QTimer::singleShot(1000, this, [this]() {
        ui->productLabel->setVisible(false);
    });

    currentProductId = -1;
    emit addProductWindow();
}

void AddProduct::populateFields(const Product &product) {
    currentProductId = product.getId();

    ui->lineEditName->setText(QString::fromStdString(product.getName()));
    ui->lineEditDescription->setText(QString::fromStdString(product.getDescription()));
    ui->lineEditPrice->setText(QString::number(product.getPrice()));
    ui->lineEditAmount->setText(QString::number(product.getAmount()));
    ui->lineEditCategory->setText(QString::number(product.getCategoryId()));

    ui->addProductButton->setText("Update Product");
}