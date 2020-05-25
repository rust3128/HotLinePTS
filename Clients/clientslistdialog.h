#ifndef CLIENTSLISTDIALOG_H
#define CLIENTSLISTDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>


namespace Ui {
class ClientsListDialog;
}

class ClientsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientsListDialog(QWidget *parent = nullptr);
    ~ClientsListDialog();

private slots:
    void on_buttonBox_accepted();
    void slotCurrentPic(QModelIndex idx);

    void on_buttonBox_rejected();

    void on_tableViewList_doubleClicked(const QModelIndex &idx);

private:
    void createUI();
    void createModel();
private:
    Ui::ClientsListDialog *ui;
    QSqlTableModel *modelClients;
    QSqlRecord *currentRecord;
};

#endif // CLIENTSLISTDIALOG_H
