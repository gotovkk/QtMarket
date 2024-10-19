#include "ui_sellermenu.h"
#include "../../include/ui/registerwindow.h"
#include "../../include/ui/mainwindow.h"
#include "../../include/ui/sellermenu.h"
#include <iostream>


SellerMenu::SellerMenu(QWidget *parent):
        QWidget(parent),
        ui(new Ui::SellerMenu) {
    ui->setupUi(this);
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
//    this->close();
//    emit firstWindow();
}


