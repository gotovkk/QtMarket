#pragma once
#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>
#include "registerwindow.h"
#include "sellermenu.h"
#include "buyermenu.h"
#include "../../source/auth/SellerAuth.h"


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

    private slots:

        void on_registerButton_clicked();

        void on_loginSellerButton_clicked();

        void on_loginButton_clicked();

        void on_lineEditLogin_editingFinished();

        void on_lineEditPassword_editingFinished();
        signals:

    private:
        Ui::MainWindow *ui;
        std::unique_ptr<RegistrationWindow> registrationWindow;
        BuyerMenu *buyerMenu;
        BuyerMenu *backToFirst;
        SellerMenu *backToFirstWin;
        SellerMenu *sellerMenu;
        SellerAuth sellerAuth;
        sqlite3* db;

    };
