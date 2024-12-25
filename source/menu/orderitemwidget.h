// orderitemwidget.h

#ifndef ORDERITEMWIDGET_H
#define ORDERITEMWIDGET_H

#include <QWidget>
#include <QString>

class QLabel;
class QPushButton;

class OrderItemWidget : public QWidget {
Q_OBJECT

public:
    explicit OrderItemWidget(int orderId, const QString &price, const QString &status, QWidget *parent = nullptr);
    ~OrderItemWidget();

signals:
    void viewOrderDetails(int orderId);

private slots:
    void onViewDetailsClicked();

private:
    int orderId;
    QString price;
    QString status;

    QLabel *orderIdLabel;
    QLabel *priceLabel;
    QLabel *statusLabel;
    QPushButton *viewDetailsButton;
};

#endif // ORDERITEMWIDGET_H
