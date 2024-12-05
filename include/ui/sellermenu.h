#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <memory>
#include "addproduct.h"
#include "../ProductListManager.h"
#include "../../source/exception/Exceptions.h"
#include "../../source/database/DatabaseLoader.h"
#include "../../source/report/reportpage.h"
#include "sellerorder.h"


#include "cartmenu.h"
#include "productitemwidget.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class SellerMenu;
    class AddProduct;
    class reportpage;
}
QT_END_NAMESPACE


class SellerMenu : public QWidget {
Q_OBJECT

public:
    explicit SellerMenu(QWidget *parent = nullptr);

    ~SellerMenu();

    bool sortByPrice;

    void setupDatabase();

    void loadProducts(sqlite3 *db, int id);

    void displaySortedProducts();
    void openSellerOrder();

private slots:

   void onDeleteItem(int productId, ProductItemWidget *productWidget);

    void on_backButton_clicked();

    void on_addProductButton_clicked();

    void on_reportButton_clicked();

    void on_orderButton_clicked();

    void onBackToSellerMenu();


    void onBackFromReportPage();

signals:

    void firstWindow();

    void switchToReportPage();


private:
    Ui::SellerMenu *ui;

    reportpage *report;
    AddProduct *addProductWindow;
    AddProduct *backToBuyersWindow;
    sellerorder *orderWindow;
    sqlite3 *db;

    ProductListManager<Product *> productManager;

};

