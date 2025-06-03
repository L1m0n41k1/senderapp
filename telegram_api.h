#ifndef TELEGRAM_API_H
#define TELEGRAM_API_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>

// Класс для работы с Telegram API
class TelegramAPI : public QObject {
    Q_OBJECT
    
public:
    explicit TelegramAPI(QObject *parent = nullptr);
    ~TelegramAPI();
    
    // Методы авторизации
    bool initialize(const QString &apiId, const QString &apiHash);
    bool login(const QString &phoneNumber);
    bool checkAuthCode(const QString &code);
    bool checkPassword(const QString &password);
    bool logout();
    bool isAuthorized() const;
    
    // Методы для работы с контактами
    QVector<QMap<QString, QString>> getContacts();
    bool importContacts(const QVector<QMap<QString, QString>> &contacts);
    
    // Методы для отправки сообщений
    bool sendMessage(const QString &userId, const QString &message);
    bool sendMediaMessage(const QString &userId, const QString &filePath, const QString &caption = "");
    
    // Методы для работы с заданиями
    bool createTask(const QString &name, const QVector<QString> &recipients, 
                   const QString &message, const QVector<QString> &media = {});
    bool startTask(const QString &taskId);
    bool pauseTask(const QString &taskId);
    bool stopTask(const QString &taskId);
    bool deleteTask(const QString &taskId);
    
signals:
    void authStatusChanged(bool authorized);
    void loginCodeRequested();
    void passwordRequested();
    void messageDelivered(const QString &userId, bool success);
    void taskProgressUpdated(const QString &taskId, int progress);
    void taskStatusChanged(const QString &taskId, const QString &status);
    
private:
    class Private;
    Private *d;
};

#endif // TELEGRAM_API_H
