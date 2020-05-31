#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"

EditObjectDialog::EditObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog)
{
    ui->setupUi(this);
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}
