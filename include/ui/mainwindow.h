#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>
#include "registerwindow.h"
#include "sellermenu.h"
#include "buyermenu.h"
#include "../../source/auth/SellerAuth.h"
#include "../../source/auth/BuyerAuth.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;

    class RegistrationWindow;

    class SellerMenu;

    class BuyerMenu;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


    void show();

private slots:

    void on_registerButton_clicked();

    void on_loginSellerButton_clicked();

    void on_loginButton_clicked();

signals:
    void showSellerMenu();
private:
    Ui::MainWindow *ui;
    BuyerMenu *buyerMenu;
    BuyerMenu *backToFirst;
    SellerMenu *backToFirstWin;
    SellerMenu *sellerMenu;
    SellerAuth sellerAuth;
    BuyerAuth buyerAuth;
    sqlite3 *db{};

    void setupDatabase();


};
