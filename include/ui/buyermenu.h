#pragma once

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPixmap>
#include <QLabel>
#include "../../source/database/DatabaseLoader.h"
#include "../Product.h"
#include "productitemwidget.h"

namespace Ui {
    class BuyerMenu;
}

class BuyerMenu : public QWidget {
Q_OBJECT

public:
    explicit BuyerMenu(QWidget *parent = nullptr);
    void loadProducts(sqlite3* db);
    ~BuyerMenu();

private slots:
    void on_logOutButton_clicked();

signals:
    void secondWindow();

private:
    Ui::BuyerMenu *ui;
    QVBoxLayout *layout;
    sqlite3* db;

    void setupDatabase();

};
