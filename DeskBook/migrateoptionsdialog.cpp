#include "migrateoptionsdialog.h"
#include "ui_migrateoptionsdialog.h"
#include "LoggingCategories/loggingcategories.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

MigrateOptionsDialog::MigrateOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MigrateOptionsDialog)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

MigrateOptionsDialog::~MigrateOptionsDialog()
{
    delete ui;
}

void MigrateOptionsDialog::createUI()
{
    QValidator *validator = new QIntValidator(100, 9999, this);
    ui->lineEditID->setValidator(validator);
    ui->groupBoxEdit->hide();
    curMigOpt.type.clear();

    ui->tableView->setModel(modelMO);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void MigrateOptionsDialog::createModel()
{
    modelMO = new QSqlTableModel(this);
    modelMO->setTable("MIGRATEOPTIONS");
    modelMO->setSort(0,Qt::AscendingOrder);

    modelMO->setHeaderData(0,Qt::Horizontal,"ID");
    modelMO->setHeaderData(1,Qt::Horizontal,"Значение(Пример)");
    modelMO->setHeaderData(2,Qt::Horizontal,"Тип");
    modelMO->setHeaderData(3,Qt::Horizontal,"Назначение опции");


    modelMO->select();
    while(modelMO->canFetchMore())
        modelMO->fetchMore();
    proxyModel = new QSortFilterProxyModel();
    proxyModel->setSourceModel(modelMO);
}

void MigrateOptionsDialog::on_lineEdit_textChanged(const QString &strFind)
{
    int columFind =-1;
    ui->tableView->setModel(proxyModel);
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(strFind.trimmed(),Qt::CaseInsensitive,syntax);
    if(ui->radioButtonID->isChecked()) columFind = 0;
    if(ui->radioButtonValue->isChecked()) columFind =1;
    if(ui->radioButtonComm->isChecked()) columFind = 3;
    proxyModel->setFilterKeyColumn(columFind);
    proxyModel->setFilterRegExp(regExp);
}

void MigrateOptionsDialog::on_pushButtonAdd_clicked()
{
    ui->groupBoxEdit->show();
    ui->groupBoxEdit->setTitle("Добавление опции");
    ui->frameInfo->setEnabled(false);

}

void MigrateOptionsDialog::on_tableView_doubleClicked(const QModelIndex &idx)
{

    QModelIndex sourceIdx = idx;
    if(!ui->lineEdit->text().isEmpty()){
        sourceIdx = proxyModel->mapToSource(idx);
    }

    ui->groupBoxEdit->show();
    ui->groupBoxEdit->setTitle("Редактирование опции");
    ui->frameInfo->setEnabled(false);

    ui->lineEditID->setText(modelMO->data(modelMO->index(sourceIdx.row(),0)).toString().trimmed());
    curMigOpt.ID=modelMO->data(modelMO->index(sourceIdx.row(),0)).toInt();
    curMigOpt.type = modelMO->data(modelMO->index(sourceIdx.row(),2)).toString().trimmed();
    if(curMigOpt.type == "I") {
        ui->radioButtonInt->setChecked(true);
    } else if(curMigOpt.type == "F") {
        ui->radioButtonFloat->setChecked(true);
    } else if(curMigOpt.type == "B") {
        ui->radioButtonBool->setChecked(true);
    } else if(curMigOpt.type == "S") {
        ui->radioButtonString->setChecked(true);
    } else if(curMigOpt.type == "D") {
        ui->radioButtonDate->setChecked(true);
    }
    ui->lineEditValue->setText(modelMO->data(modelMO->index(sourceIdx.row(),1)).toString().trimmed());
    ui->lineEditComment->setText(modelMO->data(modelMO->index(sourceIdx.row(),3)).toString().trimmed());
}

void MigrateOptionsDialog::on_buttonBoxEdit_accepted()
{
   if(validateRecord()){
        QSqlQuery *q = new QSqlQuery();
        q->prepare("UPDATE OR INSERT INTO MIGRATEOPTIONS (MIGRATEOPTION_ID, SVALUE, VTYPE, REMARK) "
                   "VALUES (:optionID, :value, :type, :remark) "
                   "MATCHING (MIGRATEOPTION_ID)");
        q->bindValue(":optionID", ui->lineEditID->text().trimmed());
        q->bindValue(":value", ui->lineEditValue->text().trimmed());
        q->bindValue(":type",curMigOpt.type);
        q->bindValue(":remark", ui->lineEditComment->text().trimmed());
        if(!q->exec()){
           qCritical(logCritical()) << "Не удалось обновить опцию " + ui->lineEditID->text().trimmed()+"!\nПричина: "+q->lastError().text();
           QMessageBox::critical(this,"Ошибка","Не удалось обновить опцию " + ui->lineEditID->text().trimmed()+"!\nПричина: "+q->lastError().text());
           return;
        }
        ui->groupBoxEdit->hide();
        ui->frameInfo->setEnabled(true);
        modelMO->select();
        while(modelMO->canFetchMore())
            modelMO->fetchMore();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
        ui->lineEditID->clear();
        ui->lineEditValue->clear();
        ui->lineEditComment->clear();
   }
}

bool MigrateOptionsDialog::validateRecord()
{

//    const int rowCount = modelMO->rowCount();
//    curMigOpt.ID = ui->lineEditID->text().trimmed().toInt();
//    for(int i = 0; i<rowCount; ++i){
//        if(modelMO->data(modelMO->index(i,0)).toInt() == curMigOpt.ID){
//            QMessageBox::warning(this,"Ошибка",QString("Добавить запись не возможно.\nОпиция с кодом "+ui->lineEditID->text().trimmed()+" уже есть в справочнике."));
//                return false;
//        }
//    }
    if(curMigOpt.type.isEmpty()){
        QMessageBox::warning(this,"Ошибка",QString("Не выбран тип опции"));
        return false;
    }
    return true;
}



void MigrateOptionsDialog::on_radioButtonInt_clicked(bool checked)
{
    if(checked) curMigOpt.type = "I";
}

void MigrateOptionsDialog::on_radioButtonFloat_clicked(bool checked)
{
    if(checked) curMigOpt.type = "F";
}

void MigrateOptionsDialog::on_radioButtonBool_clicked(bool checked)
{
    if(checked) curMigOpt.type = "B";
}

void MigrateOptionsDialog::on_radioButtonString_clicked(bool checked)
{
    if(checked) curMigOpt.type = "S";
}

void MigrateOptionsDialog::on_radioButtonDate_clicked(bool checked)
{
    if(checked) curMigOpt.type = "D";
}


void MigrateOptionsDialog::on_pushButtonClose_clicked()
{
    this->reject();
}

void MigrateOptionsDialog::on_buttonBoxEdit_rejected()
{
    ui->groupBoxEdit->hide();
    ui->frameInfo->setEnabled(true);
}
