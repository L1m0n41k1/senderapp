#include "../include/mainwindow.h"
#include <QApplication>
#include <QStackedWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QGroupBox>
#include <QScrollArea>
#include <QCheckBox>

// Реализация TitleBar
TitleBar::TitleBar(QWidget *parent) : QFrame(parent) {
    setObjectName("titleBar");
    setFixedHeight(40);
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(0);
    
    // Логотип и название приложения
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(QPixmap(":/icons/icon.png").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    QLabel *titleLabel = new QLabel("WhatsApp & Telegram Sender");
    titleLabel->setObjectName("titleLabel");
    
    // Кнопки управления окном
    QPushButton *minimizeButton = new QPushButton("_");
    minimizeButton->setObjectName("minimizeButton");
    minimizeButton->setFixedSize(30, 30);
    
    QPushButton *maximizeButton = new QPushButton("□");
    maximizeButton->setObjectName("maximizeButton");
    maximizeButton->setFixedSize(30, 30);
    
    QPushButton *closeButton = new QPushButton("×");
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(30, 30);
    
    // Добавление элементов в layout
    layout->addWidget(logoLabel);
    layout->addSpacing(5);
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);
    
    // Подключение сигналов
    connect(minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeWindow);
    connect(maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeWindow);
    connect(closeButton, &QPushButton::clicked, this, &TitleBar::closeWindow);
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - window()->frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        window()->move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void TitleBar::minimizeWindow() {
    window()->showMinimized();
}

void TitleBar::maximizeWindow() {
    if (window()->isMaximized()) {
        window()->showNormal();
    } else {
        window()->showMaximized();
    }
}

void TitleBar::closeWindow() {
    window()->close();
}

// Реализация MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Убираем стандартные рамки и заголовок окна
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    resize(1200, 800);
    
    // Создаем центральный виджет
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Добавляем кастомный заголовок
    TitleBar *titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);
    
    // Основной контент
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // Боковая панель навигации (как в Telegram/Discord)
    QFrame *sidebarFrame = new QFrame();
    sidebarFrame->setObjectName("sidebarFrame");
    sidebarFrame->setFixedWidth(250);
    
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(10, 10, 10, 10);
    sidebarLayout->setSpacing(5);
    
    // Кнопки навигации
    QPushButton *dashboardButton = new QPushButton("Дашборд");
    dashboardButton->setObjectName("navButton");
    
    QPushButton *recipientsButton = new QPushButton("Получатели");
    recipientsButton->setObjectName("navButton");
    
    QPushButton *templatesButton = new QPushButton("Шаблоны");
    templatesButton->setObjectName("navButton");
    
    QPushButton *mediaButton = new QPushButton("Медиафайлы");
    mediaButton->setObjectName("navButton");
    
    QPushButton *tasksButton = new QPushButton("Задания WhatsApp");
    tasksButton->setObjectName("navButton");
    
    QPushButton *telegramButton = new QPushButton("Telegram");
    telegramButton->setObjectName("navButton");
    
    QPushButton *subscriptionButton = new QPushButton("Подписка");
    subscriptionButton->setObjectName("navButton");
    
    QPushButton *settingsButton = new QPushButton("Настройки");
    settingsButton->setObjectName("navButton");
    
    // Добавление кнопок в боковую панель
    sidebarLayout->addWidget(dashboardButton);
    sidebarLayout->addWidget(recipientsButton);
    sidebarLayout->addWidget(templatesButton);
    sidebarLayout->addWidget(mediaButton);
    sidebarLayout->addWidget(tasksButton);
    sidebarLayout->addWidget(telegramButton);
    sidebarLayout->addWidget(subscriptionButton);
    sidebarLayout->addWidget(settingsButton);
    sidebarLayout->addStretch();
    
    // Создаем стек для контента
    contentStack = new QStackedWidget();
    contentStack->setObjectName("contentStack");
    
    // Создаем страницы для каждого раздела
    contentStack->addWidget(createDashboardPage());
    contentStack->addWidget(createRecipientsPage());
    contentStack->addWidget(createTemplatesPage());
    contentStack->addWidget(createMediaPage());
    contentStack->addWidget(createTasksPage());
    contentStack->addWidget(createTelegramPage());
    contentStack->addWidget(createSubscriptionPage());
    contentStack->addWidget(createSettingsPage());
    
    // Подключаем сигналы кнопок навигации
    connect(dashboardButton, &QPushButton::clicked, [this]() { navigateTo(0); });
    connect(recipientsButton, &QPushButton::clicked, [this]() { navigateTo(1); });
    connect(templatesButton, &QPushButton::clicked, [this]() { navigateTo(2); });
    connect(mediaButton, &QPushButton::clicked, [this]() { navigateTo(3); });
    connect(tasksButton, &QPushButton::clicked, [this]() { navigateTo(4); });
    connect(telegramButton, &QPushButton::clicked, [this]() { navigateTo(5); });
    connect(subscriptionButton, &QPushButton::clicked, [this]() { navigateTo(6); });
    connect(settingsButton, &QPushButton::clicked, [this]() { navigateTo(7); });
    
    // Добавление панелей в основной контент
    contentLayout->addWidget(sidebarFrame);
    contentLayout->addWidget(contentStack, 1);
    
    // Добавление контента в основной layout
    mainLayout->addLayout(contentLayout, 1);
    
    // Применение стилей
    setStyleSheet(
        "QMainWindow { background-color: #36393f; }"
        "QFrame#titleBar { background-color: #202225; }"
        "QLabel#titleLabel { color: white; font-size: 14px; font-weight: bold; }"
        "QPushButton#minimizeButton, QPushButton#maximizeButton, QPushButton#closeButton {"
        "    background-color: transparent; color: #dcddde; border: none; font-size: 16px; }"
        "QPushButton#closeButton:hover { background-color: #f04747; color: white; }"
        "QPushButton#minimizeButton:hover, QPushButton#maximizeButton:hover { background-color: #40444b; }"
        "QFrame#sidebarFrame { background-color: #2f3136; }"
        "QPushButton#navButton {"
        "    background-color: transparent; color: #dcddde; text-align: left; padding: 10px;"
        "    border: none; border-radius: 4px; font-size: 14px; }"
        "QPushButton#navButton:hover { background-color: #40444b; }"
        "QStackedWidget#contentStack { background-color: #36393f; }"
        "QLabel#pageTitle { color: white; font-size: 24px; font-weight: bold; margin-bottom: 10px; }"
        "QLabel { color: #dcddde; }"
        "QPushButton {"
        "    background-color: #7289da; color: white; border: none; border-radius: 4px;"
        "    padding: 8px 16px; font-size: 14px; }"
        "QPushButton:hover { background-color: #677bc4; }"
        "QLineEdit, QTextEdit, QComboBox {"
        "    background-color: #40444b; color: #dcddde; border: 1px solid #202225;"
        "    border-radius: 4px; padding: 8px; }"
        "QTableView {"
        "    background-color: #2f3136; color: #dcddde; border: none;"
        "    gridline-color: #40444b; }"
        "QHeaderView::section {"
        "    background-color: #202225; color: white; padding: 8px;"
        "    border: none; }"
        "QScrollBar:vertical {"
        "    background-color: #2f3136; width: 12px; margin: 0; }"
        "QScrollBar::handle:vertical {"
        "    background-color: #40444b; min-height: 20px; border-radius: 6px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none; }"
        "QGroupBox {"
        "    border: 1px solid #40444b; border-radius: 4px; margin-top: 20px; padding-top: 15px; }"
        "QGroupBox::title {"
        "    subcontrol-origin: margin; subcontrol-position: top left;"
        "    padding: 0 5px; color: white; }"
        "QFrame#premiumBadge {"
        "    background-color: #faa61a; color: white; border-radius: 4px; }"
        "QLabel#premiumLabel {"
        "    color: white; font-weight: bold; }"
    );
    
    // По умолчанию показываем дашборд
    navigateTo(0);
}

