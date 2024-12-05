//
// Created by anima on 18.10.2024.
//
#include "../../include/ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "../auth/SessionManager.h"
#include <iostream>
#include <QString>
#include <regex>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow) {
    ui->setupUi(this);
    sellerMenu = new SellerMenu;
    backToFirstWin = new SellerMenu;

    buyerMenu = new BuyerMenu;
    backToFirst = new BuyerMenu;

    connect(sellerMenu, &SellerMenu::firstWindow, this, &MainWindow::show);
    connect(backToFirstWin, &SellerMenu::firstWindow, this, &MainWindow::show);
    connect(buyerMenu, &BuyerMenu::secondWindow, this, &MainWindow::show);
    connect(backToFirst, &BuyerMenu::secondWindow, this, &MainWindow::show);
    ui->labelErrorMessage->setVisible(false);
    setupDatabase();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::show() {
    qDebug() << "MainWindow отображается!";
    QMainWindow::show(); // Вызываем базовый метод для отображения окна
}

void MainWindow::setupDatabase() {
    int rc = sqlite3_open(R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных(Main): " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта(Main).";
    }
}

void MainWindow::on_registerButton_clicked() {
    auto *registrationWindow = new RegistrationWindow(this);
    registrationWindow->exec();
}

bool getLoginPassword(const std::string &login, const std::string &password) {
    std::regex loginRegex("^[A-Za-z0-9]{5,}$");
    if (!std::regex_match(login, loginRegex)) {
        qDebug() << "Ошибка: Логин должен содержать только буквы и цифры и быть длиной не менее 5 символов.";
        return false;
    }

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

    if (stdLogin.empty() || stdPassword.empty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми.");
        return;
    }

    if (buyerAuth.login(db, stdLogin, stdPassword)) {
        buyerMenu->show();
        buyerMenu->loadProducts(db);
        this->close();
    }
}

void MainWindow::on_loginSellerButton_clicked() {

    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();

    std::string stdLogin = login.toStdString();
    std::string stdPassword = password.toStdString();

    if (stdLogin.empty() || stdPassword.empty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми.");
        return;
    }

    if (sellerAuth.login(db, stdLogin, stdPassword)) {
        int sellerId = SessionManager::getCurrentUserId();

        sellerMenu->loadProducts(db, sellerId);
        sellerMenu->show();

        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неправильный логин или пароль. Попробуйте снова.");
    }
}


