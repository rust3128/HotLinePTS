#include "pcviewform.h"
#include "ui_pcviewform.h"
#include "LoggingCategories/loggingcategories.h"
#include "pceditdialog.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

PCViewForm::PCViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCViewForm)
{
    ui->setupUi(this);

    ui->toolBox->setStyleSheet("QToolBox::tab#toolBox {"
                            "   background: #009deb;"
                                "border-radius: 5px;"
                                "font-size: 15px;"
                                "icon-size: 48px;"
                                "color: black;"
                            "}"
                            "QToolBox::tab:first#toolBox {"
                                "background: #4ade00;"
                                "border-radius: 5px;"
                                "font-size: 15px;"
                                "color: black;"
                            "}"
                            "QToolBox::tab:last#toolBox {"
                                "background: #f95300;"
                                "border-radius: 5px;"
                                "font-size: 15px;"
                                "color: black;"
                            "}"
                            "QToolBox::tab:selected#toolBox {" /* italicize selected tabs */
                                "font: italic bold;"
                                "font-size: 15px;"
                                "color: white;"
                            "}");


    createConnections();
}

PCViewForm::~PCViewForm()
{
    delete ui;
}

void PCViewForm::slotGetObjectID(uint clnID, uint objID)
{
    objectID = objID;
    clientID = clnID;

    modelPC = new TreePCModel(objectID,this);
    showObjectPC();
    showObjectFB();
}

void PCViewForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void PCViewForm::showObjectFB()
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("select f.servername, f.serverport, f.databasename from objectfb f where f.object_id= :objectID");
    q->bindValue(":objectID", objectID);
    if(!q->exec())
        qInfo(logInfo()) << "Не удалось получить параметры сервера FB" << q->lastError().text();
    if(q->next()){
        ui->lineEditServerFB->setText(q->value(0).toString());
        ui->lineEditDatabaseName->setText(q->value(2).toString());
        ui->lineEditPortFB->setText((q->value(1).toUInt()!=0) ? q->value(1).toString() : "");
    } else {
        ui->lineEditPortFB->clear();
        ui->lineEditServerFB->clear();
        ui->lineEditDatabaseName->clear();
    }
}



void PCViewForm::on_buttonBoxFB_clicked(QAbstractButton *button)
{
    switch (ui->buttonBoxFB->standardButton(button)) {
    case QDialogButtonBox::Save:
        saveObjectFB();
        break;
    case QDialogButtonBox::Reset:
        ui->lineEditPortFB->clear();
        ui->lineEditServerFB->clear();
        ui->lineEditDatabaseName->clear();
        break;
    default:
        break;
    }
}

void PCViewForm::createConnections()
{
    connect(ui->lineEditServerFB,&QLineEdit::textChanged,this,&PCViewForm::slotChangeIBEConn);
    connect(ui->lineEditPortFB,&QLineEdit::textChanged,this,&PCViewForm::slotChangeIBEConn);
    connect(ui->lineEditDatabaseName,&QLineEdit::textChanged,this,&PCViewForm::slotChangeIBEConn);
}

void PCViewForm::saveObjectFB()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Подтверждение"),
                          QString::fromUtf8("Сохранить параметры сервера?"),
                          QMessageBox::Yes | QMessageBox::No );
    if (reply == QMessageBox::No)
        return;

    QSqlQuery *q = new QSqlQuery();
    q->prepare("UPDATE OR INSERT INTO OBJECTFB (OBJECT_ID, SERVERNAME, SERVERPORT, DATABASENAME) "
               "VALUES (:objectID, :server, :port, :database) "
               "MATCHING (OBJECT_ID)");
    q->bindValue(":objectID", objectID);
    q->bindValue(":server", ui->lineEditServerFB->text().trimmed());
    q->bindValue(":port", ui->lineEditPortFB->text().trimmed());
    q->bindValue(":database", ui->lineEditDatabaseName->text().trimmed());
    if(!q->exec())
        qInfo(logInfo()) << "Не удалось обновить параметры сервера FB" << q->lastError().text();
}

void PCViewForm::showObjectPC()
{

    ui->treeViewPC->setModel(modelPC);

    ui->treeViewPC->resizeColumnToContents(0);
    ui->treeViewPC->resizeColumnToContents(1);
    ui->treeViewPC->hideColumn(2);
}

void PCViewForm::slotChangeIBEConn()
{
    QString server,port,database;
    server = ui->lineEditServerFB->text().trimmed();
    database = (ui->lineEditDatabaseName->text().trimmed().size()==0) ? "" : ":"+ui->lineEditDatabaseName->text().trimmed();
    port = (ui->lineEditPortFB->text().trimmed().size()==0) ? "" : "/"+ui->lineEditPortFB->text().trimmed();
    ui->lineEditIBEConnect->setText(server+port+database);
}

void PCViewForm::on_treeViewPC_doubleClicked(const QModelIndex &idx)
{
//    QModelIndex parentIndex = idx.parent();
//            modelPC->index(idx.row(),idx.column(), QModelIndex());
//    qInfo(logInfo()) << "parentindex row" << parentIndex.row() << "parentindex column" << parentIndex.column()  << "row" << idx.row() << "column" << idx.column() ;
}

void PCViewForm::on_treeViewPC_expanded(const QModelIndex &index)
{
    Q_UNUSED(index)
    ui->treeViewPC->resizeColumnToContents(0);
    ui->treeViewPC->resizeColumnToContents(1);
}


void PCViewForm::on_toolButtonPCEdit_clicked()
{
    QModelIndex idx = ui->treeViewPC->currentIndex();
    if(!idx.isValid()){
        return;
    }
    QModelIndex parentIndex = idx.parent();
    if(!parentIndex.isValid()){
        parentIndex = idx;
    }
    PCEditDialog *pcDlg = new PCEditDialog(modelPC->data(modelPC->index(parentIndex.row(),2,QModelIndex()),Qt::DisplayRole).toUInt(),objectID,this);
//    qInfo(logInfo()) << "column 2" << modelPC->data(modelPC->index(parentIndex.row(),2,QModelIndex()),Qt::DisplayRole).toString();
    pcDlg->exec();
}
