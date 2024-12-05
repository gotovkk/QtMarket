#ifndef PRODUCTLISTMANAGER_H
#define PRODUCTLISTMANAGER_H

#include <QList>
#include <algorithm>
#include <functional>
#include <QDebug>
#include "Iterator.h"
#include "ConstIterator.h"

template<typename T>
class ProductListManager {
public:
    void addItem(const T &item) {
        qDebug() << "Добавляем товар: " << item;
        items.append(item);
        qDebug() << "Товары после добавления: " << items.size();
    }

    void removeItem(const T &item) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == item) {
                items.erase(items.begin() + (it - begin()));
                break;
            }
        }
    }

    double calculateTotalPrice(std::function<double(const T &)> priceFunction) const {
        double total = 0;
        for (const auto &item: items) {
            total += priceFunction(item);
        }
        return total;
    }

    const QList<T> &getItems() const {
        return items;
    }

    void clear() {
        qDebug() << "Очищаем коллекцию товаров.";
        items.clear();
        qDebug() << "Размер коллекции после очистки: " << items.size();
    }

    void sortItems(const std::function<bool(const T &, const T &)> &comparator) {
        qDebug() << "Сортируем товары.";
        std::sort(items.begin(), items.end(), comparator);
        qDebug() << "Товары отсортированы.";
    }

    Iterator<T> begin() {
        return Iterator<T>(items.begin());
    }

    Iterator<T> end() {
        return Iterator<T>(items.end());
    }

    ConstIterator<T> begin() const {
        return ConstIterator<T>(items.begin());
    }

    ConstIterator<T> end() const {
        return ConstIterator<T>(items.end());
    }

private:
    QList<T> items;
};

#endif // PRODUCTLISTMANAGER_H
