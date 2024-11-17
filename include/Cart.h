#ifndef CART_H
#define CART_H

#include <QList>
#include <algorithm>

template<typename T>
class Cart {
public:
    void addItem(const T &item) {
        items.append(item);
    }

    void removeItem(CartItemWidget *item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
        }
    }

    double calculateTotalPrice() const {
        double total = 0;
        for (const auto &item: items) {
            total += item->getTotalPrice();
        }
        return total;
    }

    const QList<T> &getItems() const {
        return items;
    }

private:
    QList<T> items;
};

#endif // CART_H