void MainWindow::navigateTo(int index) {
    contentStack->setCurrentIndex(index);
}

QWidget* MainWindow::createDashboardPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Дашборд");
    titleLabel->setObjectName("pageTitle");
    
    QFrame *statsFrame = new QFrame();
    statsFrame->setObjectName("statsFrame");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsFrame);
    
    // Статистические карточки
    QFrame *recipientsCard = new QFrame();
    recipientsCard->setObjectName("statsCard");
    QVBoxLayout *recipientsCardLayout = new QVBoxLayout(recipientsCard);
    QLabel *recipientsCountLabel = new QLabel("0");
    recipientsCountLabel->setObjectName("statsNumber");
    QLabel *recipientsTextLabel = new QLabel("Получателей");
    recipientsCardLayout->addWidget(recipientsCountLabel);
    recipientsCardLayout->addWidget(recipientsTextLabel);
    
    QFrame *templatesCard = new QFrame();
    templatesCard->setObjectName("statsCard");
    QVBoxLayout *templatesCardLayout = new QVBoxLayout(templatesCard);
    QLabel *templatesCountLabel = new QLabel("0");
    templatesCountLabel->setObjectName("statsNumber");
    QLabel *templatesTextLabel = new QLabel("Шаблонов");
    templatesCardLayout->addWidget(templatesCountLabel);
    templatesCardLayout->addWidget(templatesTextLabel);
    
    QFrame *whatsappCard = new QFrame();
    whatsappCard->setObjectName("statsCard");
    QVBoxLayout *whatsappCardLayout = new QVBoxLayout(whatsappCard);
    QLabel *whatsappCountLabel = new QLabel("0");
    whatsappCountLabel->setObjectName("statsNumber");
    QLabel *whatsappTextLabel = new QLabel("WhatsApp сообщений");
    whatsappCardLayout->addWidget(whatsappCountLabel);
    whatsappCardLayout->addWidget(whatsappTextLabel);
    
    QFrame *telegramCard = new QFrame();
    telegramCard->setObjectName("statsCard");
    QVBoxLayout *telegramCardLayout = new QVBoxLayout(telegramCard);
    QLabel *telegramCountLabel = new QLabel("0");
    telegramCountLabel->setObjectName("statsNumber");
    QLabel *telegramTextLabel = new QLabel("Telegram сообщений");
    telegramCardLayout->addWidget(telegramCountLabel);
    telegramCardLayout->addWidget(telegramTextLabel);
    
    statsLayout->addWidget(recipientsCard);
    statsLayout->addWidget(templatesCard);
    statsLayout->addWidget(whatsappCard);
    statsLayout->addWidget(telegramCard);
    
    // Последние задания
    QLabel *recentTasksLabel = new QLabel("Последние задания");
    recentTasksLabel->setObjectName("sectionTitle");
    
    QTableView *recentTasksTable = new QTableView();
    recentTasksTable->setObjectName("recentTasksTable");
    QStandardItemModel *model = new QStandardItemModel(0, 5, recentTasksTable);
    model->setHorizontalHeaderLabels({"Название", "Платформа", "Статус", "Получателей", "Дата создания"});
    recentTasksTable->setModel(model);
    recentTasksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Кнопки быстрого доступа
    QLabel *quickActionsLabel = new QLabel("Быстрые действия");
    quickActionsLabel->setObjectName("sectionTitle");
    
    QHBoxLayout *quickActionsLayout = new QHBoxLayout();
    
    QPushButton *newRecipientButton = new QPushButton("Новый получатель");
    QPushButton *newTemplateButton = new QPushButton("Новый шаблон");
    QPushButton *newWhatsAppTaskButton = new QPushButton("Задание WhatsApp");
    QPushButton *newTelegramTaskButton = new QPushButton("Задание Telegram");
    
    quickActionsLayout->addWidget(newRecipientButton);
    quickActionsLayout->addWidget(newTemplateButton);
    quickActionsLayout->addWidget(newWhatsAppTaskButton);
    quickActionsLayout->addWidget(newTelegramTaskButton);
    
    // Информация о подписке
    QFrame *subscriptionFrame = new QFrame();
    subscriptionFrame->setObjectName("subscriptionFrame");
    QHBoxLayout *subscriptionLayout = new QHBoxLayout(subscriptionFrame);
    
    QLabel *subscriptionLabel = new QLabel("Текущая подписка: Базовая");
    QLabel *limitLabel = new QLabel("Осталось сообщений: 50/100");
    QPushButton *upgradeButton = new QPushButton("Улучшить план");
    upgradeButton->setObjectName("upgradeButton");
    
    subscriptionLayout->addWidget(subscriptionLabel);
    subscriptionLayout->addWidget(limitLabel);
    subscriptionLayout->addStretch();
    subscriptionLayout->addWidget(upgradeButton);
    
    layout->addWidget(titleLabel);
    layout->addWidget(statsFrame);
    layout->addSpacing(20);
    layout->addWidget(recentTasksLabel);
    layout->addWidget(recentTasksTable);
    layout->addSpacing(20);
    layout->addWidget(quickActionsLabel);
    layout->addLayout(quickActionsLayout);
    layout->addSpacing(20);
    layout->addWidget(subscriptionFrame);
    layout->addStretch();
    
    return page;
}

