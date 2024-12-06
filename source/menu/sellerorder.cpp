
#include "../../include/ui/sellerorder.h"
#include "ui_sellerorder.h"

sellerorder::sellerorder(QWidget *parent) :
        QWidget(parent), ui(new Ui::sellerorder) {
    ui->setupUi(this);

    // Создание интерфейса
    ordersTable = new QTableWidget(this);
    ordersTable->setColumnCount(5);
    ordersTable->setHorizontalHeaderLabels({"ID", "Товар", "Количество", "Дата заказа", "Статус"});
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Установка стилей для таблицы
    ordersTable->setStyleSheet("QTableWidget {"
                               "border: 1px solid #ccc;"
                               "border-radius: 5px;"
                               "background-color: #f9f9f9;"
                               "font-family: Arial, sans-serif;"
                               "font-size: 12pt;"
                               "}"
                               "QHeaderView::section {"
                               "background-color: #2c3e50;"
                               "color: white;"
                               "padding: 8px;"
                               "font-size: 14pt;"
                               "}"
                               "QTableWidget::item {"
                               "padding: 8px;"
                               "border-bottom: 1px solid #ddd;"
                               "}");

    // Поиск и фильтры
    searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Поиск по названию товара");
    searchField->setStyleSheet("QLineEdit {"
                               "border: 1px solid #ccc;"
                               "border-radius: 10px;"
                               "padding: 5px;"
                               "font-size: 12pt;"
                               "}");

    statusFilter = new QComboBox(this);
    statusFilter->addItems({"Все", "Ожидает", "Выполнен", "Отменен"});
    statusFilter->setStyleSheet("QComboBox {"
                                "border: 1px solid #ccc;"
                                "border-radius: 10px;"
                                "padding: 5px;"
                                "font-size: 12pt;"
                                "}");

    startDateFilter = new QDateEdit(this);
    startDateFilter->setCalendarPopup(true);
    startDateFilter->setDisplayFormat("dd.MM.yyyy");
    startDateFilter->setStyleSheet("QDateEdit {"
                                   "border: 1px solid #ccc;"
                                   "border-radius: 10px;"
                                   "padding: 5px;"
                                   "font-size: 12pt;"
                                   "}");

    endDateFilter = new QDateEdit(this);
    endDateFilter->setCalendarPopup(true);
    endDateFilter->setDisplayFormat("dd.MM.yyyy");
    endDateFilter->setStyleSheet("QDateEdit {"
                                 "border: 1px solid #ccc;"
                                 "border-radius: 10px;"
                                 "padding: 5px;"
                                 "font-size: 12pt;"
                                 "}");

    // Кнопки с новым дизайном
    refreshButton = new QPushButton("Обновить", this);
    refreshButton->setStyleSheet("QPushButton {"
                                 "background-color: #bdc3c7;"
                                 "color: #fff;"
                                 "border-radius: 15px;"
                                 "padding: 10px;"
                                 "font-size: 14pt;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #95a5a6;"
                                 "}");

    markCompletedButton = new QPushButton("Пометить как выполненный", this);
    markCompletedButton->setStyleSheet("QPushButton {"
                                       "background-color: #bdc3c7;"
                                       "color: #fff;"
                                       "border-radius: 15px;"
                                       "padding: 10px;"
                                       "font-size: 14pt;"
                                       "}"
                                       "QPushButton:hover {"
                                       "background-color: #95a5a6;"
                                       "}");

    backButton = new QPushButton("Назад", this);
    backButton->setStyleSheet("QPushButton {"
                              "background-color: #bdc3c7;"
                              "color: #fff;"
                              "border-radius: 15px;"
                              "padding: 10px;"
                              "font-size: 14pt;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #95a5a6;"
                              "}");

    // Макет для фильтров
    auto *layout = new QVBoxLayout(this);
    auto *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Поиск:"));
    filterLayout->addWidget(searchField);
    filterLayout->addWidget(new QLabel("Статус:"));
    filterLayout->addWidget(statusFilter);
    filterLayout->addWidget(new QLabel("Дата с:"));
    filterLayout->addWidget(startDateFilter);
    filterLayout->addWidget(new QLabel("по:"));
    filterLayout->addWidget(endDateFilter);
    filterLayout->addWidget(refreshButton);

    layout->addLayout(filterLayout);
    layout->addWidget(ordersTable);
    layout->addWidget(markCompletedButton);
    layout->addWidget(backButton);

    // Связывание кнопок с функциями
    connect(refreshButton, &QPushButton::clicked, this, &sellerorder::refreshOrders);
    connect(markCompletedButton, &QPushButton::clicked, this, &sellerorder::markOrderAsCompleted);
    connect(backButton, &QPushButton::clicked, this, &sellerorder::backToSellerMenu);

    // Установка монотонного серого фона для всего окна
    setStyleSheet("QWidget {"
                  "background-color: #d3d3d3;"  // Серый цвет фона
                  "font-family: Arial, sans-serif;"
                  "}"
                  "QWidget#sellerorder {"
                  "background-color: #d3d3d3;"  // Серый цвет фона для виджета
                  "}");

    // Подгонка ширины столбцов
}

sellerorder::~sellerorder() {
    delete ui;
}


void sellerorder::backToSellerMenu() {
    qDebug() << "backButton был нажать, возвращаемся в меню продавца.";  // Отладка

    this->hide();  // Скрыть окно заказов
    emit backToMenu();  // Сигнал для возврата в меню продавца

    qDebug() << "Меню продавца должно быть теперь показано.";  // Отладка
}
void sellerorder::filterOrders() {
    qDebug() << "Фильтрация заказов";
}

void sellerorder::markOrderAsCompleted() {
    // Пометить выбранный заказ как выполненный
    qDebug() << "Заказ помечен как выполненный";
}

void sellerorder::refreshOrders() {
    // Обновить список заказов
    qDebug() << "Список заказов обновлен";
}
