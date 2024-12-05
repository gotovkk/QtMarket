#include "../../include/ui/cartitemwidget.h"
#include "ui_CartItemWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QString>

CartItemWidget::CartItemWidget(const QString &name, int quantity, double price, int productId, QWidget *parent)
        : QWidget(parent),
          productName(name),
          quantity(quantity),
          pricePerUnit(price),
          productId(productId) {
    // Устанавливаем фиксированный размер для основного виджета
    setFixedSize(500, 50); // Ширина 400px, высота 50px

    // Создаем виджеты
    nameLabel = new QLabel(productName, this);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 12px; color: white;");
    nameLabel->setFixedSize(180, 30);

    priceLabel = new QLabel(QString::number(getTotalPrice(), 'f', 2) + " руб.", this);
    priceLabel->setStyleSheet("color: white; font-size: 11px;");
    priceLabel->setFixedSize(80, 30);

    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setRange(1, 99);
    quantitySpinBox->setValue(quantity);
    quantitySpinBox->setStyleSheet(R"(
        QSpinBox {
            background-color: #777;
            color: white;
            border: 1px solid #555;
            border-radius: 3px;
            padding: 2px;
        }
    )");
    quantitySpinBox->setFixedSize(100, 30); // Фиксированный размер

    removeButton = new QPushButton("Удалить", this);
    removeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #cc4444;
            color: white;
            font-weight: bold;
            border: none;
            border-radius: 3px;
            padding: 3px 8px;
        }
        QPushButton:hover {
            background-color: #ff5555;
        }
        QPushButton:pressed {
            background-color: #aa3333;
        }
    )");
    removeButton->setFixedSize(80, 30); // Фиксированный размер

    // Компоновка
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    layout->addWidget(nameLabel);
    layout->addWidget(quantitySpinBox);
    layout->addWidget(priceLabel);
    layout->addWidget(removeButton);

    setLayout(layout);

    // Устанавливаем стиль для основного виджета
    setStyleSheet(R"(
        QWidget {
            background-color: #555; /* Серый фон */
            border: 1px solid #444;
            border-radius: 6px;
        }
    )");

    // Подключение сигналов
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        emit removeItem(this); // Генерируем сигнал "удалить элемент"
    });

    connect(quantitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CartItemWidget::onQuantityChanged);
}


double CartItemWidget::getTotalPrice() const {
    return quantity * pricePerUnit;
}

QString CartItemWidget::getProductName() const {
    return productName;
}

// Получить ID продукта
int CartItemWidget::getProductId() const {
    return productId;
}

double CartItemWidget::getUnitPrice() const {
    return pricePerUnit;
}

void CartItemWidget::onQuantityChanged(int newQuantity) {
    quantity = newQuantity; // Обновляем количество
    updateTotalPrice();     // Пересчитываем общую цену
    emit quantityChanged(getTotalPrice()); // Генерируем сигнал с обновленной ценой
}

int CartItemWidget::getQuantity() const {
    return quantity;
}

void CartItemWidget::setQuantity(int newQuantity) {
    if (newQuantity <= 0) {
        emit removeItem(this); // Если количество <= 0, удаляем элемент
        return;
    }

    quantitySpinBox->setValue(newQuantity); // Устанавливаем значение в SpinBox
    updateTotalPrice();                     // Обновляем общую стоимость
}

void CartItemWidget::updateTotalPrice() {
    priceLabel->setText(QString::number(getTotalPrice(), 'f', 2) + " руб.");
}
