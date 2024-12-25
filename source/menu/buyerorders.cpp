// buyerorders.cpp

#include "../../include/ui/buyerorders.h"
#include "ui_buyerOrders.h"
#include "../auth/SessionManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "orderitemwidget.h"  // Include the OrderItemWidget class

buyerOrders::buyerOrders(QWidget *parent) :
        QWidget(parent), ui(new Ui::buyerOrders) {
    ui->setupUi(this);

    // Initialize the database
    setupDatabase();

    // Connect the "Back" button
    connect(ui->backButton, &QPushButton::clicked, this, &buyerOrders::backToCartButton);

    // Initialize orders for the buyer
    initializeOrders();
}

buyerOrders::~buyerOrders() {
    delete ui;
    if (db) {
        sqlite3_close(db);  // Close the database connection when the object is destroyed
    }
}

void buyerOrders::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc != SQLITE_OK) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "Подключение к базе данных успешно!";
    }
}

void buyerOrders::initializeOrders() {
    qDebug() << "Метод initializeOrders вызывается";

    int buyerId = SessionManager::getCurrentUserId();  // Get the current user's ID from the session

    if (buyerId != -1) {
        loadOrders(db, buyerId);  // Load orders for the buyer
    } else {
        qDebug() << "Ошибка: покупатель не авторизован!";
    }
}

void buyerOrders::loadOrders(sqlite3 *db, int buyerId) {
    // Clear the layout before loading new orders
    QVBoxLayout *itemsLayout = new QVBoxLayout(ui->scrollArea->widget());
    ui->scrollArea->widget()->setLayout(itemsLayout);

    qDebug() << "Загрузка заказов для покупателя с ID:" << buyerId;

    if (buyerId == -1) {
        qDebug() << "Ошибка: buyerId не установлен.";
        return;
    }

    // SQL query to get all orders for the buyer
    std::string sqlSelectOrders = "SELECT o.id, o.order_date, o.status, oi.quantity, oi.price, p.name "
                                  "FROM orders o "
                                  "JOIN orders_item oi ON o.id = oi.order_id "
                                  "JOIN products p ON oi.product_id = p.id "
                                  "WHERE o.buyer_id = ?";

    sqlite3_stmt *stmt;

    // Prepare the SQL query
    if (sqlite3_prepare_v2(db, sqlSelectOrders.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса для загрузки заказов: " << sqlite3_errmsg(db);
        return;
    }

    // Bind the buyerId parameter
    if (sqlite3_bind_int64(stmt, 1, buyerId) != SQLITE_OK) {
        qDebug() << "Ошибка при привязке параметра buyerId.";
        sqlite3_finalize(stmt);
        return;
    }

    int currentOrderId = -1;
    QString orderDate;
    QString status;
    QString allProducts;
    int totalQuantity = 0;
    int totalPrice = 0;

    // Loop through the query results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);
        const char *orderDateText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        orderDate = QString::fromUtf8(orderDateText ? orderDateText : "");

        const char *statusText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        status = QString::fromUtf8(statusText ? statusText : "");

        int quantity = sqlite3_column_int(stmt, 3);
        int price = sqlite3_column_int(stmt, 4);
        const char *productNameText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        QString productName = QString::fromUtf8(productNameText ? productNameText : "");

        // Process orders
        if (orderId != currentOrderId) {
            if (currentOrderId != -1) {
                // Add the previous order widget to the layout
                OrderItemWidget *orderWidget = new OrderItemWidget(currentOrderId, QString::number(totalPrice), status, ui->scrollArea->widget());

                // Add product list as a label
                QLabel *productLabel = new QLabel(allProducts, orderWidget);
                orderWidget->layout()->addWidget(productLabel);

                itemsLayout->addWidget(orderWidget);
            }

            currentOrderId = orderId;
            allProducts = productName;
            totalQuantity = quantity;
            totalPrice = price * quantity;
        } else {
            // Add product information to the current order
            allProducts += "\n" + productName;
            totalQuantity += quantity;
            totalPrice += price * quantity;
        }
    }

    // Insert the last order into the layout
    if (currentOrderId != -1) {
        OrderItemWidget *orderWidget = new OrderItemWidget(currentOrderId, QString::number(totalPrice), status, ui->scrollArea->widget());

        QLabel *productLabel = new QLabel(allProducts, orderWidget);
        orderWidget->layout()->addWidget(productLabel);

        itemsLayout->addWidget(orderWidget);
    }

    sqlite3_finalize(stmt);  // Finalize the statement to free resources
}

void buyerOrders::backToCartButton() {
    emit switchToCartMenu();  // Emit signal to switch back to the cart menu
}
