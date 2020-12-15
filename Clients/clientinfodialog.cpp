#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "Clients/editobjectdialog.h"
#include "Clients/pcviewform.h"


#include <QSqlQuery>
#include <QSqlError>



ClientInfoDialog::ClientInfoDialog(uint ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog),
    clientID(ID)
{
    ui->setupUi(this);

    createModel();
    createUI();
    createConnections();
    createTitle();
    connect(ui->tableViewObjects->selectionModel(),&QItemSelectionModel::selectionChanged,this,&ClientInfoDialog::slotSelectTerminals);
    connect(this,&ClientInfoDialog::signalSendID,ui->widgetPC,&PCViewForm::slotGetObjectID);

}

ClientInfoDialog::~ClientInfoDialog()
{
    delete ui;
}

void ClientInfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void ClientInfoDialog::createModel()
{
    QString strSQL = QString("select o.object_id, o.terminal_id, o.name,o.phone,o.phonedir, o.email, o.comments from objects o "
                             "where o.client_id=%1 and o.isactive = 'true' "
                             "order by o.terminal_id").arg(clientID);
    modelObjects = new QSqlQueryModel(this);
    modelObjects->setQuery(strSQL);
    modelObjects->setHeaderData(1,Qt::Horizontal, "№ АЗС");
    modelObjects->setHeaderData(2,Qt::Horizontal, "Расположение");
    modelObjects->setHeaderData(3,Qt::Horizontal, "Телефон АЗС");
    modelObjects->setHeaderData(4,Qt::Horizontal, "Тел.Директора");
    modelObjects->setHeaderData(5,Qt::Horizontal, "E-mail");
    modelObjects->setHeaderData(6,Qt::Horizontal, "Примечания");
}
void ClientInfoDialog::createUI()
{
    qApp->setStyleSheet(
        "QSplitter::handle:vertical   {height: 6px; image: url(images/vsplitter.png);}"
        "QSplitter::handle:horizontal {width:  6px; image: url(images/hsplitter.png);}"
        );


    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);
    ui->widgetPC->hide();

    ui->tableViewObjects->setModel(modelObjects);
    ui->tableViewObjects->verticalHeader()->hide();
    ui->tableViewObjects->hideColumn(0);
    ui->tableViewObjects->resizeColumnsToContents();
    ui->tableViewObjects->resizeRowsToContents();
    ui->tableViewObjects->horizontalHeader()->setStretchLastSection(true);

    QString SS =QString("QHeaderView::section {background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0.498, y2:0, stop:0 rgba(81, 81, 81, 255), stop:1 rgba(185, 185, 185, 255));"
                        "font-weight: bold;}");
    ui->tableViewObjects->setStyleSheet(SS);
}

void ClientInfoDialog::createConnections()
{

}


void ClientInfoDialog::createTitle()
{
    QSqlQuery q;
    q.prepare("SELECT name, logo, comments FROM clients WHERE client_id = :clientID");
    q.bindValue(":clientID", clientID);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось получить данные о клиенте."  << q.lastError().text();
        this->reject();
    }
    q.next();
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(q.value(1).toByteArray());
    if(!outPixmap.isNull()){
        ui->labelLogo->setPixmap(  (outPixmap.size().width() > 90) ? outPixmap.scaledToWidth(90) : outPixmap  );
    }
    else
        ui->labelLogo->clear();
    ui->labelName->setText(q.value(0).toString());
    ui->labelComments->setText(q.value(2).toString());

}


void ClientInfoDialog::on_tableViewObjects_doubleClicked(const QModelIndex &idx)
{
    QSqlRecord rec;
    rec = modelObjects->record(idx.row());
    EditObjectDialog *edObjDlg = new EditObjectDialog(&rec, ui->labelName->text(), clientID, this);
    if(edObjDlg->exec() == QDialog::Accepted)
        modelObjects->setQuery(modelObjects->query().lastQuery());
}

void ClientInfoDialog::on_toolButton_clicked()
{
    QSqlRecord rec;
    rec.clear();
    EditObjectDialog *edObjDlg = new EditObjectDialog(&rec, ui->labelName->text(), clientID, this);
    if(edObjDlg->exec() == QDialog::Accepted)
        modelObjects->setQuery(modelObjects->query().lastQuery());
}


void ClientInfoDialog::slotSelectTerminals(const QItemSelection &, const QItemSelection &)
{
    if(ui->widgetPC->isHidden())
        ui->widgetPC->show();
    QModelIndexList selection = ui->tableViewObjects->selectionModel()->selectedIndexes();
    emit signalSendID(clientID, modelObjects->data(modelObjects->index(selection.at(0).row(),0),Qt::DisplayRole).toInt(),
                      modelObjects->data(modelObjects->index(selection.at(0).row(),2),Qt::DisplayRole).toString(),
                      modelObjects->data(modelObjects->index(selection.at(0).row(),3),Qt::DisplayRole).toString(),
                      modelObjects->data(modelObjects->index(selection.at(0).row(),4),Qt::DisplayRole).toString());
}
