#include "ui_registerwindow.h"
#include "../../include/ui/registerwindow.h"
#include "../../include/ui/mainwindow.h"
#include <iostream>


RegistrationWindow::RegistrationWindow(QWidget *parent):
        QDialog(parent),
        ui(new Ui::RegistrationWindow) {
    ui->setupUi(this);
    regSellerMenu = new RegisterSeller;
    backToReg = new RegisterSeller;

    connect(regSellerMenu, &RegisterSeller::regSellerWindow, this, &RegistrationWindow::show);
    connect(backToReg, &RegisterSeller::regSellerWindow, this, &RegistrationWindow::show);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

void RegistrationWindow::on_sellerRegButton_clicked() {
    regSellerMenu->show();
    this->close();
}
