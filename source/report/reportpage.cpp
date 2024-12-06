#include "reportpage.h"
#include "ui_reportpage.h"
#include <QFile>
#include <QTextStream>
#include <nlohmann/json.hpp>
#include <QDebug>

using json = nlohmann::json;

reportpage::reportpage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::reportpage)
{
    ui->setupUi(this);

    connect(ui->backButton, &QPushButton::clicked, this, &reportpage::backToCartButton);
    connect(ui->generateReportButton, &QPushButton::clicked, this, &reportpage::generateReport);

    setupDatabase();

    ui->startDateEdit->setDate(QDate::currentDate().addDays(-30));
    ui->endDateEdit->setDate(QDate::currentDate());
}

void reportpage::backToCartButton() {
    emit switchToSellerMenu();
}

reportpage::~reportpage() {
    sqlite3_close(db);
    delete ui;
}

void reportpage::setupDatabase() {
    int rc = sqlite3_open("C:/Users/anima/CLionProjects/QtHydraMarket/mydb.db", &db);
    if (rc) {
        qDebug() << "Не удалось открыть базу данных: " << sqlite3_errmsg(db);
    } else {
        qDebug() << "База данных успешно открыта.";
    }
}

void reportpage::generateReport() {
    int sellerId = SessionManager::getCurrentUserId();
    qDebug() << "Генерация отчета для продавца с ID:" << sellerId;

    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();

    QString startDateStr = startDate.toString("yyyy-MM-dd");
    QString endDateStr = endDate.toString("yyyy-MM-dd");

    const char *query = R"(
        SELECT
            o.id AS order_id,
            o.order_date,
            o.status,
            o.total_price,
            p.name AS product_name,
            oi.quantity,
            oi.price
        FROM
            orders_item oi
        JOIN
            products p ON oi.product_id = p.id
        JOIN
            orders o ON oi.order_id = o.id
        WHERE
            p.seller_id = ?
            AND o.order_date BETWEEN ? AND ?;
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Ошибка подготовки SQL-запроса:" << sqlite3_errmsg(db);
        return;
    }

    if (sqlite3_bind_int(stmt, 1, sellerId) != SQLITE_OK) {
        qDebug() << "Ошибка привязки параметров запроса:" << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_bind_text(stmt, 2, startDateStr.toStdString().c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        qDebug() << "Ошибка привязки параметра startDate:" << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return;
    }

    if (sqlite3_bind_text(stmt, 3, endDateStr.toStdString().c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        qDebug() << "Ошибка привязки параметра endDate:" << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return;
    }

    std::map<int, json> orders;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);

        if (orders.find(orderId) == orders.end()) {
            orders[orderId] = {
                    {"order_id", orderId},
                    {"order_date", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))},
                    {"status", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))},
                    {"total_price", sqlite3_column_int(stmt, 3)} ,
                    {"products", json::array()}
            };
        }

        orders[orderId]["products"].push_back({
                                                      {"product_name", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))},
                                                      {"quantity", sqlite3_column_int(stmt, 5)},
                                                      {"price", sqlite3_column_int(stmt, 6)}
                                              });
    }

    sqlite3_finalize(stmt);

    json groupedOrders = json::array();
    for (const auto &entry : orders) {
        groupedOrders.push_back(entry.second);
    }

    QString filePath = QFileDialog::getSaveFileName(
            this, tr("Сохранить отчет"), QDir::homePath(), tr("JSON файлы (*.json)"));

    if (filePath.isEmpty()) {
        qDebug() << "Сохранение отменено пользователем.";
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QString::fromStdString(groupedOrders.dump(4));
        file.close();
        qDebug() << "Отчет успешно сохранен в файл:" << filePath;
    } else {
        qDebug() << "Не удалось сохранить отчет в файл:" << file.errorString();
    }
}
