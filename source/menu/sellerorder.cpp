#include "../../include/ui/sellerorder.h"
#include "ui_sellerorder.h"
#include "../auth/SessionManager.h"

sellerorder::sellerorder(QWidget *parent) : QWidget(parent), ui(new Ui::sellerorder) {

    ui->setupUi(this);

    ordersTable = new QTableWidget(this);
    ordersTable->setColumnCount(6);
    ordersTable->setHorizontalHeaderLabels({"ID", "Товар", "Кол-во", "Дата заказа", "Статус", "Цена"});
    ordersTable->verticalHeader()->setVisible(false);

    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ordersTable->setStyleSheet("QTableWidget {"
                               "border: 1px solid #ccc;"
                               "border-radius: 5px;"
                               "background-color: #1e1e1e;"
                               "color: white;"
                               "font-family: Arial, sans-serif;"
                               "font-size: 12pt;"
                               "}"
                               "QTableWidget::item {"
                               "padding: 8px;"
                               "border-bottom: 1px solid #ddd;"
                               "}"
                               "QTableWidget::item:selected {"
                               "background-color: #313638;"
                               "color: white;"
                               "}"
                               "QHeaderView::section {"
                               "background-color: #313638;"
                               "color: white;"
                               "padding: 8px;"
                               "font-size: 14pt;"
                               "border: none;"
                               "}");


    searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Поиск по названию товара");
    searchField->setStyleSheet("QLineEdit {"
                               "border: 1px solid #ccc;"
                               "border-radius: 10px;"
                               "padding: 5px;"
                               "font-size: 12pt;"
                               "}");


    statusFilter = new QComboBox(this);
    statusFilter->addItems({"Все", "Оформлен", "В процессе", "Выполнен"});
    statusFilter->setStyleSheet("QComboBox {"
                                "border: 1px solid #ccc;"
                                "border-radius: 10px;"
                                "padding: 5px;"
                                "font-size: 12pt;"
                                "background-color: #1e1e1e;"
                                "color: white;"
                                "}"
                                "QComboBox::drop-down {"
                                "border: 0px;"
                                "}"
                                "QComboBox QAbstractItemView {"
                                "background-color: #1e1e1e;"
                                "color: white;"
                                "selection-background-color: #313638;"
                                "selection-color: white;"
                                "}");

    markOrderedButton = new QPushButton("Пометить как оформленный", this);
    markOrderedButton->setStyleSheet("QPushButton {"
                                     "background-color: #313638;"
                                     "color: #fff; "
                                     "border-radius: 15px;"
                                     "padding: 10px;"
                                     "font-size: 14pt;"
                                     "}");

    markInProgressButton = new QPushButton("Пометить как в процессе", this);
    markInProgressButton->setStyleSheet("QPushButton {"
                                        "background-color: #313638;"
                                        "color: #fff; "
                                        "border-radius: 15px;"
                                        "padding: 10px;"
                                        "font-size: 14pt; "
                                        "}");
    refreshButton = new QPushButton("Обновить", this);
    refreshButton->setStyleSheet("QPushButton {"
                                 "background-color: #313638;"
                                 "color: #fff;"
                                 "border-radius: 15px;"
                                 "padding: 10px;"
                                 "font-size: 14pt;"
                                 "}");

    markCompletedButton = new QPushButton("Пометить как выполненный", this);
    markCompletedButton->setStyleSheet("QPushButton {"
                                       "background-color: #313638;"
                                       "color: #fff;"
                                       "border-radius: 15px;"
                                       "padding: 10px;"
                                       "font-size: 14pt;"
                                       "}");

    backButton = new QPushButton("Назад", this);
    backButton->setStyleSheet("QPushButton {"
                              "background-color: #313638;"
                              "color: #fff;"
                              "border-radius: 15px;"
                              "padding: 10px;"
                              "font-size: 14pt;"
                              "}");


    QDate currentDate = QDate::currentDate();
    QDate lastMonthDate = currentDate.addMonths(-1);

    startDateFilter = new QDateEdit(this);
    startDateFilter->setCalendarPopup(true);
    startDateFilter->setDisplayFormat("dd.MM.yyyy");
    startDateFilter->setDate(lastMonthDate);

    endDateFilter = new QDateEdit(this);
    endDateFilter->setCalendarPopup(true);
    endDateFilter->setDisplayFormat("dd.MM.yyyy");
    endDateFilter->setDate(currentDate);

    auto *layout = new QVBoxLayout(this);
    auto *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Поиск:"));
    filterLayout->addWidget(searchField);
    filterLayout->addWidget(new QLabel("Статус:"));
    filterLayout->addWidget(statusFilter);
    filterLayout->addWidget(new QLabel("Дата с:"));
    filterLayout->addWidget(startDateFilter);
    filterLayout->addWidget(new QLabel("по:"));
    filterLayout->addWidget(endDateFilter);
    filterLayout->addWidget(refreshButton);

    layout->addLayout(filterLayout);
    layout->addWidget(ordersTable);
    QHBoxLayout * buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(markCompletedButton);
    buttonLayout->addWidget(markOrderedButton);
    buttonLayout->addWidget(markInProgressButton);

    layout->addLayout(buttonLayout);

    layout->addWidget(backButton);

    connect(refreshButton, &QPushButton::clicked, this, &sellerorder::refreshOrders);
    connect(markCompletedButton, &QPushButton::clicked, this, &sellerorder::markOrderAsCompleted);
    connect(backButton, &QPushButton::clicked, this, &sellerorder::backToSellerButton);
    connect(searchField, &QLineEdit::textChanged, this, &sellerorder::filterOrders);
    connect(statusFilter, &QComboBox::currentTextChanged, this, &sellerorder::filterOrders);
    connect(startDateFilter, &QDateEdit::dateChanged, this, &sellerorder::filterOrders);
    connect(endDateFilter, &QDateEdit::dateChanged, this, &sellerorder::filterOrders);
    connect(statusFilter, &QComboBox::currentTextChanged, this, &sellerorder::onStatusFilterChanged);
    connect(markOrderedButton, &QPushButton::clicked, this, &sellerorder::markOrderAsOrdered);
    connect(markInProgressButton, &QPushButton::clicked, this, &sellerorder::markOrderAsInProgress);

    setStyleSheet("QWidget {"
                  "background-color: #1e1e1e;"
                  "font-family: Arial, sans-serif;}");

    setupDatabase();
    initializeOrders();
}


