#include "../../include/ui/cartitemwidget.h"
#include "ui_CartItemWidget.h"

CartItemWidget::CartItemWidget(const QString &name, int quantity, double price, int productId, QWidget *parent)
        : QWidget(parent), pricePerUnit(price), productName(name), productId(productId),
          quantity(quantity) {  // Инициализация quantity

    nameLabel = new QLabel(name, this);
    priceLabel = new QLabel(QString::number(price * quantity, 'f', 2) + " руб.", this);

    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setRange(1, 99);
    quantitySpinBox->setValue(quantity);

    removeButton = new QPushButton("Удалить", this);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(quantitySpinBox);
    layout->addWidget(priceLabel);
    layout->addWidget(removeButton);

    setLayout(layout);

    connect(removeButton, &QPushButton::clicked, this, [=]() {
        emit removeItem(this);
    });
    connect(quantitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CartItemWidget::onQuantityChanged);
}

double CartItemWidget::getTotalPrice() const {
    return quantity * pricePerUnit;
}

QString CartItemWidget::getProductName() const {
    return productName;
}

int CartItemWidget::getProductId() const {
    return productId;
}

double CartItemWidget::getUnitPrice() const {
    return pricePerUnit;
}

void CartItemWidget::onQuantityChanged(int newQuantity) {
    quantity = newQuantity;
    priceLabel->setText(QString::number(getTotalPrice(), 'f', 2) + " руб.");
    emit quantityChanged(getTotalPrice());
}

int CartItemWidget::getQuantity() const {
    return quantity;
}

void CartItemWidget::setQuantity(int quantity) {
    if (quantity <= 0) {
        emit removeItem(this);
        return;
    }

    quantitySpinBox->setValue(quantity);

    updateTotalPrice();
}

void CartItemWidget::updateTotalPrice() {
    double totalPrice = quantity * pricePerUnit;
    priceLabel->setText(QString::number(totalPrice, 'f', 2) + " руб.");
}
