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

private slots:

    void filterOrders();

    void markOrderAsCompleted();

    void refreshOrders();

    void backToSellerMenu();

signals:

    void backToMenu();

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
};

#endif //QTHYDRAMARKET_SELLERORDER_H
