// SessionManager.h
#pragma once
#include "../../sqlite/sqlite3.h"

class SessionManager {
public:
    static int getCurrentUserId() {
        return currentUserId;
    }

    static void setCurrentUserId(int userId) {
        currentUserId = userId;
    }

    static void logout() {
        currentUserId = -1;
    }

private:
    static int currentUserId;
};
