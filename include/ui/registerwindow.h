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
    class RegistrationWindow;
    class MainWindow;
}
QT_END_NAMESPACE


    class RegistrationWindow : public QDialog {
    Q_OBJECT

    public:
        explicit RegistrationWindow(QWidget *parent = nullptr);

        ~RegistrationWindow();


    private:
        Ui::RegistrationWindow *ui;
    };

