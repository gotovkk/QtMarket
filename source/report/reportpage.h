#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "nlohmann/json.hpp" // Библиотека nlohmann
#include "../../sqlite/sqlite3.h"
#include "../exception/Exceptions.h"
#include "../auth/SessionManager.h"

namespace Ui {
    class reportpage;
}

class reportpage : public QWidget {
Q_OBJECT

public:
    explicit reportpage(QWidget *parent = nullptr);
    ~reportpage();

    // Установка ID пользователя (например, текущего продавца)
    void setUserId(int id);

signals:
    void switchToSellerMenu(); // Сигнал для возврата в меню продавца

public slots:
    void backToCartButton();  // Обработчик кнопки "Назад"
    void setupDatabase();     // Инициализация базы данных
    void generateReport();    // Генерация отчета

private:
    Ui::reportpage *ui;

    // Поля для фильтров (в случае расширения функционала отчетов)
    QDateTime startDate;
    QDateTime endDate;
    double minPrice;
    double maxPrice;
    QString productName;

    sqlite3 *db;  // Указатель на базу данных
    int userId;   // ID текущего пользователя (продавца)

    // Приватные методы для генерации отчетов в различных форматах
    QString generateJsonReport(); // Генерация JSON-отчета
    QString generateXmlReport();  // Генерация XML-отчета (на будущее)
};

#endif // REPORTPAGE_H