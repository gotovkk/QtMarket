#include "../../include/ui/cartmenu.h"
#include "ui_cartmenu.h"

CartMenu::CartMenu(QWidget *parent, sqlite3 *dbConnection) : QWidget(parent), ui(new Ui::CartMenu), db(dbConnection),
                                                             sqlErrorHandler(dbConnection),
                                                             cart(new ProductListManager<CartItemWidget *>) {
    ui->setupUi(this);

    setupDatabase();
    itemsLayout = new QVBoxLayout(ui->scrollArea->widget());
    ui->scrollArea->widget()->setLayout(itemsLayout);


    connect(ui->backButton, &QPushButton::clicked, this, &CartMenu::onBackToShoppingClicked);
    connect(ui->orderButton, &QPushButton::clicked, this, &CartMenu::createOrder);

    loadCartItems();
}

CartMenu::~CartMenu() {
    delete ui;
    delete cart;
}



void CartMenu::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта.";
    }
}

void CartMenu::onBackToShoppingClicked() {
    emit backToShopping();
}

void CartMenu::addProductToCart(const QString &name, int quantity, double price, int productId) {
    for (CartItemWidget *existingItem: cart->getItems()) {
        if (existingItem->getProductId() == productId) {
            qDebug() << "Товар уже добавлен в корзину!";
            return;
        }
    }

    CartItemWidget *item = new CartItemWidget(name, quantity, price, productId, this);
    cart->addItem(item);
    itemsLayout->addWidget(item);

    connect(item, &CartItemWidget::removeItem, this, &CartMenu::removeProductFromCart);
    connect(item, &CartItemWidget::quantityChanged, this, &CartMenu::updateTotalPrice);

    checkAuthentication();

    int buyerId = SessionManager::getCurrentUserId();
    insertProductToCart(buyerId, productId, quantity, price);

    updateTotalPrice();
}

void CartMenu::checkAuthentication() {
    int buyerId = SessionManager::getCurrentUserId();
    if (buyerId == -1) {
        qDebug() << "Покупатель не авторизован!";
        throw Exceptions("Покупатель не авторизован!");
    }
}

void CartMenu::addProductToUI(const QString &name, int quantity, double price, int productId) {
    for (CartItemWidget *existingItem: cart->getItems()) {
        if (existingItem->getProductId() == productId) {
            qDebug() << "Товар уже отображается в корзине!";
            return;
        }
    }

    CartItemWidget *item = new CartItemWidget(name, quantity, price, productId, this);
    cart->addItem(item);
    itemsLayout->addWidget(item);

    connect(item, &CartItemWidget::removeItem, this, &CartMenu::removeProductFromCart);
    connect(item, &CartItemWidget::quantityChanged, this, &CartMenu::updateTotalPrice);

    updateTotalPrice();
}

void CartMenu::insertProductToCart(int buyerId, int productId, int quantity, double price) {
    std::string sqlInsert = "INSERT INTO cart (buyer_id, product_id, quantity, price) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlErrorHandler.handleError("Ошибка при подготовке SQL-запроса");
        return;
    }

    sqlite3_bind_int(stmt, 1, buyerId);
    sqlite3_bind_int(stmt, 2, productId);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, price);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlErrorHandler.handleError("Ошибка при добавлении товара в корзину");
        return;
    }

    sqlite3_finalize(stmt);
    qDebug() << "Товар добавлен в корзину.";
}

void CartMenu::removeProductFromCart(CartItemWidget *item) {
    int buyerId = SessionManager::getCurrentUserId();
    int productId = item->getProductId();

    if (buyerId != -1) {
        std::string sqlDelete = "DELETE FROM cart WHERE buyer_id = ? AND product_id = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sqlDelete.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlErrorHandler.handleError("Ошибка при подготовке SQL-запроса для удаления товара");
            return;
        }

        sqlite3_bind_int(stmt, 1, buyerId);
        sqlite3_bind_int(stmt, 2, productId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlErrorHandler.handleError("Ошибка при выполнении SQL-запроса для удаления товара");
        } else {
            qDebug() << "Товар успешно удален из базы данных.";
        }

        sqlite3_finalize(stmt);
    }

    itemsLayout->removeWidget(item);
    item->deleteLater();
    cart->removeItem(item);

    updateTotalPrice();
}

