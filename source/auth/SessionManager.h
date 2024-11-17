// SessionManager.h
#pragma once

class SessionManager {
public:
    static int getCurrentUserId() {
        return currentUserId;
    }

    static void setCurrentUserId(int userId) {
        currentUserId = userId;
    }

    static void logout() {
        currentUserId = -1;  // Очистить сессию
    }

private:
    static int currentUserId;  // Идентификатор текущего пользователя
};

