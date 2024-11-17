#pragma once

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPixmap>
#include <QLabel>
#include "../../source/database/DatabaseLoader.h"
#include "../Product.h"
#include "productitemwidget.h"
#include "cartmenu.h"

namespace Ui {
    class BuyerMenu;
}

class BuyerMenu : public QWidget {
Q_OBJECT

public:
    explicit BuyerMenu(QWidget *parent = nullptr);
    ~BuyerMenu();

private slots:
    void on_logOutButton_clicked();
    void on_toggleSortButton_clicked();
    void on_cartButton_clicked();

    // Слот для отображения корзины
    void showCart();



signals:
    void secondWindow();
    void showCartMenu();

private:
    Ui::BuyerMenu *ui;
    sqlite3* db;
    std::vector<Product> products;
    CartMenu* cartMenu;

    bool sortByPrice;
    void setupDatabase();
    void loadProducts(sqlite3 *db);
    void displaySortedProducts();
    void sortByPriceFunction();
    void sortByName();
};
