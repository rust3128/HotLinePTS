#include "connectionsettingdialog.h"
#include "ui_connectionsettingdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "GlobalSettings/globalsettings.h"
#include <QSettings>

ConnectionSettingDialog::ConnectionSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionSettingDialog)
{
    ui->setupUi(this);
    readConnData();
    qInfo(logInfo()) << "Открытие диалога настройки подключения.";
}

ConnectionSettingDialog::~ConnectionSettingDialog()
{
    delete ui;
}

void ConnectionSettingDialog::readConnData()
{
    QSettings settings(GlobalSettings::CONFIG_FILE_NAME,QSettings::IniFormat);
    settings.beginGroup("DATABASE");
    ui->lineEditServer->setText(settings.value("HostName").toString());
    ui->lineEditDatabase->setText(settings.value("DataBase").toString());
    ui->lineEditUser->setText(settings.value("User").toString());
    ui->lineEditPassword->setText(settings.value("Password").toString());
    settings.endGroup();
}

void ConnectionSettingDialog::saveConnData()
{
    QSettings settings(GlobalSettings::CONFIG_FILE_NAME,QSettings::IniFormat);
    settings.beginGroup("DATABASE");
    settings.setValue("HostName",ui->lineEditServer->text().trimmed());
    settings.setValue("DataBase",ui->lineEditDatabase->text().trimmed());
    settings.setValue("User",ui->lineEditUser->text().trimmed());
    settings.setValue("Password",ui->lineEditPassword->text().trimmed());
    settings.endGroup();
}

void ConnectionSettingDialog::on_buttonBox_accepted()
{
    saveConnData();
    this->accept();
}

void ConnectionSettingDialog::on_buttonBox_rejected()
{
    this->reject();
}
