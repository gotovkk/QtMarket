#include "ui_registerwindow.h"
#include "../../include/ui/registerwindow.h"
#include "../../include/ui/mainwindow.h"
#include <iostream>


RegistrationWindow::RegistrationWindow(QWidget *parent):
        QDialog(parent),
        ui(new Ui::RegistrationWindow) {
    ui->setupUi(this);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
