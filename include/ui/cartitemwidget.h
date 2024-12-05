#ifndef CARTITEMWIDGET_H
#define CARTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>

// Класс, представляющий элемент корзины
class CartItemWidget : public QWidget {
Q_OBJECT

public:
    explicit CartItemWidget(const QString &name, int quantity, double price, int productId, QWidget *parent = nullptr);

    // Получение данных о продукте
    double getTotalPrice() const;   // Получить общую стоимость (количество * цена за единицу)
    QString getProductName() const; // Получить название продукта
    int getProductId() const;       // Получить ID продукта
    double getUnitPrice() const;    // Получить цену за единицу
    int getQuantity() const;        // Получить текущее количество

    // Установка количества
    void setQuantity(int quantity);

signals:
    void removeItem(CartItemWidget *item);  // Сигнал об удалении элемента
    void quantityChanged(double totalPrice); // Сигнал об изменении общей стоимости товара

private slots:
    void onQuantityChanged(int newQuantity); // Обработчик изменения количества

private:
    // UI-компоненты
    QLabel *nameLabel;         // Метка для имени продукта
    QLabel *priceLabel;        // Метка для общей стоимости
    QSpinBox *quantitySpinBox; // Спинбокс для изменения количества
    QPushButton *removeButton; // Кнопка для удаления элемента

    // Данные продукта
    double pricePerUnit;       // Цена за единицу товара
    QString productName;       // Название товара
    int productId;             // ID продукта
    int quantity;              // Текущее количество товара

    // Вспомогательные методы
    void updateTotalPrice();   // Метод для обновления метки с общей стоимостью
};

#endif // CARTITEMWIDGET_H
