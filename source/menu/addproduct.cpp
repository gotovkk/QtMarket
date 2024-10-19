#include "ui_addproduct.h"
#include "../../include/ui/addproduct.h"

#include <iostream>


AddProduct::AddProduct(QWidget *parent):
        QWidget(parent),
        ui(new Ui::AddProduct) {
    ui->setupUi(this);
}

AddProduct::~AddProduct()
{
    delete ui;
}
