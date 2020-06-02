#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(QSqlRecord *rec, QString clnName, QWidget *parent = nullptr);
    ~EditObjectDialog();
private slots:
    void on_plainTextEditComments_textChanged();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void createUI();
private:
    Ui::EditObjectDialog *ui;
    QSqlRecord *curRecord;

};

#endif // EDITOBJECTDIALOG_H
