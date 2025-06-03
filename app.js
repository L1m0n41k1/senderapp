// Main application JavaScript for WhatsApp Sender Desktop

// Глобальные переменные
let currentSection = 'accounts';
let accounts = [];
let recipients = [];
let templates = [];
let mediaFiles = [];
let tasks = [];
let settings = {
    delayBetweenMessages: 5,
    maxMessagesPerDay: 100
};

// Инициализация приложения
document.addEventListener('DOMContentLoaded', () => {
    // Проверяем доступность API
    if (!window.api) {
        showError('Ошибка инициализации', 'API не доступен. Пожалуйста, перезапустите приложение.');
        return;
    }

    // Инициализируем обработчики событий
    initEventListeners();
    
    // Загружаем данные
    loadData();
    
    // Показываем уведомление о запуске
    showNotification('Приложение запущено', 'WhatsApp Sender Desktop успешно инициализирован.');
});

// Инициализация обработчиков событий
function initEventListeners() {
    // Навигация по разделам
    document.querySelectorAll('.nav-item').forEach(item => {
        item.addEventListener('click', () => {
            const section = item.getAttribute('data-section');
            switchSection(section);
        });
    });
    
    // Кнопки добавления
    document.getElementById('add-account-btn').addEventListener('click', showAddAccountModal);
    document.getElementById('add-recipient-btn').addEventListener('click', showAddRecipientModal);
    document.getElementById('add-template-btn').addEventListener('click', showAddTemplateModal);
    document.getElementById('add-media-btn').addEventListener('click', showAddMediaModal);
    document.getElementById('add-task-btn').addEventListener('click', showAddTaskModal);
    
    // Кнопка импорта получателей
    document.getElementById('import-recipients-btn').addEventListener('click', showImportRecipientsModal);
    
    // Кнопка сохранения настроек
    document.getElementById('save-settings-btn').addEventListener('click', saveSettings);
    
    // Закрытие модальных окон
    document.querySelectorAll('.close-modal-btn, .cancel-btn').forEach(btn => {
        btn.addEventListener('click', closeAllModals);
    });
    
    // Сохранение аккаунта
    document.getElementById('save-account-btn').addEventListener('click', saveAccount);
    
    // Обновление QR-кода
    document.getElementById('refresh-qr-btn').addEventListener('click', refreshQRCode);
    
    // Обработчик IPC событий
    window.api.onNotification(handleNotification);
}

// Загрузка данных
async function loadData() {
    try {
        // Загружаем аккаунты
        accounts = await window.api.getAccounts();
        renderAccounts();
        
        // Загружаем получателей
        recipients = await window.api.getRecipients();
        renderRecipients();
        
        // Загружаем шаблоны
        templates = await window.api.getTemplates();
        renderTemplates();
        
        // Загружаем медиафайлы
        mediaFiles = await window.api.getMediaFiles();
        renderMediaFiles();
        
        // Загружаем задания
        tasks = await window.api.getJobs();
        renderTasks();
        
        // Загружаем настройки
        settings = await window.api.getSettings() || settings;
        renderSettings();
    } catch (error) {
        console.error('Error loading data:', error);
        showError('Ошибка загрузки данных', error.message || 'Не удалось загрузить данные');
    }
}

// Переключение между разделами
function switchSection(section) {
    // Обновляем активный элемент меню
    document.querySelectorAll('.nav-item').forEach(item => {
        if (item.getAttribute('data-section') === section) {
            item.classList.add('active');
        } else {
            item.classList.remove('active');
        }
    });
    
    // Обновляем активный раздел
    document.querySelectorAll('.content-section').forEach(section => {
        section.classList.remove('active');
    });
    document.getElementById(section).classList.add('active');
    
    // Обновляем текущий раздел
    currentSection = section;
}

