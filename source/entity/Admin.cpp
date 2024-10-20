//
// Created by anima on 18.10.2024.
//

#include "../../include/entity/Admin.h"


void Admin::approveSeller([[maybe_unused]] const Storage& storage, [[maybe_unused]] int sellerId) const {
    std::cout << "Админ " << User::getUsername() << " зашел под аккаунтом продавца с ID: " << sellerId << std::endl;
}