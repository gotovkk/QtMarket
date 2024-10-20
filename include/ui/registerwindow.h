#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <memory>
#include "registerseller.h"
#include "../../source/auth/BuyerAuth.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class RegistrationWindow;

    class MainWindow;
}
QT_END_NAMESPACE


class RegistrationWindow : public QDialog {
Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);

    ~RegistrationWindow();

private slots:

    void on_registerButton_clicked();

    void on_sellerRegButton_clicked();

private:
    Ui::RegistrationWindow *ui;
    RegisterSeller *regSellerMenu;
    RegisterSeller *backToReg;
    sqlite3 *db;
    BuyerAuth buyerAuth;

    void setupDatabase();
};

