#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "nlohmann/json.hpp"
#include "../../sqlite/sqlite3.h"
#include "../exception/Exceptions.h"
#include "../auth/SessionManager.h"

namespace Ui {
    class reportpage;
}

class reportpage : public QWidget {
Q_OBJECT

public:
    explicit reportpage(QWidget *parent = nullptr);
    ~reportpage();

    void setUserId(int id);

signals:
    void switchToSellerMenu();

public slots:
    void backToCartButton();
    void setupDatabase();
    void generateReport();

private:
    Ui::reportpage *ui;

    QDateTime startDate;
    QDateTime endDate;
    double minPrice;
    double maxPrice;
    QString productName;

    sqlite3 *db;
    int userId;

    QString generateJsonReport();
    QString generateXmlReport();
};

#endif // REPORTPAGE_H