sellerorder::~sellerorder() {
    delete ui;
}

void sellerorder::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc != SQLITE_OK) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "Заказ помечен как выполненный";

    }
}

void sellerorder::backToSellerButton() {
    emit switchToSellerMenu();
}

void sellerorder::filterOrders() {
    QString searchText = searchField->text().trimmed();
    QString selectedStatus = statusFilter->currentText();

    for (int i = 0; i < ordersTable->rowCount(); ++i) {
        bool matchesSearch =
                searchText.isEmpty() || ordersTable->item(i, 0)->text().contains(searchText, Qt::CaseInsensitive);

        bool matchesStatus = (selectedStatus == "Все") || (ordersTable->item(i, 4)->text() == selectedStatus);

        ordersTable->setRowHidden(i, !(matchesSearch && matchesStatus));
    }

    qDebug() << "Фильтрация заказов завершена";
}


void sellerorder::onStatusFilterChanged() {
    filterOrders();
}

void sellerorder::markOrderAsCompleted() {
    QList<QTableWidgetItem *> selectedItems = ordersTable->selectedItems();

    if (selectedItems.isEmpty()) {
        qDebug() << "Нет выбранных заказов для выполнения.";
        return;
    }

    for (QTableWidgetItem *item: selectedItems) {
        int row = item->row() ;
        QString orderId = ordersTable->item(row, 0)->text();

        QTableWidgetItem *statusItem = ordersTable->item(row, 4);

        if (statusItem->text() != "Выполнен") {
            std::string updateQuery = std::format("UPDATE orders SET status = 'Выполнен' WHERE id = {};",
                                                  orderId.toStdString());

            char *errMsg = nullptr;
            int rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                sqlite3_free(errMsg);
            } else {

                if (statusItem) {
                    statusItem->setText("Выполнен");
                } else {
                }
            }
        }
    }
}


void sellerorder::markOrderAsOrdered() {
    QList<QTableWidgetItem *> selectedItems = ordersTable->selectedItems();

    if (selectedItems.isEmpty()) {
        qDebug() << "Нет выбранных заказов для выполнения.";
        return;
    }

    for (QTableWidgetItem *item: selectedItems) {
        int row = item->row() ;
        QTableWidgetItem *statusItem = ordersTable->item(row, 4);
        QString orderId = ordersTable->item(row, 0)->text();

        if (statusItem->text() != "Оформлен") {
            std::string updateQuery = std::format("UPDATE orders SET status = 'Оформлен' WHERE id = {};",
                                                  orderId.toStdString());

            char *errMsg = nullptr;
            int rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                qDebug() << "Ошибка при обновлении статуса заказа в базе данных: " << errMsg;
                sqlite3_free(errMsg);
            } else {
                qDebug() << "Заказ с ID" << orderId << "помечен как оформленный в базе данных.";

                if (statusItem) {
                    statusItem->setText("Оформлен");
                } else {
                    qDebug() << "Не удалось обновить статус для заказа с ID: " << orderId;
                }
            }
        }
    }
}


