#include "ui_registerseller.h"
#include "../../include/ui/registerseller.h"

#include <iostream>


RegisterSeller::RegisterSeller(QWidget *parent):
        QWidget(parent),
        ui(new Ui::RegisterSeller)
{
    ui->setupUi(this);
}

RegisterSeller::~RegisterSeller()
{
    delete ui;
}

void RegisterSeller::on_backButton_clicked() {
    this->close();
    emit regSellerWindow();
}
