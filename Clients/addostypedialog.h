#ifndef ADDOSTYPEDIALOG_H
#define ADDOSTYPEDIALOG_H

#include <QDialog>

namespace Ui {
class AddOSTypeDialog;
}

class AddOSTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddOSTypeDialog(QWidget *parent = nullptr);
    ~AddOSTypeDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddOSTypeDialog *ui;
};

#endif // ADDOSTYPEDIALOG_H
