#include "ui_sellermenu.h"
#include "../../include/ui/registerwindow.h"
#include <iostream>


SellerMenu::SellerMenu(QWidget *parent) : QWidget(parent), ui(new Ui::SellerMenu), orderWindow(nullptr) {
    ui->setupUi(this);

    report = new reportpage;
    addProductWindow = new AddProduct;
    backToBuyersWindow = new AddProduct;
    report->hide();
    connect(addProductWindow, &AddProduct::addProductWindow, this, &SellerMenu::show);
    connect(backToBuyersWindow, &AddProduct::addProductWindow, this, &SellerMenu::show);
    connect(report, &reportpage::switchToSellerMenu, this, &SellerMenu::onBackFromReportPage);

    setupDatabase();
}


void SellerMenu::onBackFromReportPage() {
    report->hide();

    this->show();
}
void SellerMenu::onBackToSellerMenu() {
    orderWindow->hide();
    this->show();

}


SellerMenu::~SellerMenu() {
    delete ui;
    sqlite3_close(db);
}

void SellerMenu::on_reportButton_clicked() {
    emit switchToReportPage();
    report->show();
}

void SellerMenu::on_orderButton_clicked() {
    orderWindow = new sellerorder;
    connect(orderWindow, &sellerorder::switchToSellerMenu, this, &SellerMenu::onBackToSellerMenu);
    this->hide();
    emit switchToSellerPage();
    orderWindow->show();
}

void SellerMenu::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc != SQLITE_OK) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
    }
}

void SellerMenu::loadProducts(sqlite3 *db, int id) {
    productManager.clear();

    int sellerId = id;
    qDebug() << "Полученный sellerId из SessionManager:" << sellerId;

    if (sellerId == -1) {
        qDebug() << "Ошибка: sellerId не установлен.";
        return;
    }

    std::string sqlSelect = "SELECT id, name, description, price, amount, seller_id, category_id, added_date "
                            "FROM products WHERE seller_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    if (sqlite3_bind_int64(stmt, 1, sellerId) != SQLITE_OK) {
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nameText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string name = nameText ? nameText : "";
        const char *descriptionText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string description = descriptionText ? descriptionText : "";
        int price = sqlite3_column_int(stmt, 3);
        int amount = sqlite3_column_int(stmt, 4);
        sqlite3_int64 seller_id = sqlite3_column_int64(stmt, 5);
        int category_id = sqlite3_column_int(stmt, 6);
        const char *dateText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        std::string added_date = dateText ? dateText : "";


        Product *product = new Product(id, name, description, price, amount, seller_id, category_id, added_date);
        productManager.addItem(product);

    }

    if (sqlite3_finalize(stmt) != SQLITE_OK) {
    }
    ui->scrollArea->update();

    displaySortedProducts();
    ui->scrollArea->update();

}

void SellerMenu::displaySortedProducts() {

    delete ui->scrollArea->widget();

    auto *container = new QWidget();
    auto *pBoxLayout = new QVBoxLayout(container);

    const auto &products = productManager.getItems();

    for (const auto &product: products) {
        qDebug() << "Добавление виджета для продукта с id:" << product->getId();

        auto *productWidget = new ProductItemWidget(*product);
        productWidget->setPageForRole(true);
        productWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        productWidget->setMinimumHeight(250);

        connect(productWidget, &ProductItemWidget::updateItem, this, &SellerMenu::onUpdateProduct);

        connect(productWidget, &ProductItemWidget::deleteItem, [this, productWidget, product](int productId) {
            this->onDeleteItem(productId, productWidget);
        });

        pBoxLayout->addWidget(productWidget);
    }

    qDebug() << "Количество виджетов в layout:" << pBoxLayout->count();

    container->setLayout(pBoxLayout);
    container->setMinimumWidth(600);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    ui->scrollArea->update();

    ui->scrollArea->setWidget(container);
    ui->scrollArea->update();
}


void SellerMenu::onDeleteItem(int productId, ProductItemWidget *productWidget) {
    if (productId == -1) {
        qDebug() << "Некорректный ID товара.";
        return;
    }

    std::string sqlDelete = "DELETE FROM products WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlDelete.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса для удаления товара: " << sqlite3_errmsg(db);
        return;
    }

    if (sqlite3_bind_int(stmt, 1, productId) != SQLITE_OK) {
        qDebug() << "Ошибка при привязке параметра ID товара: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
    } else {
        qDebug() << "Ошибка при выполнении SQL-запроса: " << sqlite3_errmsg(db);
    }

    sqlite3_finalize(stmt);

    auto productToDelete = std::find_if(productManager.getItems().begin(), productManager.getItems().end(),
                                        [productId](const Product *product) {
                                            return product->getId() == productId;
                                        });

    if (productToDelete != productManager.getItems().end()) {
        productManager.removeItem(*productToDelete);
    } else {
        qDebug() << "Товар с ID " << productId << " не найден в productManager.";
    }

    if (productWidget) {
        productWidget->deleteLater();
    }

    displaySortedProducts();
}


void SellerMenu::on_backButton_clicked() {
    this->close();
    emit firstWindow();
}

void SellerMenu::on_addProductButton_clicked() {
    addProductWindow->show();
    this->close();
}

void SellerMenu::onUpdateProduct(const Product &product) {
    qDebug() << "Обновление товара с ID:" << product.getId();

    addProductWindow->populateFields(product);
    addProductWindow->show();
    this->close();
}
