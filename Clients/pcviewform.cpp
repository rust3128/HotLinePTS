#include "pcviewform.h"
#include "ui_pcviewform.h"
#include "LoggingCategories/loggingcategories.h"

PCViewForm::PCViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCViewForm)
{
    ui->setupUi(this);
}

PCViewForm::~PCViewForm()
{
    delete ui;
}

void PCViewForm::slotGetObjectID(int ID)
{
    objectID = ID;
    qInfo(logInfo()) << "ID in FORM" << objectID;
}

void PCViewForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
