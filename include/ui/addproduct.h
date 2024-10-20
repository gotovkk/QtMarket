#pragma once
#include <QWidget>
#include <QTimer>
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <memory>
#include "../../source/database/DatabaseLoader.h"
#include "../Storage.h"
#include "../../source/management/ProductManagement.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class AddProduct;
}
QT_END_NAMESPACE


class AddProduct : public QWidget {
Q_OBJECT

public:
    explicit AddProduct(QWidget *parent = nullptr);
    ~AddProduct();

private slots:
    void on_backButton_clicked();
    void on_addProductButton_clicked();

signals:
    void addProductWindow();


private:
    Ui::AddProduct *ui;
    DatabaseLoader databaseLoader;
    Storage storage;
    sqlite3* db;

    void setupDatabase();
};

