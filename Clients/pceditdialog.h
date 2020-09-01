#ifndef PCEDITDIALOG_H
#define PCEDITDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>

namespace Ui {
class PCEditDialog;
}

class PCEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCEditDialog(int pcID, uint objID, QWidget *parent = nullptr);
    ~PCEditDialog();

protected:
    void changeEvent(QEvent *e);


private slots:
    void on_toolButtonAddModelPC_clicked();

    void on_toolButtonAddOSType_clicked();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    void createUI();
    void createModel();
private:
    Ui::PCEditDialog *ui;
    int pcID;
    uint objectID;
    QSqlTableModel *modelPCType;
    QSqlTableModel *modelPCModel;
    QSqlTableModel *modelOSType;
    QSqlTableModel *modelEXEFile;
};

#endif // PCEDITDIALOG_H
