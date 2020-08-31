#include "addostypedialog.h"
#include "ui_addostypedialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QSqlQuery>
#include <QSqlError>

AddOSTypeDialog::AddOSTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOSTypeDialog)
{
    ui->setupUi(this);
}

AddOSTypeDialog::~AddOSTypeDialog()
{
    delete ui;
}

void AddOSTypeDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AddOSTypeDialog::on_buttonBox_accepted()
{
    QString osName;
    osName.clear();

    if(ui->radioButtonWinXP->isChecked()) osName = ui->radioButtonWinXP->text()+" ";
    if(ui->radioButtonWin7->isChecked()) osName = ui->radioButtonWin7->text()+" ";
    if(ui->radioButtonWin10->isChecked()) osName = ui->radioButtonWin10->text()+" ";

    if(ui->checkBox->isChecked()) osName += ui->checkBox->text()+" ";

    if(ui->radioButton32->isChecked()) osName += ui->radioButton32->text();
    if(ui->radioButton64->isChecked()) osName += ui->radioButton64->text();

    qInfo(logInfo()) << osName;
    QSqlQuery q;
    q.prepare("UPDATE OR INSERT INTO PCOS (PCOS) VALUES (:osname) MATCHING (PCOS)");
    q.bindValue(":osname", osName);
    q.exec();
    this->accept();

}
