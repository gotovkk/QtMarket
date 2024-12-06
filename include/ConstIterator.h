#ifndef CONSTITERATOR_H
#define CONSTITERATOR_H

#include <QList>

template<typename T>
class ConstIterator {
public:
    explicit ConstIterator(typename QList<T>::const_iterator it) : current(it) {}

    const T &operator*() const {
        return *current;
    }

    ConstIterator &operator++() {
        ++current;
        return *this;
    }

    ConstIterator operator++(int) {
        ConstIterator temp = *this;
        ++current;
        return temp;
    }

    bool operator!=(const ConstIterator &other) const {
        return current != other.current;
    }

    bool operator==(const ConstIterator &other) const {
        return current == other.current;
    }

private:
    typename QList<T>::const_iterator current;
};

#endif // CONSTITERATOR_H
