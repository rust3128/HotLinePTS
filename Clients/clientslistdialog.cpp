#include "clientslistdialog.h"
#include "ui_clientslistdialog.h"
#include "Clients/addclientdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QSqlError>

ClientsListDialog::ClientsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientsListDialog)
{
    ui->setupUi(this);

    createModel();
    createUI();
}

ClientsListDialog::~ClientsListDialog()
{
    delete ui;
}

void ClientsListDialog::createUI()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->tableViewList->setModel(modelClients);
    ui->tableViewList->setColumnHidden(0,true);
    ui->tableViewList->setColumnHidden(2,true);
    ui->tableViewList->setColumnHidden(4,true);
    ui->tableViewList->setColumnHidden(5,true);

    ui->tableViewList->verticalHeader()->hide();
    ui->tableViewList->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableViewList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this, SLOT(slotCurrentPic(QModelIndex)));
    modelClients->select();
    qDebug(logDebug()) << "Model Clients" << modelClients->lastError().text();
    ui->tableViewList->resizeColumnsToContents();
}

void ClientsListDialog::createModel()
{
    modelClients = new QSqlTableModel();
    modelClients->setTable("CLIENTS");
    modelClients->setHeaderData(1,Qt::Horizontal,"Наименование");
    modelClients->setHeaderData(3,Qt::Horizontal,"Коментарии");
    currentRecord = new QSqlRecord();

}

void ClientsListDialog::on_buttonBox_accepted()
{
    currentRecord->clear();
    AddClientDialog *addClient = new AddClientDialog(currentRecord, this);
    addClient->exec();
    modelClients->select();
    ui->tableViewList->resizeColumnsToContents();
}

void ClientsListDialog::slotCurrentPic(QModelIndex idx)
{

    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(modelClients->data(modelClients->index(idx.row(), 2)).toByteArray());
    // Устанавливаем изображение в picLabel
    if(!outPixmap.isNull())
        ui->labelLogo->setPixmap(outPixmap.scaledToWidth(150));
    else
        ui->labelLogo->setText("Логотип отсутствует");
}

void ClientsListDialog::on_buttonBox_rejected()
{
    this->reject();
}

void ClientsListDialog::on_tableViewList_doubleClicked(const QModelIndex &idx)
{
    QSqlRecord r = modelClients->record(idx.row());
    currentRecord = &r;
    AddClientDialog *addClient = new AddClientDialog(currentRecord, this);
    addClient->exec();
    modelClients->select();
    ui->tableViewList->resizeColumnsToContents();
}
