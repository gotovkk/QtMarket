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
    class RegisterSeller;
}
QT_END_NAMESPACE


class RegisterSeller : public QWidget {
Q_OBJECT

public:
    explicit RegisterSeller(QWidget *parent = nullptr);
    ~RegisterSeller();

private slots:
    void on_backButton_clicked();


signals:
    void regSellerWindow();


private:
    Ui::RegisterSeller *ui;
    DatabaseLoader databaseLoader;
    Storage storage;
    sqlite3* db;

};

