//
// Created by anima on 18.10.2024.
//
#include "../../include/ui/mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QString>
#include <regex>

MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent),
            ui(new Ui::MainWindow) {
        ui->setupUi(this);
        sellerMenu = new SellerMenu;
        buyerMenu = new BuyerMenu;
        backToFirstWin = new SellerMenu;
        backToFirst = new BuyerMenu;

        connect(sellerMenu, &SellerMenu::firstWindow, this, &MainWindow::show);
        connect(backToFirstWin, &SellerMenu::firstWindow, this, &MainWindow::show);
        connect(buyerMenu, &BuyerMenu::secondWindow, this, &MainWindow::show);
        connect(backToFirst, &BuyerMenu::secondWindow, this, &MainWindow::show);
      ui->labelErrorMessage->setVisible(false);


}
MainWindow::~MainWindow() {
        delete ui;
}

void MainWindow::on_registerButton_clicked() {
    RegistrationWindow *registrationWindow = new RegistrationWindow(this);
    registrationWindow->exec();
}

// Функция проверки логина и пароля
bool getLoginPassword(const std::string &login, const std::string &password) {
    // Проверка, что логин состоит только из букв и цифр и длиной более 4 символов
    std::regex loginRegex("^[A-Za-z0-9]{5,}$");
    if (!std::regex_match(login, loginRegex)) {
        qDebug() << "Ошибка: Логин должен содержать только буквы и цифры и быть длиной не менее 5 символов.";
        return false;
    }

    // Проверка, что пароль длиной не менее 5 символов и не содержит специальных символов
    std::regex passwordRegex("^[A-Za-z0-9]{5,}$");
    if (!std::regex_match(password, passwordRegex)) {
        qDebug() << "Ошибка: Пароль должен быть длиной не менее 5 символов и не содержать специальных символов.";
        return false;
    }

    qDebug() << "Логин и пароль прошли проверку.";
    qDebug() << "Логин: " << QString::fromStdString(login);
    qDebug() << "Пароль: " << QString::fromStdString(password);
    return true;
}


void MainWindow::on_loginButton_clicked() {
    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    std::string stdLogin = login.toStdString();
    std::string stdPassword = password.toStdString();

    if (getLoginPassword(stdLogin, stdPassword)) {
        buyerMenu->show();
        this->close();
    } else {
        ui->labelErrorMessage->setText("Неправильный логин или пароль. Попробуйте снова.");
        ui->labelErrorMessage->setVisible(true);
    }
}
    void MainWindow::on_lineEditLogin_editingFinished() {
        QString login = ui->lineEditLogin->text();
        if (login.isEmpty()) {
            qDebug() << "Логин не должен быть пустым";
        } else {
            qDebug() << "Логин введен: " << login;
        }
    }

    void MainWindow::on_lineEditPassword_editingFinished() {
        QString password = ui->lineEditPassword->text();
        if (password.length() < 6) {
            qDebug() << "Пароль слишком короткий";
        } else {
            qDebug() << "Пароль введен";
        }
    }

void MainWindow::on_loginSellerButton_clicked() {
    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    std::string stdLogin = login.toStdString();
    std::string stdPassword = password.toStdString();

    if (getLoginPassword(stdLogin, stdPassword)) {
        sellerMenu->show();
        this->close();
    } else {
        ui->labelErrorMessage->setText("Неправильный логин или пароль. Попробуйте снова.");
        ui->labelErrorMessage->setVisible(true);
    }
}


