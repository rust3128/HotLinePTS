#ifndef ADDCLIENTDIALOG_H
#define ADDCLIENTDIALOG_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class AddClientDialog;
}

class AddClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddClientDialog(QSqlRecord *rec, QWidget *parent = nullptr);
    ~AddClientDialog();

private slots:
    void on_plainTextEdit_textChanged();

    void on_pushButtonLoad_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEditName_textChanged(const QString &name);

private:
    void createUI();
private:
    Ui::AddClientDialog *ui;
    QByteArray inByteArray;
    QSqlRecord *record;
    QByteArray logoArray;
};

#endif // ADDCLIENTDIALOG_H