void sellerorder::markOrderAsInProgress() {
    QList<QTableWidgetItem *> selectedItems = ordersTable->selectedItems();

    if (selectedItems.isEmpty()) {
        return;
    }

    for (QTableWidgetItem *item: selectedItems) {
        int row = item->row();
        QTableWidgetItem *statusItem = ordersTable->item(row, 4);

        if (statusItem->text() != "В процессе") {
            QString orderId = ordersTable->item(row, 0)->text();


            std::string updateQuery = std::format("UPDATE orders SET status = 'В процессе' WHERE id = {};",
                                                  orderId.toStdString());

            char *errMsg = nullptr;
            int rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                sqlite3_free(errMsg);
            } else {

                if (statusItem) {
                    statusItem->setText("В процессе");
                } else {
                }
            }
        }
    }
}

void sellerorder::refreshOrders() {
    qDebug() << "Обновление списка заказов...";

    int sellerId = SessionManager::getCurrentUserId();;

    ordersTable->setRowCount(0);

    loadOrders(db, sellerId);
}

void sellerorder::initializeOrders() {
    qDebug() << "Метод initializeOrders вызывается";

    int sellerId = SessionManager::getCurrentUserId();

    loadOrders(db, sellerId);
}


void sellerorder::loadOrders(sqlite3 *db, int sellerId) {
    ordersTable->clearContents();

    qDebug() << "Загрузка заказов для продавца с ID:" << sellerId;

    if (sellerId == -1) {
        qDebug() << "Ошибка: sellerId не установлен.";
        return;
    }

    std::string sqlSelectOrders = "SELECT o.id, o.order_date, o.status, oi.quantity, oi.price, p.name "
                                  "FROM orders o "
                                  "JOIN orders_item oi ON o.id = oi.order_id "
                                  "JOIN products p ON oi.product_id = p.id "
                                  "WHERE p.seller_id = ?";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sqlSelectOrders.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка при подготовке SQL-запроса для загрузки заказов: " << sqlite3_errmsg(db);
        return;
    }

    if (sqlite3_bind_int64(stmt, 1, sellerId) != SQLITE_OK) {
        qDebug() << "Ошибка при привязке параметра sellerId.";
        sqlite3_finalize(stmt);
        return;
    }

    int row = 0;
    int currentOrderId = -1;
    QString orderDate;
    QString status;
    QString allProducts;
    int totalQuantity = 0;
    int totalPrice = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);
        const char *orderDateText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string orderDateTextStr = orderDateText ? orderDateText : "";
        orderDate = QString::fromStdString(orderDateTextStr);

        const char *statusText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string statusTextStr = statusText ? statusText : "";
        status = QString::fromStdString(statusTextStr);

        int quantity = sqlite3_column_int(stmt, 3);
        int price = sqlite3_column_int(stmt, 4);
        const char *productNameText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        std::string productNameTextStr = productNameText ? productNameText : "";

        if (orderId != currentOrderId) {
            if (currentOrderId != -1) {
                ordersTable->insertRow(row);
                ordersTable->setItem(row, 0, new QTableWidgetItem(QString::number(currentOrderId)));
                ordersTable->setItem(row, 1, new QTableWidgetItem(allProducts));
                ordersTable->setItem(row, 2, new QTableWidgetItem(QString::number(totalQuantity)));
                ordersTable->setItem(row, 3, new QTableWidgetItem(orderDate));
                ordersTable->setItem(row, 4, new QTableWidgetItem(status));
                ordersTable->setItem(row, 5, new QTableWidgetItem(QString::number(totalPrice)));
                row++;
            }

            currentOrderId = orderId;
            allProducts = QString::fromStdString(productNameTextStr);
            totalQuantity = quantity;
            totalPrice = price;
        } else {
            allProducts += "\n" + QString::fromStdString(productNameTextStr);
            totalQuantity += quantity;
            totalPrice += price * quantity;
        }
    }

    if (currentOrderId != -1) {
        ordersTable->insertRow(row);
        ordersTable->setItem(row, 0, new QTableWidgetItem(QString::number(currentOrderId)));
        ordersTable->setItem(row, 1, new QTableWidgetItem(allProducts));
        ordersTable->setItem(row, 2, new QTableWidgetItem(QString::number(totalQuantity)));
        ordersTable->setItem(row, 3, new QTableWidgetItem(orderDate));
        ordersTable->setItem(row, 4, new QTableWidgetItem(status));
        ordersTable->setItem(row, 5, new QTableWidgetItem(QString::number(totalPrice)));
    }

    sqlite3_finalize(stmt);

    ordersTable->setColumnWidth(0, 83);
    ordersTable->setColumnWidth(1, 212);
    ordersTable->setColumnWidth(2, 110);
    ordersTable->setColumnWidth(3, 129);
    ordersTable->setColumnWidth(4, 115);
    ordersTable->setColumnWidth(5, 110);

    for (int row = 0; row < ordersTable->rowCount(); ++row) {
        for (int col = 0; col < ordersTable->columnCount(); ++col) {
            QTableWidgetItem *item = ordersTable->item(row, col);
            if (item) {
                QLabel *label = new QLabel(item->text());
                label->setWordWrap(true);
                label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                ordersTable->setCellWidget(row, col, label);
            }
        }
    }

    ordersTable->resizeRowsToContents();
}

