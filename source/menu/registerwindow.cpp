#include "ui_registerwindow.h"
#include "../../include/ui/registerwindow.h"
#include "../../include/ui/mainwindow.h"
#include <iostream>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RegistrationWindow) {
    ui->setupUi(this);
    regSellerMenu = new RegisterSeller;
    backToReg = new RegisterSeller;

    connect(regSellerMenu, &RegisterSeller::regSellerWindow, this, &RegistrationWindow::show);
    connect(backToReg, &RegisterSeller::regSellerWindow, this, &RegistrationWindow::show);

    setupDatabase();
}

void RegistrationWindow::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных(RegisterUser): " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта(RegisterUser).";
    }
}

RegistrationWindow::~RegistrationWindow() {
    delete ui;
}

void RegistrationWindow::on_sellerRegButton_clicked() {
    regSellerMenu->show();
    this->close();
}

void RegistrationWindow::on_registerButton_clicked() {
    QString name = ui->lineEditLogin->text();
    QString email = ui->lineEditEmail->text();
    QString password = ui->lineEditPassword->text();

    std::string stdName = name.toStdString();
    std::string stdEmail = email.toStdString();
    std::string stdPassword = password.toStdString();
    int buyer_id;

    if (buyerAuth.registerBuyer(db, stdName, stdPassword, stdEmail, buyer_id)) {
        QMessageBox::information(this, "Регистрация",
                                 "Регистрация прошла успешно! Ваш ID: " + QString::number(buyer_id));
        this->close();
    } else {
        QMessageBox::warning(this, "Регистрация", "Ошибка регистрации! Возможно, такое имя уже существует.");
    }
}

