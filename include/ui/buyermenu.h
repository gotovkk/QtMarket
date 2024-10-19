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
    class BuyerMenu;
}
QT_END_NAMESPACE


class BuyerMenu : public QWidget {
Q_OBJECT

public:
    explicit BuyerMenu(QWidget *parent = nullptr);
    ~BuyerMenu();

private slots:
    void on_logOutButton_clicked();

signals:
    void secondWindow();

private:
    Ui::BuyerMenu *ui;
};