// Отображение аккаунтов
function renderAccounts() {
    const accountsList = document.querySelector('.accounts-list');
    
    if (accounts.length === 0) {
        accountsList.innerHTML = `
            <div class="empty-state">
                <p>Нет добавленных аккаунтов</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    accounts.forEach(account => {
        html += `
            <div class="list-item" data-id="${account.id}">
                <div class="list-item-content">
                    <div class="list-item-title">
                        <span class="status-indicator ${account.status === 'online' ? 'online' : 'offline'}"></span>
                        ${account.name}
                    </div>
                    <div class="list-item-subtitle">${account.phone}</div>
                </div>
                <div class="list-item-actions">
                    <button class="secondary-btn login-account-btn" data-id="${account.id}">Войти</button>
                    <button class="secondary-btn edit-account-btn" data-id="${account.id}">Изменить</button>
                    <button class="secondary-btn delete-account-btn" data-id="${account.id}">Удалить</button>
                </div>
            </div>
        `;
    });
    
    accountsList.innerHTML = html;
    
    // Добавляем обработчики событий для кнопок
    document.querySelectorAll('.login-account-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            loginAccount(id);
        });
    });
    
    document.querySelectorAll('.edit-account-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            editAccount(id);
        });
    });
    
    document.querySelectorAll('.delete-account-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            deleteAccount(id);
        });
    });
}

// Отображение получателей
function renderRecipients() {
    const recipientsList = document.querySelector('.recipients-list');
    
    if (recipients.length === 0) {
        recipientsList.innerHTML = `
            <div class="empty-state">
                <p>Нет добавленных получателей</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    recipients.forEach(recipient => {
        html += `
            <div class="list-item" data-id="${recipient.id}">
                <div class="list-item-content">
                    <div class="list-item-title">${recipient.name}</div>
                    <div class="list-item-subtitle">${recipient.phone}</div>
                </div>
                <div class="list-item-actions">
                    <button class="secondary-btn edit-recipient-btn" data-id="${recipient.id}">Изменить</button>
                    <button class="secondary-btn delete-recipient-btn" data-id="${recipient.id}">Удалить</button>
                </div>
            </div>
        `;
    });
    
    recipientsList.innerHTML = html;
    
    // Добавляем обработчики событий для кнопок
    document.querySelectorAll('.edit-recipient-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            editRecipient(id);
        });
    });
    
    document.querySelectorAll('.delete-recipient-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            deleteRecipient(id);
        });
    });
}

// Отображение шаблонов
function renderTemplates() {
    const templatesList = document.querySelector('.templates-list');
    
    if (templates.length === 0) {
        templatesList.innerHTML = `
            <div class="empty-state">
                <p>Нет добавленных шаблонов</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    templates.forEach(template => {
        html += `
            <div class="list-item" data-id="${template.id}">
                <div class="list-item-content">
                    <div class="list-item-title">${template.name}</div>
                    <div class="list-item-subtitle">${template.content.substring(0, 50)}${template.content.length > 50 ? '...' : ''}</div>
                </div>
                <div class="list-item-actions">
                    <button class="secondary-btn edit-template-btn" data-id="${template.id}">Изменить</button>
                    <button class="secondary-btn delete-template-btn" data-id="${template.id}">Удалить</button>
                </div>
            </div>
        `;
    });
    
    templatesList.innerHTML = html;
    
    // Добавляем обработчики событий для кнопок
    document.querySelectorAll('.edit-template-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            editTemplate(id);
        });
    });
    
    document.querySelectorAll('.delete-template-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            deleteTemplate(id);
        });
    });
}

// Отображение медиафайлов
function renderMediaFiles() {
    const mediaList = document.querySelector('.media-list');
    
    if (mediaFiles.length === 0) {
        mediaList.innerHTML = `
            <div class="empty-state">
                <p>Нет добавленных медиафайлов</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    mediaFiles.forEach(media => {
        html += `
            <div class="list-item" data-id="${media.id}">
                <div class="list-item-content">
                    <div class="list-item-title">${media.name}</div>
                    <div class="list-item-subtitle">${media.type}</div>
                </div>
                <div class="list-item-actions">
                    <button class="secondary-btn view-media-btn" data-id="${media.id}">Просмотр</button>
                    <button class="secondary-btn delete-media-btn" data-id="${media.id}">Удалить</button>
                </div>
            </div>
        `;
    });
    
    mediaList.innerHTML = html;
    
    // Добавляем обработчики событий для кнопок
    document.querySelectorAll('.view-media-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            viewMedia(id);
        });
    });
    
    document.querySelectorAll('.delete-media-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            deleteMedia(id);
        });
    });
}

