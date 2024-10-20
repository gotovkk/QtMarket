#include "ui_productitemwidget.h"

#include "../../include/ui/productitemwidget.h"
#include <iostream>

ProductItemWidget::ProductItemWidget(const Product& product, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ProductItemWidget) {
    ui->setupUi(this);

    ui->nameLabel->setText(QString::fromStdString(product.getName()));
    ui->descriptionLabel->setText(QString::fromStdString(product.getDescription()));
    ui->priceLabel->setText(QString::number(product.getPrice()));
    ui->amountLabel->setText(QString::number(product.getAmount()));
}

ProductItemWidget::~ProductItemWidget() {
    delete ui;
}