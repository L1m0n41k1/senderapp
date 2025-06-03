const { contextBridge, ipcRenderer } = require('electron');

// Экспортируем API для использования в рендерере
contextBridge.exposeInMainWorld('api', {
  // Статистика
  getAccountsCount: () => ipcRenderer.invoke('get-accounts-count'),
  getRecipientsCount: () => ipcRenderer.invoke('get-recipients-count'),
  getTemplatesCount: () => ipcRenderer.invoke('get-templates-count'),
  getActiveJobsCount: () => ipcRenderer.invoke('get-active-jobs-count'),
  getRecentActivity: () => ipcRenderer.invoke('get-recent-activity'),
  
  // WhatsApp аккаунты
  getAccounts: () => ipcRenderer.invoke('get-whatsapp-accounts'),
  createAccount: (data) => ipcRenderer.invoke('create-whatsapp-account', data),
  updateAccount: (id, data) => ipcRenderer.invoke('update-whatsapp-account', id, data),
  deleteAccount: (id) => ipcRenderer.invoke('delete-whatsapp-account', id),
  getLoginQrCode: (id) => ipcRenderer.invoke('get-login-qr-code', id),
  checkLoginStatus: (id) => ipcRenderer.invoke('check-login-status', id),
  logoutAccount: (id) => ipcRenderer.invoke('logout-whatsapp-account', id),
  onAccountStatusChange: (callback) => {
    ipcRenderer.on('whatsapp-account-status-change', (event, data) => callback(data));
    return () => ipcRenderer.removeAllListeners('whatsapp-account-status-change');
  },
  
  // Получатели
  getRecipients: (filters) => ipcRenderer.invoke('get-recipients', filters),
  createRecipient: (data) => ipcRenderer.invoke('create-recipient', data),
  updateRecipient: (id, data) => ipcRenderer.invoke('update-recipient', id, data),
  deleteRecipient: (id) => ipcRenderer.invoke('delete-recipient', id),
  importRecipientsFromCsv: (filePath, options) => ipcRenderer.invoke('import-recipients-from-csv', filePath, options),
  
  // Шаблоны
  getTemplates: (filters) => ipcRenderer.invoke('get-templates', filters),
  createTemplate: (data) => ipcRenderer.invoke('create-template', data),
  updateTemplate: (id, data) => ipcRenderer.invoke('update-template', id, data),
  deleteTemplate: (id) => ipcRenderer.invoke('delete-template', id),
  processTemplate: (templateContent, variables) => ipcRenderer.invoke('process-template', templateContent, variables),
  
  // Медиафайлы
  getMediaFiles: () => ipcRenderer.invoke('get-media-files'),
  uploadMediaFile: (filePath, type, name, category) => ipcRenderer.invoke('upload-media-file', filePath, type, name, category),
  deleteMediaFile: (id) => ipcRenderer.invoke('delete-media-file', id),
  
  // Задания
  getJobs: () => ipcRenderer.invoke('get-jobs'),
  createJob: (data) => ipcRenderer.invoke('create-job', data),
  startJob: (id) => ipcRenderer.invoke('start-job', id),
  pauseJob: (id) => ipcRenderer.invoke('pause-job', id),
  stopJob: (id) => ipcRenderer.invoke('stop-job', id),
  deleteJob: (id) => ipcRenderer.invoke('delete-job', id),
  getMessages: (jobId, filters) => ipcRenderer.invoke('get-messages', jobId, filters),
  onJobStatusChange: (callback) => {
    ipcRenderer.on('job-status-change', (event, data) => callback(data));
    return () => ipcRenderer.removeAllListeners('job-status-change');
  },
  onMessageStatusChange: (callback) => {
    ipcRenderer.on('message-status-change', (event, data) => callback(data));
    return () => ipcRenderer.removeAllListeners('message-status-change');
  },
  
  // Уведомления
  onNotification: (callback) => {
    ipcRenderer.on('notification', (event, data) => callback(data));
    return () => ipcRenderer.removeAllListeners('notification');
  },
  
  // Диалоги
  selectFile: (options) => ipcRenderer.invoke('select-file', options),
  saveFile: (options) => ipcRenderer.invoke('save-file', options),
  showMessage: (options) => ipcRenderer.invoke('show-message', options),
  
  // Системные функции
  openExternalLink: (url) => ipcRenderer.invoke('open-external-link', url),
  quitApp: () => ipcRenderer.invoke('quit-app')
});
