#include "ui_buyermenu.h"
#include "../../include/ui/buyermenu.h"
#include "../auth/SessionManager.h"
#include <iostream>

BuyerMenu::BuyerMenu(QWidget *parent) : QWidget(parent), ui(new Ui::BuyerMenu), sortByPrice(true) {
    ui->setupUi(this);

    setupDatabase();
    loadProducts(db);
    cartMenu = new CartMenu;  // добавляем поле CartMenu
    cartMenu->hide();

    cartMenu->loadCartItems();  // Загрузка корзины для текущего пользователя


    connect(this, &BuyerMenu::showCartMenu, this, [=]() {
        this->hide();        // Скрываем BuyerMenu
        cartMenu->show();    // Показываем CartMenu
    });

    connect(cartMenu, &CartMenu::backToShopping, this, [=]() {
        cartMenu->hide();    // Скрываем CartMenu
        this->show();        // Показываем BuyerMenu
    });
}

BuyerMenu::~BuyerMenu() {
    delete ui;
    sqlite3_close(db);
}



void BuyerMenu::on_toggleSortButton_clicked() {

    sortByPrice = !sortByPrice;
    qDebug() << "Кнопка нажата. Состояние сортировки: " << sortByPrice;

    if (sortByPrice) {
        ui->toggleSortButton->setText("Имя a⭢b");
        sortByPriceFunction();
        qDebug() << "Продукты отсортированы по цене.";
    } else {
        ui->toggleSortButton->setText("Цена⭡");

        sortByName();
        qDebug() << "Продукты отсортированы по имени.";
    }

    displaySortedProducts();
}

template<typename T, typename Comparator>
void sortProducts(std::vector<T> &products, Comparator comp) {
    std::sort(products.begin(), products.end(), comp);
}

void BuyerMenu::sortByPriceFunction() {
    sortProducts(products, [](const Product &a, const Product &b) {
        return a.getPrice() < b.getPrice();
    });
    qDebug() << "Продукты отсортированы по цене.";
}

void BuyerMenu::sortByName() {
    sortProducts(products, [](const Product &a, const Product &b) {
        return a.getName() < b.getName();
    });
    qDebug() << "Продукты отсортированы по имени.";
}

void BuyerMenu::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта.";
    }
}

void BuyerMenu::on_logOutButton_clicked() {
    SessionManager::logout();
    cartMenu->clearCart(); // Очищаем корзину
    this->hide();
    emit secondWindow();
}

void BuyerMenu::loadProducts(sqlite3 *db) {
    products.clear();

    std::string sqlSelect = "SELECT id, name, description, price, amount, seller_id, category_id, added_date FROM products;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса: " << sqlite3_errmsg(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nameText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string name = nameText ? nameText : "";
        const char *descriptionText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string description = descriptionText ? descriptionText : "";
        int price = sqlite3_column_int(stmt, 3);
        int amount = sqlite3_column_int(stmt, 4);
        int seller_id = sqlite3_column_int(stmt, 5);
        int category_id = sqlite3_column_int(stmt, 6);
        const char *dateText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        std::string added_date = dateText ? dateText : "";

        Product product(id, name, description, price, amount, seller_id, category_id, added_date);
        products.push_back(product);
    }

    sqlite3_finalize(stmt);

    displaySortedProducts();
}

void BuyerMenu::displaySortedProducts() {
    delete ui->scrollArea->widget();

    auto *container = new QWidget();
    auto *pBoxLayout = new QVBoxLayout(container);

    for (const auto &product: products) {
        auto *productWidget = new ProductItemWidget(product);
        productWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        productWidget->setMinimumHeight(250);
        pBoxLayout->addWidget(productWidget);
        connect(productWidget, &ProductItemWidget::addItemToCart, this, [this](const Product &product) {
            cartMenu->addProductToCart(QString::fromStdString(product.getName()), 1, product.getPrice(), product.getId());
        });
    }
    container->setLayout(pBoxLayout);
    ui->scrollArea->setWidget(container);
    container->adjustSize();
    container->setMinimumHeight(pBoxLayout->sizeHint().height());
}

void BuyerMenu::on_cartButton_clicked() {
    cartMenu->loadCartItems();  // Загружаем товары в корзину
    emit showCartMenu();  // Отправляем сигнал для открытия корзины
}

void BuyerMenu::showCart() {
    // Здесь создается и показывается окно корзины
    CartMenu *cartMenu = new CartMenu();
    cartMenu->show();

    // Подключаем сигнал возврата к покупкам
    connect(cartMenu, &CartMenu::backToShopping, this, &BuyerMenu::show);
}






