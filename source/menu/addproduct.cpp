#include "ui_addproduct.h"
#include "../../include/ui/addproduct.h"

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
    int seller_id = 1;

    databaseLoader.addToTable(storage, db, seller_id, name.toStdString(), description.toStdString(), price, amount,
                              category_id);

    ui->lineEditName->clear();
    ui->lineEditDescription->clear();
    ui->lineEditPrice->clear();
    ui->lineEditAmount->clear();
    ui->lineEditCategory->clear();
    ui->productLabel->setVisible(true);
    QTimer::singleShot(1000, this, [this]() {
        ui->productLabel->setVisible(false);
    });
}