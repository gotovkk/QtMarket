#pragma once
#include <QWidget>
#include "../Product.h" // Убедитесь, что импортируете ваш класс Product

namespace Ui {
    class ProductItemWidget;
}

class ProductItemWidget : public QWidget {
Q_OBJECT

public:
    explicit ProductItemWidget(const Product& product, QWidget *parent = nullptr);
    ~ProductItemWidget();



private slots:
    void on_addItemButton_clicked();

signals:
    void addItemToCart(const Product &product);

private:
    Ui::ProductItemWidget *ui;
    Product product;
};