QWidget* MainWindow::createRecipientsPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Получатели");
    titleLabel->setObjectName("pageTitle");
    
    // Панель инструментов
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *addButton = new QPushButton("Добавить");
    QPushButton *importButton = new QPushButton("Импорт");
    QPushButton *exportButton = new QPushButton("Экспорт");
    QPushButton *deleteButton = new QPushButton("Удалить");
    
    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Поиск получателей...");
    searchInput->setFixedWidth(300);
    
    toolbarLayout->addWidget(addButton);
    toolbarLayout->addWidget(importButton);
    toolbarLayout->addWidget(exportButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(searchInput);
    
    // Таблица получателей
    QTableView *recipientsTable = new QTableView();
    recipientsTable->setObjectName("recipientsTable");
    QStandardItemModel *model = new QStandardItemModel(0, 5, recipientsTable);
    model->setHorizontalHeaderLabels({"Имя", "Телефон", "Telegram", "Группа", "Примечание"});
    recipientsTable->setModel(model);
    recipientsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addWidget(titleLabel);
    layout->addLayout(toolbarLayout);
    layout->addWidget(recipientsTable);
    
    return page;
}

QWidget* MainWindow::createTemplatesPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Шаблоны сообщений");
    titleLabel->setObjectName("pageTitle");
    
    // Панель инструментов
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *addButton = new QPushButton("Добавить");
    QPushButton *editButton = new QPushButton("Редактировать");
    QPushButton *deleteButton = new QPushButton("Удалить");
    
    QComboBox *platformCombo = new QComboBox();
    platformCombo->addItems({"Все платформы", "WhatsApp", "Telegram"});
    platformCombo->setFixedWidth(150);
    
    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Поиск шаблонов...");
    searchInput->setFixedWidth(300);
    
    toolbarLayout->addWidget(addButton);
    toolbarLayout->addWidget(editButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(platformCombo);
    toolbarLayout->addWidget(searchInput);
    
    // Таблица шаблонов
    QTableView *templatesTable = new QTableView();
    templatesTable->setObjectName("templatesTable");
    QStandardItemModel *model = new QStandardItemModel(0, 4, templatesTable);
    model->setHorizontalHeaderLabels({"Название", "Платформа", "Текст", "Медиафайлы"});
    templatesTable->setModel(model);
    templatesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addWidget(titleLabel);
    layout->addLayout(toolbarLayout);
    layout->addWidget(templatesTable);
    
    return page;
}

