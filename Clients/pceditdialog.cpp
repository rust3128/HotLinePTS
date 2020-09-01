#include "pceditdialog.h"
#include "ui_pceditdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "Clients/addostypedialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDir>

PCEditDialog::PCEditDialog(int pcID, uint objID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCEditDialog),
    pcID(pcID),
    objectID(objID)
{
    ui->setupUi(this);
    createModel();
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
    QSqlQuery q;
    q.prepare("select clients.name, objects.terminal_id, objects.name from clients "
              "left outer join objects on (clients.client_id = objects.client_id) "
              "where (objects.object_id = :objectID)");
    q.bindValue(":objectID", objectID);
    q.exec();
    q.next();
    ui->labelTitle->setText("<html><head/><body><p align='center'><span style=' font-size:14pt; font-weight:600; color:#4e9a06;'>"+q.value(0).toString()+"<br>АЗС "+q.value(1).toString()+"<br>"+q.value(2).toString()+"</span></p></body></html>");
//    ui->labelTitle->setText(q.value(0).toString()+"<br>АЗС "+q.value(1).toString()+"<br>"+q.value(2).toString());
    q.finish();
    if(pcID<0){
        this->setWindowTitle("Добавление рабочего места");
    } else {
        this->setWindowTitle("Редактирование рабочего места");



        q.prepare("select p.pctype_id, p.exetype_id, p.pos_id, p.ipadress, p.vncpass, p.pcmodel_id, p.pcos_id from pclist p "
                  "where p.pc_id=:pcID");
        q.bindValue(":pcID", pcID);
        if(!q.exec()) qCritical(logCritical()) << "Не получены данные о рабочем месте" << q.lastError().text();
        q.next();

        ui->comboBoxPCType->setModel(modelPCType);
        ui->comboBoxPCType->setModelColumn(1);
        int rowcount = modelPCType->rowCount();
        int curIdx=-1;
        for(int i =0;i<rowcount;++i){
            if(modelPCType->data(modelPCType->index(i,0)).toInt() == q.value(0).toInt() ){
                curIdx = i;
                break;
            }
        }
        ui->comboBoxPCType->setCurrentIndex(curIdx);

        rowcount = modelEXEFile->rowCount();
        curIdx=-1;
        for(int i =0;i<rowcount;++i){
            if(modelEXEFile->data(modelEXEFile->index(i,0)).toInt() == q.value(1).toInt() ){
                curIdx = i;
                break;
            }
        }
        ui->comboBoxEXEType->setCurrentIndex(curIdx);
        if(curIdx<0)
            ui->comboBoxEXEType->setCurrentText("Не указано");

        ui->spinBoxPosID->setValue(q.value(2).toInt());
        ui->lineEditIP->setText(q.value(3).toString());
        ui->lineEditPass->setText(q.value(4).toString());

        ui->comboBoxModelPC->setModel(modelPCModel);
        ui->comboBoxModelPC->setModelColumn(1);
        rowcount = modelPCModel->rowCount();
        curIdx = -1;

        for(int i =0;i<rowcount;++i){
            if(modelPCModel->data(modelPCModel->index(i,0)).toInt() == q.value(5).toInt() ){
                curIdx = i;
                break;
            }
        }
        ui->comboBoxModelPC->setCurrentIndex(curIdx);
        if(curIdx<0)
            ui->comboBoxModelPC->setCurrentText("Не указано");

        ui->comboBoxOSType->setModel(modelOSType);
        ui->comboBoxOSType->setModelColumn(1);
        rowcount=modelOSType->rowCount();
        curIdx = -1;

        for(int i =0;i<rowcount;++i){
            if(modelOSType->data(modelOSType->index(i,0)).toInt() == q.value(6).toInt() ){
                curIdx = i;
                break;
            }
        }
        ui->comboBoxOSType->setCurrentIndex(curIdx);
        if(curIdx<0)
            ui->comboBoxOSType->setCurrentText("Не указано");
    }
}

void PCEditDialog::createModel()
{
    modelPCType = new QSqlTableModel(this);
    modelPCType->setTable("PCTYPE");
    modelPCType->select();

    modelPCModel = new QSqlTableModel(this);
    modelPCModel->setTable("PCMODEL");
    modelPCModel->select();

    modelOSType = new QSqlTableModel(this);
    modelOSType->setTable("PCOS");
    modelOSType->select();

    modelEXEFile = new QSqlTableModel(this);
    modelEXEFile->setTable("EXETYPE");
    modelEXEFile->select();

}

void PCEditDialog::on_toolButtonAddModelPC_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString::fromUtf8("Добавить модель ПК"),
                                 QString::fromUtf8("Укажите новую модель ПК:"),
                                 QLineEdit::Normal,
                                 "", &ok);
    if (ok && !text.isEmpty()){
        QSqlQuery q;
        q.prepare("INSERT INTO PCMODEL (PCMODEL) VALUES(:pcmodel)");
        q.bindValue(":pcmodel",text.trimmed());
        q.exec();
    }
    modelPCModel->select();
}

void PCEditDialog::on_toolButtonAddOSType_clicked()
{
    AddOSTypeDialog *osType = new AddOSTypeDialog(this);
    if(osType->exec() == QDialog::Accepted){

    }

}

void PCEditDialog::on_buttonBox_rejected()
{
    this->reject();
}

void PCEditDialog::on_buttonBox_accepted()
{

}
