#include "ui_buyermenu.h"
#include "../../include/ui/buyermenu.h"
#include <iostream>

BuyerMenu::BuyerMenu(QWidget *parent) : QWidget(parent), ui(new Ui::BuyerMenu), sortByPrice(true) {
    ui->setupUi(this);

    setupDatabase();
    loadProducts(db);
    cartMenu = new CartMenu;  
    cartMenu->hide();

    cartMenu->loadCartItems();


    connect(this, &BuyerMenu::showCartMenu, this, [=]() {
        this->hide();
        cartMenu->show();
    });

    connect(cartMenu, &CartMenu::backToShopping, this, [=]() {
        cartMenu->hide();
        this->show();
    });

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &BuyerMenu::filterProductsByName);


}

BuyerMenu::~BuyerMenu() {
    delete ui;
    sqlite3_close(db);
}

void BuyerMenu::filterProductsByName(const QString &searchText) {
    QString lowerCaseSearchText = searchText.toLower();

    filteredProducts.clear();
    for (const auto &product : productManager.getItems()) {
        QString productName = QString::fromStdString(product->getName()).toLower();
        if (productName.contains(lowerCaseSearchText)) {
            filteredProducts.push_back(product);
        }
    }
    displayFilteredProducts();
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

    displayFilteredProducts();
}

template<typename T, typename Comparator>
void sortProducts(std::vector<T> &products, Comparator comp) {
    std::sort(products.begin(), products.end(), comp);
}

void BuyerMenu::sortByPriceFunction() {
    productManager.sortItems([](const Product *a, const Product *b) {
        return a->getPrice() < b->getPrice();
    });
    qDebug() << "Продукты отсортированы по цене.";
}

void BuyerMenu::sortByName() {
    productManager.sortItems([](const Product *a, const Product *b) {
        return a->getName() < b->getName();
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
    cartMenu->clearCart();
    this->hide();
    emit secondWindow();
}

void BuyerMenu::loadProducts(sqlite3 *db) {
    productManager.clear();

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

        Product *product = new Product(id, name, description, price, amount, seller_id, category_id, added_date);
        productManager.addItem(product);
    }

    sqlite3_finalize(stmt);

    filteredProducts.clear();
    filteredProducts.insert(filteredProducts.end(), productManager.getItems().begin(), productManager.getItems().end());

    displayFilteredProducts();
}


void BuyerMenu::displayFilteredProducts() {
    delete ui->scrollArea->widget();

    auto *container = new QWidget();
    auto *pBoxLayout = new QVBoxLayout(container);

    for (const auto &product : filteredProducts) {
        auto *productWidget = new ProductItemWidget(*product);
        productWidget->setPageForRole(false);
        productWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        productWidget->setMinimumHeight(250);

        connect(productWidget, &ProductItemWidget::addItemToCart, this, [this](const Product &product) {
            cartMenu->addProductToCart(QString::fromStdString(product.getName()), 1, product.getPrice(), product.getId());
        });

        pBoxLayout->addWidget(productWidget);
    }

    container->setLayout(pBoxLayout);
    ui->scrollArea->setWidget(container);
    container->adjustSize();
    container->setMinimumHeight(pBoxLayout->sizeHint().height());
}


void BuyerMenu::on_cartButton_clicked() {
    cartMenu->loadCartItems();
    emit showCartMenu();
}

void BuyerMenu::showCart() {
    CartMenu *cartMenu = new CartMenu();
    cartMenu->show();

    connect(cartMenu, &CartMenu::backToShopping, this, &BuyerMenu::show);
}






