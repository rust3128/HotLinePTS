#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"

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

}
