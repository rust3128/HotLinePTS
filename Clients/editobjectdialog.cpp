#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QSqlQuery>
#include <QSqlError>


EditObjectDialog::EditObjectDialog(QSqlRecord *rec, QString clnName,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog),
    curRecord(rec)
{
    ui->setupUi(this);
    ui->labelClientName->setText(clnName);
    createUI();
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

void EditObjectDialog::createUI()
{
    ui->lineEditTerminalID->setText(curRecord->value(1).toString());
    ui->lineEditAdress->setText(curRecord->value(2).toString());
    ui->lineEditPhoneAZS->setText(curRecord->value(3).toString());
    ui->lineEditPhoneDir->setText(curRecord->value(4).toString());
    ui->lineEditEmail->setText(curRecord->value(5).toString());
    ui->plainTextEditComments->setPlainText(curRecord->value(6).toString());
}


void EditObjectDialog::on_plainTextEditComments_textChanged()
{
    if(ui->plainTextEditComments->toPlainText().length() > 200){
        ui->plainTextEditComments->setPlainText(ui->plainTextEditComments->toPlainText().left(200));
    }
}

void EditObjectDialog::on_buttonBox_rejected()
{
    this->reject();
}

void EditObjectDialog::on_buttonBox_accepted()
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("UPDATE OBJECTS SET NAME = :name, PHONE = :phone, PHONEDIR = :phoneDir, EMAIL = :email, COMMENTS = :comment "
               "WHERE (OBJECT_ID = :objectID)");
    q->bindValue(":name", ui->lineEditAdress->text().trimmed());
    q->bindValue(":phone",ui->lineEditPhoneAZS->text().trimmed());
    q->bindValue(":phoneDir", ui->lineEditPhoneDir->text().trimmed());
    q->bindValue(":email", ui->lineEditEmail->text().trimmed());
    q->bindValue(":comment", ui->plainTextEditComments->toPlainText().trimmed());
    q->bindValue(":objectID", curRecord->value(0).toInt());
    if(!q->exec()){
        qCritical(logCritical()) << "Не удалось обновить информацию об АЗС" << endl << q->lastQuery() << q->lastError().text();
        return;
    }
    qInfo(logInfo()) << "Информация об АЗС " << curRecord->value(1).toString() << "успешно обновлена.";
    this->accept();
}
