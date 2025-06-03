#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QPoint>

// Кастомный заголовок окна
class TitleBar : public QFrame {
    Q_OBJECT
    
public:
    TitleBar(QWidget *parent = nullptr);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    
private slots:
    void minimizeWindow();
    void maximizeWindow();
    void closeWindow();
    
private:
    QPoint dragPosition;
};

// Главное окно приложения
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    
private slots:
    void navigateTo(int index);
    
private:
    QStackedWidget *contentStack;
    QWidget *createDashboardPage();
    QWidget *createRecipientsPage();
    QWidget *createTemplatesPage();
    QWidget *createMediaPage();
    QWidget *createTasksPage();
    QWidget *createTelegramPage(); // Новый раздел для Telegram
    QWidget *createSubscriptionPage(); // Новый раздел для подписок
    QWidget *createSettingsPage();
};

#endif // MAINWINDOW_H
