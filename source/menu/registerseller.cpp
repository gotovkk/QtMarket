#include "ui_registerseller.h"
#include "../../include/ui/registerseller.h"
#include "../exception/Exceptions.h"

#include <iostream>


RegisterSeller::RegisterSeller(QWidget *parent) : QWidget(parent), ui(new Ui::RegisterSeller) {
    ui->setupUi(this);
    setupDatabase();

}

void RegisterSeller::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных(RegisterSellerMenu): " << sqlite3_errmsg(db);
    } else {
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

    if (username.isEmpty() || email.isEmpty() || phone.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка регистрации", "Все поля должны быть заполнены.");
        return;
    }

    if (password.length() < 6) {
        QMessageBox::warning(this, "Ошибка регистрации", "Пароль должен содержать не менее 6 символов.");
        return;
    }

    QRegularExpression emailPattern("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch emailMatch = emailPattern.match(email);
    if (!emailMatch.hasMatch()) {
        QMessageBox::warning(this, "Ошибка регистрации", "Введите корректный адрес электронной почты.");
        return;
    }

    QRegularExpression phonePattern("^\\d{7,15}$");
    QRegularExpressionMatch phoneMatch = phonePattern.match(phone);
    if (!phoneMatch.hasMatch()) {
        QMessageBox::warning(this, "Ошибка регистрации",
                             "Введите корректный номер телефона (только цифры, от 7 до 15 символов).");
        return;
    }

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
        throw SQLException("Ошибка регистрации! Возможно, такой логин уже существует.");
    }
}