QWidget* MainWindow::createMediaPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Медиафайлы");
    titleLabel->setObjectName("pageTitle");
    
    // Панель инструментов
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *uploadButton = new QPushButton("Загрузить");
    QPushButton *deleteButton = new QPushButton("Удалить");
    
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Все типы", "Изображения", "Видео", "Документы"});
    typeCombo->setFixedWidth(150);
    
    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Поиск файлов...");
    searchInput->setFixedWidth(300);
    
    toolbarLayout->addWidget(uploadButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(typeCombo);
    toolbarLayout->addWidget(searchInput);
    
    // Сетка медиафайлов (заглушка)
    QFrame *mediaGrid = new QFrame();
    mediaGrid->setObjectName("mediaGrid");
    QGridLayout *gridLayout = new QGridLayout(mediaGrid);
    gridLayout->setSpacing(10);
    
    // Заглушки для медиафайлов
    for (int i = 0; i < 12; i++) {
        QFrame *mediaItem = new QFrame();
        mediaItem->setObjectName("mediaItem");
        mediaItem->setFixedSize(150, 150);
        
        QVBoxLayout *mediaItemLayout = new QVBoxLayout(mediaItem);
        QLabel *mediaIcon = new QLabel("📁");
        mediaIcon->setAlignment(Qt::AlignCenter);
        mediaIcon->setObjectName("mediaIcon");
        
        QLabel *mediaName = new QLabel("file_" + QString::number(i+1) + ".jpg");
        mediaName->setAlignment(Qt::AlignCenter);
        
        mediaItemLayout->addWidget(mediaIcon);
        mediaItemLayout->addWidget(mediaName);
        
        gridLayout->addWidget(mediaItem, i/4, i%4);
    }
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(mediaGrid);
    
    layout->addWidget(titleLabel);
    layout->addLayout(toolbarLayout);
    layout->addWidget(scrollArea);
    
    return page;
}

