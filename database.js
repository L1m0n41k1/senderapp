const { Sequelize, DataTypes } = require('sequelize');
const path = require('path');
const fs = require('fs');

// Инициализация базы данных
async function initializeDatabase() {
  try {
    // Создаем директорию для базы данных, если она не существует
    const dbDir = path.join(process.env.APPDATA || process.env.HOME, 'WhatsAppSenderDesktop', 'data');
    if (!fs.existsSync(dbDir)) {
      fs.mkdirSync(dbDir, { recursive: true });
    }

    // Инициализация SQLite базы данных
    const dbPath = path.join(dbDir, 'whatsapp-sender.db');
    const sequelize = new Sequelize({
      dialect: 'sqlite',
      storage: dbPath,
      logging: false
    });

    // Проверка соединения
    await sequelize.authenticate();
    console.log('Database connection has been established successfully.');

    // Определение моделей
    const Account = sequelize.define('Account', {
      name: {
        type: DataTypes.STRING,
        allowNull: false
      },
      phone: {
        type: DataTypes.STRING,
        allowNull: true
      },
      isLoggedIn: {
        type: DataTypes.BOOLEAN,
        defaultValue: false
      },
      proxy: {
        type: DataTypes.JSON,
        allowNull: true
      },
      sessionData: {
        type: DataTypes.TEXT,
        allowNull: true
      }
    });

    const Recipient = sequelize.define('Recipient', {
      name: {
        type: DataTypes.STRING,
        allowNull: false
      },
      phone: {
        type: DataTypes.STRING,
        allowNull: false
      },
      group: {
        type: DataTypes.STRING,
        allowNull: true
      },
      variables: {
        type: DataTypes.JSON,
        allowNull: true
      }
    });

    const Template = sequelize.define('Template', {
      name: {
        type: DataTypes.STRING,
        allowNull: false
      },
      content: {
        type: DataTypes.TEXT,
        allowNull: false
      },
      category: {
        type: DataTypes.STRING,
        allowNull: true
      }
    });

    const MediaFile = sequelize.define('MediaFile', {
      name: {
        type: DataTypes.STRING,
        allowNull: false
      },
      type: {
        type: DataTypes.STRING,
        allowNull: false
      },
      path: {
        type: DataTypes.STRING,
        allowNull: false
      },
      category: {
        type: DataTypes.STRING,
        allowNull: true
      }
    });

    const Job = sequelize.define('Job', {
      name: {
        type: DataTypes.STRING,
        allowNull: false
      },
      status: {
        type: DataTypes.ENUM('pending', 'running', 'paused', 'completed', 'failed'),
        defaultValue: 'pending'
      },
      accountId: {
        type: DataTypes.INTEGER,
        allowNull: false
      },
      templateId: {
        type: DataTypes.INTEGER,
        allowNull: false
      },
      mediaFileId: {
        type: DataTypes.INTEGER,
        allowNull: true
      },
      schedule: {
        type: DataTypes.JSON,
        allowNull: true
      },
      progress: {
        type: DataTypes.INTEGER,
        defaultValue: 0
      },
      total: {
        type: DataTypes.INTEGER,
        defaultValue: 0
      }
    });

    const Message = sequelize.define('Message', {
      jobId: {
        type: DataTypes.INTEGER,
        allowNull: false
      },
      recipientId: {
        type: DataTypes.INTEGER,
        allowNull: false
      },
      status: {
        type: DataTypes.ENUM('pending', 'sent', 'delivered', 'read', 'failed'),
        defaultValue: 'pending'
      },
      content: {
        type: DataTypes.TEXT,
        allowNull: false
      },
      error: {
        type: DataTypes.TEXT,
        allowNull: true
      }
    });

    // Определение связей
    Job.hasMany(Message);
    Message.belongsTo(Job);

    // Синхронизация моделей с базой данных
    await sequelize.sync();
    console.log('All models were synchronized successfully.');

    // Возвращаем объект с моделями и экземпляром Sequelize
    return {
      sequelize,
      models: {
        Account,
        Recipient,
        Template,
        MediaFile,
        Job,
        Message
      }
    };
  } catch (error) {
    console.error('Unable to initialize database:', error);
    throw error;
  }
}

module.exports = {
  initializeDatabase
};
