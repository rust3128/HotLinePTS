#ifndef PCVIEWFORM_H
#define PCVIEWFORM_H

#include "Clients/treepcmodel.h"

#include <QWidget>
#include <QAbstractButton>
#include <QProcess>

namespace Ui {
class PCViewForm;
}

class PCViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit PCViewForm(QWidget *parent = nullptr);
    ~PCViewForm();

public slots:
    void slotGetObjectID(uint clnID, uint objID);
protected:
    void changeEvent(QEvent *e);

private slots:
    void slotChangeIBEConn();
//    void slotFinVNC();
    void on_buttonBoxFB_clicked(QAbstractButton *button);

    void on_treeViewPC_doubleClicked(const QModelIndex &idx);

    void on_treeViewPC_expanded(const QModelIndex &index);

    void on_toolButtonPCEdit_clicked();

private:
    void createConnections();
    void showObjectFB();
    void saveObjectFB();
    void showObjectPC();
private:
    Ui::PCViewForm *ui;
    uint objectID;
    uint clientID;
    TreePCModel *modelPC;
    QProcess *vncStart;
};

#endif // PCVIEWFORM_H
