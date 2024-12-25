#ifndef QTHYDRAMARKET_SELLERORDER_H
#define QTHYDRAMARKET_SELLERORDER_H

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

#include "../ProductListManager.h"
#include "../../source/database/DatabaseLoader.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class sellerorder;
}
QT_END_NAMESPACE

class sellerorder : public QWidget {
Q_OBJECT

public:
    explicit sellerorder(QWidget *parent = nullptr);

    ~sellerorder() override;
    void loadOrders(sqlite3 *db,int sellerId);
    void setupDatabase();
    void initializeOrders();
    void markOrderAsInProgress();
    void markOrderAsOrdered();
private slots:
    void filterOrders();
    void markOrderAsCompleted();
    void refreshOrders();
    void backToSellerButton();
    void onStatusFilterChanged();

signals:
    void switchToSellerMenu();

private:
    Ui::sellerorder *ui;
    sellerorder *orderWindow;
    QTableWidget *ordersTable;
    QLineEdit *searchField;
    QComboBox *statusFilter;
    QDateEdit *startDateFilter;
    QDateEdit *endDateFilter;
    QPushButton *refreshButton;
    QPushButton *backButton;
    QPushButton *markCompletedButton;
    QPushButton *markOrderedButton;
    QPushButton *markInProgressButton;
    sqlite3 *db;
    ProductListManager<Product *> productManager;

};

#endif //QTHYDRAMARKET_SELLERORDER_H