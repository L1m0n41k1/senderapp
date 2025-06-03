#ifndef SUBSCRIPTION_MANAGER_H
#define SUBSCRIPTION_MANAGER_H

#include <QObject>
#include <QString>
#include <QDate>

// Типы подписок
enum class SubscriptionPlan {
    Basic,
    Premium,
    Business,
    Enterprise
};

// Класс для управления подписками
class SubscriptionManager : public QObject {
    Q_OBJECT
    
public:
    explicit SubscriptionManager(QObject *parent = nullptr);
    ~SubscriptionManager();
    
    // Методы для работы с подписками
    SubscriptionPlan getCurrentPlan() const;
    QDate getExpiryDate() const;
    int getMessageLimit() const;
    int getMessageUsage() const;
    
    // Проверка доступности функций
    bool isTelegramAvailable() const;
    bool isSchedulingAvailable() const;
    bool isBulkMessagingAvailable() const;
    bool isAdvancedAnalyticsAvailable() const;
    bool isApiAccessAvailable() const;
    bool isMultiUserAvailable() const;
    
    // Методы для обновления подписки
    bool upgradeToPremium(const QString &paymentMethod, const QString &paymentDetails);
    bool upgradeToBusiness(const QString &paymentMethod, const QString &paymentDetails);
    bool requestEnterprisePlan(const QString &companyName, const QString &contactEmail);
    
    // Методы для отслеживания использования
    void incrementMessageCount(int count = 1);
    bool canSendMessage() const;
    
signals:
    void planChanged(SubscriptionPlan newPlan);
    void messageUsageUpdated(int used, int total);
    void subscriptionExpiring(int daysLeft);
    
private:
    class Private;
    Private *d;
};

#endif // SUBSCRIPTION_MANAGER_H