// Отображение заданий
function renderTasks() {
    const tasksList = document.querySelector('.tasks-list');
    
    if (tasks.length === 0) {
        tasksList.innerHTML = `
            <div class="empty-state">
                <p>Нет созданных заданий</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    tasks.forEach(task => {
        const progress = task.total > 0 ? Math.round((task.progress / task.total) * 100) : 0;
        
        html += `
            <div class="list-item" data-id="${task.id}">
                <div class="list-item-content">
                    <div class="list-item-title">${task.name}</div>
                    <div class="list-item-subtitle">
                        Статус: ${task.status}
                        <div class="progress-bar">
                            <div class="progress-bar-fill" style="width: ${progress}%"></div>
                        </div>
                    </div>
                </div>
                <div class="list-item-actions">
                    ${task.status === 'pending' ? `<button class="secondary-btn start-task-btn" data-id="${task.id}">Запустить</button>` : ''}
                    ${task.status === 'running' ? `<button class="secondary-btn pause-task-btn" data-id="${task.id}">Пауза</button>` : ''}
                    ${task.status === 'paused' ? `<button class="secondary-btn resume-task-btn" data-id="${task.id}">Продолжить</button>` : ''}
                    ${task.status !== 'completed' ? `<button class="secondary-btn stop-task-btn" data-id="${task.id}">Остановить</button>` : ''}
                    <button class="secondary-btn delete-task-btn" data-id="${task.id}">Удалить</button>
                </div>
            </div>
        `;
    });
    
    tasksList.innerHTML = html;
    
    // Добавляем обработчики событий для кнопок
    document.querySelectorAll('.start-task-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            startTask(id);
        });
    });
    
    document.querySelectorAll('.pause-task-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            pauseTask(id);
        });
    });
    
    document.querySelectorAll('.resume-task-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            resumeTask(id);
        });
    });
    
    document.querySelectorAll('.stop-task-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            stopTask(id);
        });
    });
    
    document.querySelectorAll('.delete-task-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-id');
            deleteTask(id);
        });
    });
}

// Отображение настроек
function renderSettings() {
    document.getElementById('delay-between-messages').value = settings.delayBetweenMessages;
    document.getElementById('max-messages-per-day').value = settings.maxMessagesPerDay;
}

// Показать модальное окно добавления аккаунта
function showAddAccountModal() {
    const modalContainer = document.getElementById('modal-container');
    const modal = document.getElementById('add-account-modal');
    
    // Очищаем поля формы
    document.getElementById('account-name').value = '';
    document.getElementById('account-phone').value = '';
    document.getElementById('proxy-host').value = '';
    document.getElementById('proxy-port').value = '';
    document.getElementById('proxy-username').value = '';
    document.getElementById('proxy-password').value = '';
    
    // Показываем модальное окно
    modalContainer.classList.add('active');
    modal.classList.add('active');
}

// Показать модальное окно добавления получателя
function showAddRecipientModal() {
    showNotification('Функция в разработке', 'Добавление получателей будет доступно в следующей версии.');
}

// Показать модальное окно добавления шаблона
function showAddTemplateModal() {
    showNotification('Функция в разработке', 'Добавление шаблонов будет доступно в следующей версии.');
}

// Показать модальное окно добавления медиафайла
function showAddMediaModal() {
    showNotification('Функция в разработке', 'Добавление медиафайлов будет доступно в следующей версии.');
}

// Показать модальное окно добавления задания
function showAddTaskModal() {
    showNotification('Функция в разработке', 'Создание заданий будет доступно в следующей версии.');
}

// Показать модальное окно импорта получателей
function showImportRecipientsModal() {
    showNotification('Функция в разработке', 'Импорт получателей будет доступен в следующей версии.');
}

// Закрыть все модальные окна
function closeAllModals() {
    const modalContainer = document.getElementById('modal-container');
    
    // Скрываем все модальные окна
    document.querySelectorAll('.modal').forEach(modal => {
        modal.classList.remove('active');
    });
    
    // Скрываем контейнер модальных окон
    modalContainer.classList.remove('active');
}

// Сохранить аккаунт
async function saveAccount() {
    try {
        const name = document.getElementById('account-name').value;
        const phone = document.getElementById('account-phone').value;
        const proxyHost = document.getElementById('proxy-host').value;
        const proxyPort = document.getElementById('proxy-port').value;
        const proxyUsername = document.getElementById('proxy-username').value;
        const proxyPassword = document.getElementById('proxy-password').value;
        
        // Проверяем обязательные поля
        if (!name || !phone) {
            showError('Ошибка валидации', 'Пожалуйста, заполните все обязательные поля.');
            return;
        }
        
        // Создаем объект аккаунта
        const account = {
            name,
            phone,
            proxy: proxyHost ? {
                host: proxyHost,
                port: proxyPort,
                username: proxyUsername,
                password: proxyPassword
            } : null
        };
        
        // Сохраняем аккаунт
        const result = await window.api.createAccount(account);
        
        // Закрываем модальное окно
        closeAllModals();
        
        // Обновляем список аккаунтов
        accounts.push(result);
        renderAccounts();
        
        // Показываем уведомление
        showNotification('Аккаунт добавлен', `Аккаунт "${name}" успешно добавлен.`);
        
        // Показываем QR-код для входа
        showQRCodeModal(result.id);
    } catch (error) {
        console.error('Error saving account:', error);
        showError('Ошибка сохранения', error.message || 'Не удалось сохранить аккаунт.');
    }
}

// Показать QR-код для входа
async function showQRCodeModal(accountId) {
    try {
        const modalContainer = document.getElementById('modal-container');
        const modal = document.getElementById('qr-code-modal');
        
        // Получаем QR-код
        const qrCodeData = await window.api.getLoginQrCode(accountId);
        
        // Устанавливаем QR-код
        document.getElementById('qr-code-img').src = qrCodeData;
        
        // Показываем модальное окно
        modalContainer.classList.add('active');
        modal.classList.add('active');
        
        // Сохраняем ID аккаунта
        modal.setAttribute('data-account-id', accountId);
    } catch (error) {
        console.error('Error showing QR code:', error);
        showError('Ошибка QR-кода', error.message || 'Не удалось получить QR-код для входа.');
    }
}

// Обновить QR-код
async function refreshQRCode() {
    try {
        const modal = document.getElementById('qr-code-modal');
        const accountId = modal.getAttribute('data-account-id');
        
        // Получаем QR-код
        const qrCodeData = await window.api.getLoginQrCode(accountId);
        
        // Устанавливаем QR-код
        document.getElementById('qr-code-img').src = qrCodeData;
        
        // Показываем уведомление
        showNotification('QR-код обновлен', 'QR-код для входа успешно обновлен.');
    } catch (error) {
        console.error('Error refreshing QR code:', error);
        showError('Ошибка обновления', error.message || 'Не удалось обновить QR-код для входа.');
    }
}

// Войти в аккаунт
async function loginAccount(accountId) {
    try {
        // Показываем QR-код для входа
        showQRCodeModal(accountId);
    } catch (error) {
        console.error('Error logging in account:', error);
        showError('Ошибка входа', error.message || 'Не удалось войти в аккаунт.');
    }
}

// Редактировать аккаунт
function editAccount(accountId) {
    showNotification('Функция в разработке', 'Редактирование аккаунтов будет доступно в следующей версии.');
}

// Удалить аккаунт
async function deleteAccount(accountId) {
    try {
        // Подтверждение удаления
        if (!confirm('Вы уверены, что хотите удалить этот аккаунт?')) {
            return;
        }
        
        // Удаляем аккаунт
        await window.api.deleteAccount(accountId);
        
        // Обновляем список аккаунтов
        accounts = accounts.filter(account => account.id !== accountId);
        renderAccounts();
        
        // Показываем уведомление
        showNotification('Аккаунт удален', 'Аккаунт успешно удален.');
    } catch (error) {
        console.error('Error deleting account:', error);
        showError('Ошибка удаления', error.message || 'Не удалось удалить аккаунт.');
    }
}

// Редактировать получателя
function editRecipient(recipientId) {
    showNotification('Функция в разработке', 'Редактирование получателей будет доступно в следующей версии.');
}

// Удалить получателя
function deleteRecipient(recipientId) {
    showNotification('Функция в разработке', 'Удаление получателей будет доступно в следующей версии.');
}

// Редактировать шаблон
function editTemplate(templateId) {
    showNotification('Функция в разработке', 'Редактирование шаблонов будет доступно в следующей версии.');
}

// Удалить шаблон
function deleteTemplate(templateId) {
    showNotification('Функция в разработке', 'Удаление шаблонов будет доступно в следующей версии.');
}

// Просмотреть медиафайл
function viewMedia(mediaId) {
    showNotification('Функция в разработке', 'Просмотр медиафайлов будет доступен в следующей версии.');
}

// Удалить медиафайл
function deleteMedia(mediaId) {
    showNotification('Функция в разработке', 'Удаление медиафайлов будет доступно в следующей версии.');
}

// Запустить задание
function startTask(taskId) {
    showNotification('Функция в разработке', 'Запуск заданий будет доступен в следующей версии.');
}

// Приостановить задание
function pauseTask(taskId) {
    showNotification('Функция в разработке', 'Приостановка заданий будет доступна в следующей версии.');
}

// Возобновить задание
function resumeTask(taskId) {
    showNotification('Функция в разработке', 'Возобновление заданий будет доступно в следующей версии.');
}

// Остановить задание
function stopTask(taskId) {
    showNotification('Функция в разработке', 'Остановка заданий будет доступна в следующей версии.');
}

// Удалить задание
function deleteTask(taskId) {
    showNotification('Функция в разработке', 'Удаление заданий будет доступно в следующей версии.');
}

// Сохранить настройки
async function saveSettings() {
    try {
        const delayBetweenMessages = parseInt(document.getElementById('delay-between-messages').value);
        const maxMessagesPerDay = parseInt(document.getElementById('max-messages-per-day').value);
        
        // Проверяем значения
        if (isNaN(delayBetweenMessages) || isNaN(maxMessagesPerDay) || delayBetweenMessages < 1 || maxMessagesPerDay < 1) {
            showError('Ошибка валидации', 'Пожалуйста, введите корректные значения.');
            return;
        }
        
        // Создаем объект настроек
        const newSettings = {
            delayBetweenMessages,
            maxMessagesPerDay
        };
        
        // Сохраняем настройки
        await window.api.saveSettings(newSettings);
        
        // Обновляем настройки
        settings = newSettings;
        
        // Показываем уведомление
        showNotification('Настройки сохранены', 'Настройки успешно сохранены.');
    } catch (error) {
        console.error('Error saving settings:', error);
        showError('Ошибка сохранения', error.message || 'Не удалось сохранить настройки.');
    }
}

// Обработчик уведомлений
function handleNotification(data) {
    if (data.type === 'error') {
        showError(data.title, data.message);
    } else {
        showNotification(data.title, data.message);
    }
}

// Показать уведомление
function showNotification(title, message) {
    const notificationContainer = document.getElementById('notification-container');
    
    // Создаем элемент уведомления
    const notification = document.createElement('div');
    notification.className = 'notification';
    notification.innerHTML = `
        <div class="notification-header">
            <h4>${title}</h4>
            <button class="close-notification-btn">&times;</button>
        </div>
        <div class="notification-body">
            <p>${message}</p>
        </div>
    `;
    
    // Добавляем уведомление в контейнер
    notificationContainer.appendChild(notification);
    
    // Добавляем обработчик закрытия
    notification.querySelector('.close-notification-btn').addEventListener('click', () => {
        notification.remove();
    });
    
    // Автоматически скрываем уведомление через 5 секунд
    setTimeout(() => {
        notification.remove();
    }, 5000);
}

// Показать ошибку
function showError(title, message) {
    const notificationContainer = document.getElementById('notification-container');
    
    // Создаем элемент уведомления
    const notification = document.createElement('div');
    notification.className = 'notification error';
    notification.innerHTML = `
        <div class="notification-header">
            <h4>${title}</h4>
            <button class="close-notification-btn">&times;</button>
        </div>
        <div class="notification-body">
            <p>${message}</p>
        </div>
    `;
    
    // Добавляем уведомление в контейнер
    notificationContainer.appendChild(notification);
    
    // Добавляем обработчик закрытия
    notification.querySelector('.close-notification-btn').addEventListener('click', () => {
        notification.remove();
    });
    
    // Автоматически скрываем уведомление через 10 секунд
    setTimeout(() => {
        notification.remove();
    }, 10000);
}
