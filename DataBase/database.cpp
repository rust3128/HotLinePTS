#include "database.h"
#include "GlobalSettings/globalsettings.h"
#include "LoggingCategories/loggingcategories.h"
#include "ConnectionSettingDialog/connectionsettingdialog.h"


#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

bool DataBase::connectDB()
{
    bool result = false;

    QSettings settings(GlobalSettings::CONFIG_FILE_NAME, QSettings::IniFormat);
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");

    //Установка параметров подключения из файла настроек
    settings.beginGroup("DATABASE");
    db.setHostName(settings.value("HostName").toString());
    db.setDatabaseName(settings.value("DataBase").toString());
    db.setUserName(settings.value("User").toString());
    db.setPassword(settings.value("Password").toString());
    settings.endGroup();
    //Открываем базу данных, если попытка не удачная вызываем диалог настроек подключения
    if(!db.open()) {
        qCritical(logCritical()) <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << db.lastError().text();
        int rez = QMessageBox::question(nullptr, QObject::tr("Ошибка подключения"),
                              "Не могу установить соединение с базой данных.\nПроверить настройки подключения?",
                              QMessageBox::Yes | QMessageBox::No);
        if(rez == QMessageBox::Yes) {
            ConnectionSettingDialog *connDlg = new ConnectionSettingDialog();
            int result = connDlg->exec();
            if(result == QDialog::Accepted) {
                qInfo(logInfo()) << "Сохранение настроек подключения.";
            }
        }
        //Завершаем работу
        result = false;
    } else {
        qInfo(logInfo()) << "Успешное подключение к центральной базе данных.";
        result = true;
    }


    return result;
}
