#include "../include/telegram_api.h"
#include <QDebug>

class TelegramAPI::Private {
public:
    bool authorized = false;
    QString apiId;
    QString apiHash;
    QString phoneNumber;
    
    // Заглушки для демонстрации
    QVector<QMap<QString, QString>> mockContacts;
    QMap<QString, QMap<QString, QString>> mockTasks;
};

TelegramAPI::TelegramAPI(QObject *parent) : QObject(parent), d(new Private) {
    // Инициализация заглушек для демонстрации
    QMap<QString, QString> contact1;
    contact1["id"] = "user1";
    contact1["name"] = "Иван Петров";
    contact1["phone"] = "+79001234567";
    
    QMap<QString, QString> contact2;
    contact2["id"] = "user2";
    contact2["name"] = "Мария Сидорова";
    contact2["phone"] = "+79009876543";
    
    d->mockContacts.append(contact1);
    d->mockContacts.append(contact2);
}

TelegramAPI::~TelegramAPI() {
    delete d;
}

bool TelegramAPI::initialize(const QString &apiId, const QString &apiHash) {
    d->apiId = apiId;
    d->apiHash = apiHash;
    qDebug() << "Telegram API initialized with ID:" << apiId;
    return true;
}

bool TelegramAPI::login(const QString &phoneNumber) {
    d->phoneNumber = phoneNumber;
    qDebug() << "Login requested for phone:" << phoneNumber;
    emit loginCodeRequested();
    return true;
}

bool TelegramAPI::checkAuthCode(const QString &code) {
    qDebug() << "Auth code checked:" << code;
    if (code == "12345") { // Заглушка для демонстрации
        d->authorized = true;
        emit authStatusChanged(true);
        return true;
    } else {
        emit passwordRequested();
        return false;
    }
}

bool TelegramAPI::checkPassword(const QString &password) {
    qDebug() << "Password checked";
    if (password.length() > 0) { // Заглушка для демонстрации
        d->authorized = true;
        emit authStatusChanged(true);
        return true;
    }
    return false;
}

bool TelegramAPI::logout() {
    d->authorized = false;
    emit authStatusChanged(false);
    qDebug() << "Logged out from Telegram";
    return true;
}

bool TelegramAPI::isAuthorized() const {
    return d->authorized;
}

QVector<QMap<QString, QString>> TelegramAPI::getContacts() {
    if (!d->authorized) {
        return QVector<QMap<QString, QString>>();
    }
    return d->mockContacts;
}

bool TelegramAPI::importContacts(const QVector<QMap<QString, QString>> &contacts) {
    if (!d->authorized) {
        return false;
    }
    
    for (const auto &contact : contacts) {
        qDebug() << "Importing contact:" << contact["name"] << contact["phone"];
        d->mockContacts.append(contact);
    }
    
    return true;
}

bool TelegramAPI::sendMessage(const QString &userId, const QString &message) {
    if (!d->authorized) {
        return false;
    }
    
    qDebug() << "Sending message to" << userId << ":" << message;
    emit messageDelivered(userId, true);
    return true;
}

bool TelegramAPI::sendMediaMessage(const QString &userId, const QString &filePath, const QString &caption) {
    if (!d->authorized) {
        return false;
    }
    
    qDebug() << "Sending media to" << userId << ":" << filePath << "with caption:" << caption;
    emit messageDelivered(userId, true);
    return true;
}

bool TelegramAPI::createTask(const QString &name, const QVector<QString> &recipients, 
                           const QString &message, const QVector<QString> &media) {
    if (!d->authorized) {
        return false;
    }
    
    QString taskId = "task_" + QString::number(d->mockTasks.size() + 1);
    
    QMap<QString, QString> task;
    task["name"] = name;
    task["recipients"] = QString::number(recipients.size());
    task["message"] = message;
    task["media"] = QString::number(media.size());
    task["status"] = "created";
    task["progress"] = "0";
    
    d->mockTasks[taskId] = task;
    
    qDebug() << "Created task:" << taskId << name;
    return true;
}

bool TelegramAPI::startTask(const QString &taskId) {
    if (!d->authorized || !d->mockTasks.contains(taskId)) {
        return false;
    }
    
    d->mockTasks[taskId]["status"] = "running";
    emit taskStatusChanged(taskId, "running");
    
    // Эмуляция прогресса для демонстрации
    for (int i = 0; i <= 100; i += 10) {
        d->mockTasks[taskId]["progress"] = QString::number(i);
        emit taskProgressUpdated(taskId, i);
        // В реальном приложении здесь был бы асинхронный код
    }
    
    d->mockTasks[taskId]["status"] = "completed";
    emit taskStatusChanged(taskId, "completed");
    
    qDebug() << "Started task:" << taskId;
    return true;
}

bool TelegramAPI::pauseTask(const QString &taskId) {
    if (!d->authorized || !d->mockTasks.contains(taskId)) {
        return false;
    }
    
    d->mockTasks[taskId]["status"] = "paused";
    emit taskStatusChanged(taskId, "paused");
    
    qDebug() << "Paused task:" << taskId;
    return true;
}

bool TelegramAPI::stopTask(const QString &taskId) {
    if (!d->authorized || !d->mockTasks.contains(taskId)) {
        return false;
    }
    
    d->mockTasks[taskId]["status"] = "stopped";
    emit taskStatusChanged(taskId, "stopped");
    
    qDebug() << "Stopped task:" << taskId;
    return true;
}

bool TelegramAPI::deleteTask(const QString &taskId) {
    if (!d->authorized || !d->mockTasks.contains(taskId)) {
        return false;
    }
    
    d->mockTasks.remove(taskId);
    
    qDebug() << "Deleted task:" << taskId;
    return true;
}
