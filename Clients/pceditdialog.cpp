#include "pceditdialog.h"
#include "ui_pceditdialog.h"
#include <QSqlQuery>
#include <QSqlError>

PCEditDialog::PCEditDialog(uint pcID, uint objID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCEditDialog),
    pcID(pcID),
    objectID(objID)
{
    ui->setupUi(this);
    createUI();
}

PCEditDialog::~PCEditDialog()
{
    delete ui;
}

void PCEditDialog::changeEvent(QEvent *e)
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

void PCEditDialog::createUI()
{
    if(pcID>0){
        this->setWindowTitle("Редактирование рабочего места");
    } else {
        this->setWindowTitle("Добавление рабочего места");
    }

    QSqlQuery q;
    q.prepare("select clients.name, objects.terminal_id, objects.name from clients "
              "left outer join objects on (clients.client_id = objects.client_id) "
              "where (objects.object_id = :objectID)");
    q.bindValue(":objectID", objectID);
    q.exec();
    q.next();
    ui->labelTitle->setText("<html><head/><body><p align='center'><span style=' font-size:14pt; font-weight:600; color:#4e9a06;'>"+q.value(0).toString()+"<br>АЗС "+q.value(1).toString()+"<br>"+q.value(2).toString()+"</span></p></body></html>");
//    ui->labelTitle->setText(q.value(0).toString()+"<br>АЗС "+q.value(1).toString()+"<br>"+q.value(2).toString());

}

void PCEditDialog::createModel()
{

}
