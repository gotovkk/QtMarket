#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <memory>
#include "addproduct.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class SellerMenu;

    class AddProduct;
}
QT_END_NAMESPACE


class SellerMenu : public QWidget {
Q_OBJECT

public:
    explicit SellerMenu(QWidget *parent = nullptr);

    ~SellerMenu();

private slots:

    void on_backButton_clicked();

    void on_addProductButton_clicked();

signals:

    void firstWindow();

private:
    Ui::SellerMenu *ui;
    AddProduct *addProductWindow;
    AddProduct *backToBuyersWindow;
};

