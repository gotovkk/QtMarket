#ifndef ITERATOR_H
#define ITERATOR_H

#include <QList>

template<typename T>
class Iterator {
public:
    explicit Iterator(typename QList<T>::iterator it) : current(it) {}

    T &operator*() {
        return *current;
    }

    Iterator &operator++() {
        ++current;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ++current;
        return temp;
    }

    bool operator!=(const Iterator &other) const {
        return current != other.current;
    }

    bool operator==(const Iterator &other) const {
        return current == other.current;
    }

    std::ptrdiff_t operator-(const Iterator &other) const {
        return std::distance(other.current, current);
    }

private:
    typename QList<T>::iterator current;
};

#endif // ITERATOR_H
