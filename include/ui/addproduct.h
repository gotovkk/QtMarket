#pragma once
#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <memory>


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

signals:

private:
    Ui::AddProduct *ui;
};