void CartMenu::updateTotalPrice() {
    double total = cart->calculateTotalPrice([](CartItemWidget *item) {
        return item->getTotalPrice();
    });
    ui->totalPriceLabel->setText(QString("Итого: %1 руб.").arg(total, 0, 'f', 2));
}

QList<CartItemWidget *> CartMenu::getCartItems() const {
    return cart->getItems();
}

void CartMenu::createOrder() {
    try {
        checkAuthentication();
    } catch (const UnauthorizedException &e) {
        qDebug() << "Ошибка: " << e.what();
        return;
    }

    double totalPrice = cart->calculateTotalPrice([](CartItemWidget *item) {
        return item->getTotalPrice();
    });
    std::string sqlInsertOrder = "INSERT INTO orders (buyer_id, status, total_price) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sqlInsertOrder.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlErrorHandler.handleError("Ошибка при подготовке SQL-запроса");
        return;
    }

    int buyerId = SessionManager::getCurrentUserId();
    sqlite3_bind_int(stmt, 1, buyerId);
    sqlite3_bind_text(stmt, 2, "В процессе", -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, totalPrice);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlErrorHandler.handleError("Ошибка при создании заказа");
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    int orderId = sqlite3_last_insert_rowid(db);
    qDebug() << "Заказ создан. ID заказа: " << orderId;

    for (CartItemWidget *item: cart->getItems()) {
        addProductToOrder(orderId, item);
    }

    clearCart();
    clearCartFromDatabase();
}

void CartMenu::addProductToOrder(int orderId, CartItemWidget *item) {
    int productId = item->getProductId();
    int quantity = item->getQuantity();
    double price = item->getUnitPrice();

    std::string sqlInsertOrderItem = "INSERT INTO orders_item (order_id, product_id, quantity, price) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sqlInsertOrderItem.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlErrorHandler.handleError("Ошибка при подготовке SQL-запроса");
        return;
    }

    sqlite3_bind_int(stmt, 1, orderId);
    sqlite3_bind_int(stmt, 2, productId);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, price);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlErrorHandler.handleError("Ошибка при добавлении товара в заказ");
    } else {
        qDebug() << "Товар добавлен в заказ.";
    }

    sqlite3_finalize(stmt);
}

void CartMenu::loadCartItems() {
    int buyerId = SessionManager::getCurrentUserId();

    std::string sqlSelectCart = "SELECT product_id, quantity, price FROM cart WHERE buyer_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelectCart.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlErrorHandler.handleError("Ошибка при подготовке SQL-запроса");
        return;
    }

    sqlite3_bind_int(stmt, 1, buyerId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int productId = sqlite3_column_int(stmt, 0);
        int quantity = sqlite3_column_int(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);

        QString name = getProductNameById(productId);
        addProductToUI(name, quantity, price, productId);
    }

    sqlite3_finalize(stmt);
    updateTotalPrice();
}


QString CartMenu::getProductNameById(int productId) {
    std::string sqlSelectProduct = "SELECT name FROM products WHERE id = ?;";
    sqlite3_stmt *stmt;
    QString productName;

    if (sqlite3_prepare_v2(db, sqlSelectProduct.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlErrorHandler.handleError("Ошибка при подготовке sql запроса для получения продуктта");
        return productName;
    }

    sqlite3_bind_int(stmt, 1, productId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        productName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    } else {
        qDebug() << "Продукт с ID " << productId << " не найден.";
    }

    sqlite3_finalize(stmt);
    return productName;
}


void CartMenu::clearCart() {
    while (!cart->getItems().isEmpty()) {
        removeProductFromCart(cart->getItems().first());
    }
    updateTotalPrice();
}

void CartMenu::clearCartFromDatabase() {
    int buyerId = SessionManager::getCurrentUserId();
    if (buyerId != -1) {
        std::string sqlClearCart = "DELETE FROM cart WHERE buyer_id = ?;";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sqlClearCart.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlErrorHandler.handleError("Ошибка при подготовке sql запроса");
            return;
        }

        sqlite3_bind_int(stmt, 1, buyerId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlErrorHandler.handleError("Ошибка при очистке корзины из бд");
        } else {
            qDebug() << "Корзина успешно очищена из базы данных.";
        }

        sqlite3_finalize(stmt);
    }
}
