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
#include "../ProductListManager.h"
#include "cartmenu.h"

namespace Ui {
    class BuyerMenu;
}

class BuyerMenu : public QWidget {
Q_OBJECT

public:
    explicit BuyerMenu(QWidget *parent = nullptr);
    ~BuyerMenu();


    void setupDatabase();
    void loadProducts(sqlite3 *db);
    //void displaySortedProducts();
    void displayFilteredProducts();
    void sortByPriceFunction();
    void sortByName();
    void filterProductsByName(const QString &searchText);


private slots:
    void on_logOutButton_clicked();
    void on_toggleSortButton_clicked();
    void on_cartButton_clicked();

    void showCart();
signals:
    void secondWindow();
    void showCartMenu();

private:
    Ui::BuyerMenu *ui;
    sqlite3* db;
    std::vector<Product> products;
    CartMenu* cartMenu;

    ProductListManager<Product *> productManager;
    std::vector<Product *> filteredProducts;
    bool sortByPrice;
    QVBoxLayout *descriptionLayout;


};
