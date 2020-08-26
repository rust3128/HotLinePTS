#ifndef PCEDITDIALOG_H
#define PCEDITDIALOG_H

#include <QDialog>

namespace Ui {
class PCEditDialog;
}

class PCEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCEditDialog(uint pcID, uint objID, QWidget *parent = nullptr);
    ~PCEditDialog();

protected:
    void changeEvent(QEvent *e);


private:
    void createUI();
    void createModel();
private:
    Ui::PCEditDialog *ui;
    uint pcID;
    uint objectID;
};

#endif // PCEDITDIALOG_H
