#ifndef CARTITEMWIDGET_H
#define CARTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>

class CartItemWidget : public QWidget {
Q_OBJECT

public:
    explicit CartItemWidget(const QString& name, int quantity, double price, int productId, QWidget *parent = nullptr);
    double getTotalPrice() const;
    QString getProductName() const;
    int getProductId() const;
    double getUnitPrice() const;
    int getQuantity() const;
    void setQuantity(int quantity);

signals:
    void removeItem(CartItemWidget *item);
    void quantityChanged(double totalPrice);

private slots:
    void onQuantityChanged(int newQuantity);

private:
    QLabel *nameLabel;
    QLabel *priceLabel;
    QSpinBox *quantitySpinBox;
    QPushButton *removeButton;
    double pricePerUnit;
    QString productName;
    int productId;
    int quantity;  // Добавили переменную quantity
    void updateTotalPrice();  // Метод для обновления общей цены
};

#endif // CARTITEMWIDGET_H