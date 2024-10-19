#include "ui_buyermenu.h"
#include "../../include/ui/buyermenu.h"
#include <iostream>


BuyerMenu::BuyerMenu(QWidget *parent):
        QWidget(parent),
        ui(new Ui::BuyerMenu) {
    ui->setupUi(this);
}

BuyerMenu::~BuyerMenu()
{
    delete ui;
}

void BuyerMenu::on_logOutButton_clicked() {
    this->close();
    emit secondWindow();
}