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
private:
    void createUI();
private:
    Ui::EditObjectDialog *ui;
    QSqlRecord *curRecord;
};

#endif // EDITOBJECTDIALOG_H
