#include "ui_sellermenu.h"
#include "../../include/ui/registerwindow.h"
#include "../../include/ui/mainwindow.h"
#include <iostream>


SellerMenu::SellerMenu(QWidget *parent):
        QWidget(parent),
        ui(new Ui::SellerMenu) {
    ui->setupUi(this);
    addProductWindow = new AddProduct;
    backToBuyersWindow = new AddProduct;

    connect(addProductWindow, &AddProduct::addProductWindow, this, &SellerMenu::show);
    connect(backToBuyersWindow, &AddProduct::addProductWindow, this, &SellerMenu::show);
}

SellerMenu::~SellerMenu()
{
    delete ui;
}

void SellerMenu::on_backButton_clicked() {
    this->close();
    emit firstWindow();
}

void SellerMenu::on_addProductButton_clicked() {
    addProductWindow->show();
    this->close();
}



