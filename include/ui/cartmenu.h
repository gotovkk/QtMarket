#ifndef CARTMENU_H
#define CARTMENU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "cartitemwidget.h"
#include "../../source/database/DatabaseLoader.h"
#include "../../../sqlite/sqlite3.h"
#include "../ProductListManager.h"
#include "../../source/database/CartDatabase.h"
#include "../../source/exception/Exceptions.h"
#include "../../source/report/reportpage.h"
#include "buyerorders.h"

namespace Ui {
    class CartMenu;
}

class CartMenu : public QWidget {
Q_OBJECT

public:
    explicit CartMenu(QWidget *parent = nullptr, sqlite3 *dbConnection = nullptr);

    ~CartMenu();

    void addProductToCart(const QString &name, int quantity, double price, int productId);

    QList<CartItemWidget *> getCartItems() const;

    void updateTotalPrice();

    void removeProductFromCart(CartItemWidget *item);

    void setupDatabase();

    void clearCart();

    void addProductToUI(const QString &name, int quantity, double price, int productId);

    void loadCartItems();

    QString getProductNameById(int productId);

    void clearCartFromDatabase();

    void insertProductToCart(int buyerId, int productId, int quantity, double price);

    void checkAuthentication();


signals:

    void backToShopping();

    void switchToOrderPage();

private slots:

    void onBackToShoppingClicked();
    void onBackToCartMenu();


    void createOrder();
    void on_myOrderButton_clicked();
private:
    void addProductToOrder(int orderId, CartItemWidget *item);

    Ui::CartMenu *ui;
    QVBoxLayout *itemsLayout;
    CartDatabase *cartDatabase;
    ProductListManager<CartItemWidget *> *cart;
    buyerOrders *ordersWindow;


    sqlite3 *db;
    SqlErrorHandler sqlErrorHandler;

};

#endif // CARTMENU_H
