#pragma once

#include <QWidget>
#include "../Product.h" // Убедитесь, что импортируете ваш класс Product

namespace Ui {
    class ProductItemWidget;
}

class ProductItemWidget : public QWidget {
Q_OBJECT

public:
    explicit ProductItemWidget(const Product &product, QWidget *parent = nullptr);

    ~ProductItemWidget();

    void setPageForRole(bool isSeller);

    int getProductId() const;

private slots:

    void on_addItemButton_clicked();

    void on_delButton_clicked();

    void on_updateButton_clicked();

signals:

    void addItemToCart(const Product &product);

    void deleteItem(int productId);

    void updateItem(const Product &product);

private:
    Ui::ProductItemWidget *ui;
    Product product;

    QPushButton *addToCartButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;

    void setupBuyerPage();

    void setupSellerPage();
};

