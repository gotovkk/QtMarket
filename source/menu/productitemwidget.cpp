#include "ui_productitemwidget.h"
#include "../../include/ui/productitemwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

ProductItemWidget::ProductItemWidget(const Product &product, QWidget *parent) : QWidget(parent),
                                                                                ui(new Ui::ProductItemWidget),
                                                                                product(product) {

    ui->setupUi(this);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    QHBoxLayout * namePriceLayout = new QHBoxLayout();
    QVBoxLayout * descriptionLayout = new QVBoxLayout();

    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(20);

    QLabel *nameLabel = new QLabel(QString::fromStdString(product.getName()), this);
    nameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");
    nameLabel->setMinimumHeight(100);

    QLabel *priceLabel = new QLabel(QString::number(product.getPrice()) + " руб.", this);
    priceLabel->setStyleSheet("font-size: 16px; color: #95a5a6;");
    priceLabel->setMinimumHeight(100);

    namePriceLayout->addWidget(nameLabel);
    namePriceLayout->addWidget(priceLabel);

    QLabel *descriptionLabel = new QLabel(QString::fromStdString(product.getDescription()), this);
    descriptionLabel->setStyleSheet("font-size: 14px; color: #93A7A9;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMinimumHeight(60);

    QHBoxLayout * buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    addToCartButton = new QPushButton("Добавить в корзину", this);
    addToCartButton->setStyleSheet(
            "background-color: #3498db; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    addToCartButton->setMinimumHeight(50);  // Увеличиваем высоту кнопки
    connect(addToCartButton, &QPushButton::clicked, this, &ProductItemWidget::on_addItemButton_clicked);

    updateButton = new QPushButton("Обновить товар", this);
    updateButton->setStyleSheet(
            "background-color: #3498db; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    connect(updateButton, &QPushButton::clicked, this, &ProductItemWidget::on_updateButton_clicked);

    deleteButton = new QPushButton("Удалить товар", this);
    deleteButton->setStyleSheet(
            "background-color: #e74c3c; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    connect(deleteButton, &QPushButton::clicked, this, &ProductItemWidget::on_delButton_clicked);


    descriptionLayout->addWidget(descriptionLabel);

    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    descriptionLayout->addLayout(buttonLayout);

    descriptionLayout->addWidget(addToCartButton);


    mainLayout->addLayout(namePriceLayout);
    mainLayout->addLayout(descriptionLayout);

    setLayout(mainLayout);
    setStyleSheet("background-color: #272723; border-radius: 10px;");
}


void ProductItemWidget::setupBuyerPage() {
    ui->nameLabel_3->setText(QString::fromStdString(product.getName()));
    ui->descriptionLabel_2->setText(QString::fromStdString(product.getDescription()));
    ui->priceLabel_3->setText(QString::number(product.getPrice()));
    ui->amountLabel_2->setText(QString::number(product.getAmount()));

    addToCartButton->setVisible(true);
    updateButton->setVisible(false);
    deleteButton->setVisible(false);

}

void ProductItemWidget::setupSellerPage() {
    ui->nameLabel_2->setText(QString::fromStdString(product.getName()));
    ui->descriptionLabel->setText(QString::fromStdString(product.getDescription()));
    ui->priceLabel_2->setText(QString::number(product.getPrice()));
    ui->amountLabel->setText(QString::number(product.getAmount()));

    ui->stackedWidget->setCurrentIndex(0);

    addToCartButton->setVisible(false);
    updateButton->setVisible(true);
    deleteButton->setVisible(true);
}


void ProductItemWidget::setPageForRole(bool isSeller) {
    if (isSeller) {
        setupSellerPage();
    } else {
        setupBuyerPage();
    }
}

void ProductItemWidget::on_addItemButton_clicked() {
    emit addItemToCart(product);
}

void ProductItemWidget::on_delButton_clicked() {
    emit deleteItem(product.getId());
}

void ProductItemWidget::on_updateButton_clicked() {
    emit updateItem(product);
}

int ProductItemWidget::getProductId() const {
    return product.getId();
}

ProductItemWidget::~ProductItemWidget() {
    delete ui;
}
