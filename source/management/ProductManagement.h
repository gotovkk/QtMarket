#pragma once
#include <memory>
#include "../../include/Product.h"
#include "../../include/Storage.h"

class ProductManagement {
public:

    virtual void addProduct(
            Storage& storage,
            std::unique_ptr<Product> product
    ) const = 0;

    virtual bool removeProduct(Storage& storage, int sellerId) const = 0;


    virtual void updateProduct(
            Storage& storage,
            int sellerId
    ) const = 0;

    virtual ~ProductManagement() = default;
};