QWidget* MainWindow::createTasksPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Задания WhatsApp");
    titleLabel->setObjectName("pageTitle");
    
    // Панель инструментов
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *addButton = new QPushButton("Новое задание");
    QPushButton *startButton = new QPushButton("Запустить");
    QPushButton *pauseButton = new QPushButton("Пауза");
    QPushButton *stopButton = new QPushButton("Остановить");
    QPushButton *deleteButton = new QPushButton("Удалить");
    
    toolbarLayout->addWidget(addButton);
    toolbarLayout->addWidget(startButton);
    toolbarLayout->addWidget(pauseButton);
    toolbarLayout->addWidget(stopButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addStretch();
    
    // Таблица заданий
    QTableView *tasksTable = new QTableView();
    tasksTable->setObjectName("tasksTable");
    QStandardItemModel *model = new QStandardItemModel(0, 5, tasksTable);
    model->setHorizontalHeaderLabels({"Название", "Статус", "Прогресс", "Получателей", "Дата создания"});
    tasksTable->setModel(model);
    tasksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addWidget(titleLabel);
    layout->addLayout(toolbarLayout);
    layout->addWidget(tasksTable);
    
    return page;
}

QWidget* MainWindow::createTelegramPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Telegram");
    titleLabel->setObjectName("pageTitle");
    
    // Секция авторизации
    QGroupBox *authGroup = new QGroupBox("Авторизация Telegram");
    QVBoxLayout *authLayout = new QVBoxLayout(authGroup);
    
    QLabel *statusLabel = new QLabel("Статус: Не авторизован");
    QPushButton *authButton = new QPushButton("Авторизоваться в Telegram");
    QPushButton *logoutButton = new QPushButton("Выйти");
    logoutButton->setEnabled(false);
    
    authLayout->addWidget(statusLabel);
    authLayout->addWidget(authButton);
    authLayout->addWidget(logoutButton);
    
    // Панель инструментов для заданий
    QLabel *tasksLabel = new QLabel("Задания Telegram");
    tasksLabel->setObjectName("sectionTitle");
    
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *addButton = new QPushButton("Новое задание");
    QPushButton *startButton = new QPushButton("Запустить");
    QPushButton *pauseButton = new QPushButton("Пауза");
    QPushButton *stopButton = new QPushButton("Остановить");
    QPushButton *deleteButton = new QPushButton("Удалить");
    
    toolbarLayout->addWidget(addButton);
    toolbarLayout->addWidget(startButton);
    toolbarLayout->addWidget(pauseButton);
    toolbarLayout->addWidget(stopButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addStretch();
    
    // Таблица заданий
    QTableView *tasksTable = new QTableView();
    tasksTable->setObjectName("tasksTable");
    QStandardItemModel *model = new QStandardItemModel(0, 5, tasksTable);
    model->setHorizontalHeaderLabels({"Название", "Статус", "Прогресс", "Получателей", "Дата создания"});
    tasksTable->setModel(model);
    tasksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Премиум-функции
    QFrame *premiumFrame = new QFrame();
    premiumFrame->setObjectName("premiumFrame");
    QVBoxLayout *premiumLayout = new QVBoxLayout(premiumFrame);
    
    QLabel *premiumTitle = new QLabel("Премиум-функции Telegram");
    premiumTitle->setObjectName("sectionTitle");
    
    QFrame *premiumFeatures = new QFrame();
    QGridLayout *featuresLayout = new QGridLayout(premiumFeatures);
    
    // Премиум-функции (заблокированные для базовой версии)
    QCheckBox *schedulingCheck = new QCheckBox("Планирование отправки");
    schedulingCheck->setEnabled(false);
    
    QCheckBox *bulkCheck = new QCheckBox("Массовая отправка в группы");
    bulkCheck->setEnabled(false);
    
    QCheckBox *analyticsCheck = new QCheckBox("Расширенная аналитика");
    analyticsCheck->setEnabled(false);
    
    QCheckBox *automationCheck = new QCheckBox("Автоматизация ответов");
    automationCheck->setEnabled(false);
    
    featuresLayout->addWidget(schedulingCheck, 0, 0);
    featuresLayout->addWidget(bulkCheck, 0, 1);
    featuresLayout->addWidget(analyticsCheck, 1, 0);
    featuresLayout->addWidget(automationCheck, 1, 1);
    
    QPushButton *unlockButton = new QPushButton("Разблокировать премиум-функции");
    
    premiumLayout->addWidget(premiumTitle);
    premiumLayout->addWidget(premiumFeatures);
    premiumLayout->addWidget(unlockButton);
    
    layout->addWidget(titleLabel);
    layout->addWidget(authGroup);
    layout->addSpacing(20);
    layout->addWidget(tasksLabel);
    layout->addLayout(toolbarLayout);
    layout->addWidget(tasksTable);
    layout->addSpacing(20);
    layout->addWidget(premiumFrame);
    
    return page;
}

