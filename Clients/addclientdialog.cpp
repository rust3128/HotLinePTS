#include "addclientdialog.h"
#include "ui_addclientdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QFileDialog>
#include <QBuffer>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

AddClientDialog::AddClientDialog(QSqlRecord *rec, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClientDialog),
    record(rec)
{
    ui->setupUi(this);
    createUI();
}

AddClientDialog::~AddClientDialog()
{
    delete ui;
}

void AddClientDialog::on_plainTextEdit_textChanged()
{
    if(ui->plainTextEdit->placeholderText().length() > 100)
        ui->plainTextEdit->setPlainText(ui->plainTextEdit->placeholderText().left(100));
}


void AddClientDialog::on_pushButtonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть логотип"), "", tr("Изображения (*.png *.jpeg *.jpg);;Все файлы (*.*)"));
    QPixmap inPixmap(fileName); // Сохраняем его в изображение объекта QPixmap;

    if(inPixmap.width()>800 || inPixmap.height()>600){
        QMessageBox::warning(this,"Внимание","Размер логотипа слишком большой. Выберите другой файл.");
        return;
    }

    ui->labelLogo->setPixmap(inPixmap);
    QBuffer inBuffer( &inByteArray );                   // Сохранение изображения производим через буффер
    inBuffer.open( QIODevice::WriteOnly );              // Открываем буффер
    inPixmap.save( &inBuffer, "PNG" );                  // Записываем inPixmap в inByteArray
}

void AddClientDialog::on_buttonBox_accepted()
{

    if(record->isEmpty()){
        int result = QMessageBox::question(this,tr("Вопрос"),
                                           QString(tr("Вы дейстиветльно хотите добавить клиента %1 в базу данных?"))
                                           .arg(ui->lineEditName->text().trimmed()));
        if(result == QMessageBox::Yes){
            QSqlQuery q;
            q.prepare("INSERT INTO clients (name, logo, comments) "
                      "VALUES (:name, :logo, :comments)");
            q.bindValue(":name", ui->lineEditName->text().trimmed());
            q.bindValue(":logo", inByteArray);
            q.bindValue(":comments", ui->plainTextEdit->toPlainText());
            if(!q.exec()) {
                qCritical(logCritical()) << "Не возможно добавить клиента." << endl << q.lastError().text();
                return;
            }
            qInfo(logInfo()) << "Клиент добавлен.";
        }
    } else {
        QSqlQuery q;

        q.prepare("UPDATE clients SET name = :name, logo = :logo, comments = :comments "
                  "WHERE client_id = :clientID");
        q.bindValue(":name", ui->lineEditName->text().trimmed());
        q.bindValue(":logo",  (record->value(2).isNull()) ? inByteArray : logoArray);
        q.bindValue(":comments", ui->plainTextEdit->toPlainText());
        q.bindValue(":clientID", record->value(0).toInt());
        if(!q.exec()) {
            qCritical(logCritical()) << "Не возможно обновить данные клиента." << endl << q.lastError().text();
            return;
        }
        qInfo(logInfo()) << "Информация о клиенте обновлена.";
    }
    this->accept();
}

void AddClientDialog::on_buttonBox_rejected()
{
    this->reject();
}

void AddClientDialog::on_lineEditName_textChanged(const QString &name)
{
    bool enabled = (name.length() >2) ? true : false;
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(enabled);
}

void AddClientDialog::createUI()
{
    if(record->isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
        return;
    }
    this->setWindowTitle(tr("Редактирование данных клиента"));
    ui->lineEditName->setText(record->value(1).toString());
    QPixmap outPixmap = QPixmap();
    logoArray = record->value(2).toByteArray();
    outPixmap.loadFromData(logoArray);
    if(!outPixmap.isNull())
        ui->labelLogo->setPixmap(outPixmap);
    else
        ui->labelLogo->setText("Логотип отсутствует");
    ui->plainTextEdit->setPlainText(record->value(3).toString());
}
