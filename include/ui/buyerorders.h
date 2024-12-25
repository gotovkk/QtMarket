//
// Created by anima on 25.12.2024.
//

#ifndef QTHYDRAMARKET_BUYERORDERS_H
#define QTHYDRAMARKET_BUYERORDERS_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include "../../source/database/DatabaseLoader.h"


QT_BEGIN_NAMESPACE
namespace Ui { class buyerOrders; }
QT_END_NAMESPACE

class buyerOrders : public QWidget {
Q_OBJECT

public:
    explicit buyerOrders(QWidget *parent = nullptr);


    ~buyerOrders() override;

    void loadOrders(sqlite3 *db, int buyerId);
    void initializeOrders();
    void setupDatabase();
    private slots:
    void backToCartButton();
signals:
    void switchToCartMenu();

private:
    Ui::buyerOrders *ui;
  //  QPushButton *backButton;
    QTableWidget *ordersTable;
    sqlite3 *db;

};


#endif //QTHYDRAMARKET_BUYERORDERS_H