QWidget* MainWindow::createSubscriptionPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Подписка");
    titleLabel->setObjectName("pageTitle");
    
    // Текущая подписка
    QGroupBox *currentGroup = new QGroupBox("Текущая подписка");
    QVBoxLayout *currentLayout = new QVBoxLayout(currentGroup);
    
    QLabel *planLabel = new QLabel("План: Базовый");
    QLabel *expiryLabel = new QLabel("Срок действия: Бессрочно");
    QLabel *limitLabel = new QLabel("Лимит сообщений: 100 в месяц");
    QLabel *usageLabel = new QLabel("Использовано: 50 сообщений");
    
    currentLayout->addWidget(planLabel);
    currentLayout->addWidget(expiryLabel);
    currentLayout->addWidget(limitLabel);
    currentLayout->addWidget(usageLabel);
    
    // Планы подписки
    QLabel *plansLabel = new QLabel("Доступные планы");
    plansLabel->setObjectName("sectionTitle");
    
    // Базовый план
    QFrame *basicFrame = new QFrame();
    basicFrame->setObjectName("planFrame");
    QVBoxLayout *basicLayout = new QVBoxLayout(basicFrame);
    
    QLabel *basicTitle = new QLabel("Базовый");
    basicTitle->setObjectName("planTitle");
    QLabel *basicPrice = new QLabel("Бесплатно");
    basicPrice->setObjectName("planPrice");
    
    QLabel *basicFeatures = new QLabel(
        "• 100 сообщений в месяц\n"
        "• Базовые шаблоны\n"
        "• Только WhatsApp\n"
        "• Базовая статистика"
    );
    
    QPushButton *basicButton = new QPushButton("Текущий план");
    basicButton->setEnabled(false);
    
    basicLayout->addWidget(basicTitle);
    basicLayout->addWidget(basicPrice);
    basicLayout->addWidget(basicFeatures);
    basicLayout->addStretch();
    basicLayout->addWidget(basicButton);
    
    // Премиум план
    QFrame *premiumFrame = new QFrame();
    premiumFrame->setObjectName("planFrame");
    QVBoxLayout *premiumLayout = new QVBoxLayout(premiumFrame);
    
    QLabel *premiumTitle = new QLabel("Премиум");
    premiumTitle->setObjectName("planTitle");
    QLabel *premiumPrice = new QLabel("$9.99 / месяц");
    premiumPrice->setObjectName("planPrice");
    
    QLabel *premiumFeatures = new QLabel(
        "• 1000 сообщений в месяц\n"
        "• Расширенные шаблоны\n"
        "• WhatsApp + Telegram\n"
        "• Расширенная аналитика\n"
        "• Планирование отправки\n"
        "• Приоритетная поддержка"
    );
    
    QPushButton *premiumButton = new QPushButton("Перейти на Премиум");
    
    premiumLayout->addWidget(premiumTitle);
    premiumLayout->addWidget(premiumPrice);
    premiumLayout->addWidget(premiumFeatures);
    premiumLayout->addStretch();
    premiumLayout->addWidget(premiumButton);
    
    // Бизнес план
    QFrame *businessFrame = new QFrame();
    businessFrame->setObjectName("planFrame");
    QVBoxLayout *businessLayout = new QVBoxLayout(businessFrame);
    
    QLabel *businessTitle = new QLabel("Бизнес");
    businessTitle->setObjectName("planTitle");
    QLabel *businessPrice = new QLabel("$29.99 / месяц");
    businessPrice->setObjectName("planPrice");
    
    QLabel *businessFeatures = new QLabel(
        "• Неограниченные сообщения\n"
        "• Все шаблоны и функции\n"
        "• WhatsApp + Telegram\n"
        "• Полная аналитика и отчеты\n"
        "• API для интеграции\n"
        "• Многопользовательский доступ\n"
        "• Выделенная поддержка"
    );
    
    QPushButton *businessButton = new QPushButton("Перейти на Бизнес");
    
    businessLayout->addWidget(businessTitle);
    businessLayout->addWidget(businessPrice);
    businessLayout->addWidget(businessFeatures);
    businessLayout->addStretch();
    businessLayout->addWidget(businessButton);
    
    // Корпоративный план
    QFrame *enterpriseFrame = new QFrame();
    enterpriseFrame->setObjectName("planFrame");
    QVBoxLayout *enterpriseLayout = new QVBoxLayout(enterpriseFrame);
    
    QLabel *enterpriseTitle = new QLabel("Корпоративный");
    enterpriseTitle->setObjectName("planTitle");
    QLabel *enterprisePrice = new QLabel("Индивидуально");
    enterprisePrice->setObjectName("planPrice");
    
    QLabel *enterpriseFeatures = new QLabel(
        "• Индивидуальные лимиты\n"
        "• Все функции Бизнес-плана\n"
        "• Индивидуальная настройка\n"
        "• Интеграция с CRM\n"
        "• Выделенный менеджер\n"
        "• SLA и расширенная поддержка"
    );
    
    QPushButton *enterpriseButton = new QPushButton("Связаться с отделом продаж");
    
    enterpriseLayout->addWidget(enterpriseTitle);
    enterpriseLayout->addWidget(enterprisePrice);
    enterpriseLayout->addWidget(enterpriseFeatures);
    enterpriseLayout->addStretch();
    enterpriseLayout->addWidget(enterpriseButton);
    
    // Размещение планов в сетке
    QHBoxLayout *plansLayout = new QHBoxLayout();
    plansLayout->addWidget(basicFrame);
    plansLayout->addWidget(premiumFrame);
    plansLayout->addWidget(businessFrame);
    plansLayout->addWidget(enterpriseFrame);
    
    layout->addWidget(titleLabel);
    layout->addWidget(currentGroup);
    layout->addSpacing(20);
    layout->addWidget(plansLabel);
    layout->addLayout(plansLayout);
    
    return page;
}

