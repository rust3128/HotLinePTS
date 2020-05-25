#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "GlobalSettings/globalsettings.h"
#include <LoggingCategories/loggingcategories.h>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    createUI();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::changeEvent(QEvent *e)
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

void OptionsDialog::createUI()
{
    optChanged=false;
    QSqlQuery q("select o.option_id, o.optionvalue, o.comment from options o order by o.option_id");
    while(q.next()){
        opt.insert(q.value(0).toUInt(), q.value(1));
        switch (q.value(0).toUInt()) {
        case GlobalSettings::OPT_MIMIMAZE_TO_TRAY:
            ui->checkBoxTray->setChecked(q.value(1).toBool());
            ui->checkBoxTray->setText(q.value(2).toString().trimmed());
            break;
        default:
            break;
        }
    }
}

void OptionsDialog::on_buttonBox_rejected()
{
    this->reject();
}

void OptionsDialog::on_checkBoxTray_clicked()
{
    optChanged=true;
    opt[GlobalSettings::OPT_MIMIMAZE_TO_TRAY]=ui->checkBoxTray->isChecked();
}

void OptionsDialog::on_buttonBox_accepted()
{
    if(optChanged){
        QSqlQuery q;
        QString comment;
        QHashIterator<uint,QVariant> i(opt);
        while(i.hasNext()){
            i.next();
            q.prepare("UPDATE OR INSERT INTO OPTIONS (OPTION_ID, OPTIONVALUE, COMMENT) "
                      "VALUES (:optionID, :optionValue, :optComment) MATCHING (OPTION_ID)");
            q.bindValue(":optionID", i.key());
            q.bindValue(":optionValue",i.value().toString());
            switch (i.key()) {
            case GlobalSettings::OPT_MIMIMAZE_TO_TRAY:
                comment = ui->checkBoxTray->text().trimmed();
                break;
            default:
                break;
            }
            q.bindValue(":optComment",comment);
            if(!q.exec()) {
                qCritical(logCritical()) << tr("Не удалось изменить значение опции ")+QString::number(i.key()) << q.lastError().text();
            }
        }
    }
    this->accept();
}
