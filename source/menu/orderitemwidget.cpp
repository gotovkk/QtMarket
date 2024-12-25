// orderitemwidget.cpp

#include "orderitemwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

OrderItemWidget::OrderItemWidget(int orderId, const QString &price, const QString &status, QWidget *parent)
        : QWidget(parent), orderId(orderId), price(price), status(status) {

    QString formattedPrice = QString::number(price.toDouble(), 'f', 2) + " руб.";

    orderIdLabel = new QLabel(QString("Заказ №%1").arg(orderId), this);
    orderIdLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: white;");

    priceLabel = new QLabel(formattedPrice, this);
    priceLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");

    statusLabel = new QLabel("Статус: " + status, this);
    statusLabel->setStyleSheet("color: white; font-size: 14px; font-weight: normal;");

    if (status == "В процессе") {
        statusLabel->setStyleSheet("color: #FFA500; font-size: 14px; font-weight: normal;");
    } else if (status == "Выполнен") {
        statusLabel->setStyleSheet("color: #499167; font-size: 14px; font-weight: normal;");
    } else if (status == "Оформлен") {
        statusLabel->setStyleSheet("color: #FFF8F0; font-size: 14px; font-weight: normal;");
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(orderIdLabel);
    mainLayout->addWidget(priceLabel);
    mainLayout->addWidget(statusLabel);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(2);
    shadowEffect->setYOffset(2);
    shadowEffect->setColor(QColor(0, 0, 0, 160));
    this->setGraphicsEffect(shadowEffect);

    setLayout(mainLayout);

    setStyleSheet(R"(
        QWidget {
            background-color: #555;
            border: 1px solid #444;
            border-radius: 10px;
            padding: 10px;
            color: white;
            font-family: 'Arial', sans-serif;
        }
    )");
}

OrderItemWidget::~OrderItemWidget() {

}

void OrderItemWidget::onViewDetailsClicked() {
    emit viewOrderDetails(orderId);
}