QWidget* MainWindow::createSettingsPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *titleLabel = new QLabel("Настройки");
    titleLabel->setObjectName("pageTitle");
    
    // Группа настроек WhatsApp
    QGroupBox *whatsappGroup = new QGroupBox("Настройки WhatsApp");
    QFormLayout *whatsappLayout = new QFormLayout(whatsappGroup);
    
    QLineEdit *delayInput = new QLineEdit("5");
    QComboBox *browserCombo = new QComboBox();
    browserCombo->addItems({"Chrome", "Firefox", "Edge"});
    QCheckBox *headlessCheck = new QCheckBox();
    
    whatsappLayout->addRow("Задержка между сообщениями (сек):", delayInput);
    whatsappLayout->addRow("Браузер:", browserCombo);
    whatsappLayout->addRow("Скрытый режим:", headlessCheck);
    
    // Группа настроек Telegram
    QGroupBox *telegramGroup = new QGroupBox("Настройки Telegram");
    QFormLayout *telegramLayout = new QFormLayout(telegramGroup);
    
    QLineEdit *apiIdInput = new QLineEdit();
    QLineEdit *apiHashInput = new QLineEdit();
    QLineEdit *telegramDelayInput = new QLineEdit("3");
    
    telegramLayout->addRow("API ID:", apiIdInput);
    telegramLayout->addRow("API Hash:", apiHashInput);
    telegramLayout->addRow("Задержка между сообщениями (сек):", telegramDelayInput);
    
    // Группа настроек приложения
    QGroupBox *appGroup = new QGroupBox("Настройки приложения");
    QFormLayout *appLayout = new QFormLayout(appGroup);
    
    QComboBox *themeCombo = new QComboBox();
    themeCombo->addItems({"Темная", "Светлая"});
    QComboBox *langCombo = new QComboBox();
    langCombo->addItems({"Русский", "English"});
    QCheckBox *startupCheck = new QCheckBox();
    QCheckBox *updateCheck = new QCheckBox();
    updateCheck->setChecked(true);
    
    appLayout->addRow("Тема:", themeCombo);
    appLayout->addRow("Язык:", langCombo);
    appLayout->addRow("Запускать при старте системы:", startupCheck);
    appLayout->addRow("Автоматически проверять обновления:", updateCheck);
    
    // Кнопки
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *cancelButton = new QPushButton("Отмена");
    
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(saveButton);
    
    layout->addWidget(titleLabel);
    layout->addWidget(whatsappGroup);
    layout->addWidget(telegramGroup);
    layout->addWidget(appGroup);
    layout->addStretch();
    layout->addLayout(buttonsLayout);
    
    return page;
}

// Точка входа в приложение
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
