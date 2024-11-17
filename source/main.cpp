#include "../include/ui/mainwindow.h"
#include "../include/Storage.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    system("chcp 1251");
    sqlite3 *db;
    int rc;
    const char *dbPath = R"(C:\Users\anima\CLionProjects\QtHydraMarket\mydb.db)";

    rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка открытия бд" << sqlite3_errmsg(db) << std::endl;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
