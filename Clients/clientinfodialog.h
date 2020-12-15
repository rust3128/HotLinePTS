#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QItemSelection>

namespace Ui {
class ClientInfoDialog;
}

class ClientInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoDialog(uint ID, QWidget *parent = nullptr);
    ~ClientInfoDialog();

protected:
    void changeEvent(QEvent *e);

signals:
    void signalSendID(uint, uint, QString, QString, QString);

private slots:
    void slotSelectTerminals(const QItemSelection &, const QItemSelection &);
    void on_tableViewObjects_doubleClicked(const QModelIndex &idx);

    void on_toolButton_clicked();

private:
    void createUI();
    void createConnections();
    void createTitle();
    void createModel();

private:
    Ui::ClientInfoDialog *ui;
    uint clientID;
    QSqlQueryModel *modelObjects = nullptr;

};

#endif // CLIENTINFODIALOG_H
