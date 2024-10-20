#include "ui_registerseller.h"
#include "../../include/ui/registerseller.h"

#include <iostream>


RegisterSeller::RegisterSeller(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RegisterSeller) {
    ui->setupUi(this);
    setupDatabase();

}

void RegisterSeller::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных(RegisterSellerMenu): " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта.(RegisterSellerMenu)";
    }
}

RegisterSeller::~RegisterSeller() {
    delete ui;
}

void RegisterSeller::on_backButton_clicked() {
    this->close();
    emit regSellerWindow();
}

void RegisterSeller::on_registerButton_clicked() {
    QString username = ui->lineEditLogin->text();
    QString email = ui->lineEditEmail->text();
    QString phone = ui->lineEditNum->text();
    QString password = ui->lineEditPassword->text();

    std::string stdUsername = username.toStdString();
    std::string stdEmail = email.toStdString();
    std::string stdPhone = phone.toStdString();
    std::string stdPassword = password.toStdString();
    int seller_id;

    if (sellerAuth.registerSeller(db, stdUsername, stdPassword, stdEmail, stdPhone, seller_id)) {
        QMessageBox::information(this, "Регистрация",
                                 "Регистрация прошла успешно! Ваш ID: " + QString::number(seller_id));
        this->close();
    } else {
        QMessageBox::warning(this, "Регистрация", "Ошибка регистрации! Возможно, такой логин уже существует.");
    }
}
