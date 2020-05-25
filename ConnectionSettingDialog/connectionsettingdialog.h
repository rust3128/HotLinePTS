#ifndef CONNECTIONSETTINGDIALOG_H
#define CONNECTIONSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionSettingDialog;
}

class ConnectionSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionSettingDialog(QWidget *parent = nullptr);
    ~ConnectionSettingDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConnectionSettingDialog *ui;
private:
    void readConnData();    //Чтение настроек подключения
    void saveConnData();    //Запись настроек подключения
};

#endif // CONNECTIONSETTINGDIALOG_H
