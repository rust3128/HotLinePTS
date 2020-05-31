#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include <QDialog>

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(QWidget *parent = nullptr);
    ~EditObjectDialog();

private:
    Ui::EditObjectDialog *ui;
};

#endif // EDITOBJECTDIALOG_H
