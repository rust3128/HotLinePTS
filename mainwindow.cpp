#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "GlobalSettings/globalsettings.h"
#include "OptionsDialog/optionsdialog.h"
#include "Clients/clientslistdialog.h"
#include "DynamicButton/dynamiicbutton.h"
#include "Clients/addclientdialog.h"



#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCheckBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
    setToolBarClient();
    m_clnInfDlg=nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotExit()
{
    toClose=true;
}



void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        /* Событие игнорируется в том случае, если чекбокс не отмечен
         * */
            /* иначе, если окно видимо, то оно скрывается,
             * и наоборот, если скрыто, то разворачивается на экран
             * */
            if(!this->isVisible()){
                this->show();
            } else {
                this->hide();
        }
        break;
    default:
        break;
    }
}

void MainWindow::createUI()
{
    /* Инициализируем иконку трея, устанавливаем иконку из набора системных иконок,
     * а также задаем всплывающую подсказку
     * */
    trayIcon = new QSystemTrayIcon(QIcon(":/Images/logo.png"), this);
    trayIcon->setToolTip("Hotline PTS" "\n"
                         "Рабочее место сотрудника горячей линии");
    /* После чего создаем контекстное меню из двух пунктов*/
    QMenu * trayMenu = new QMenu(this);
    QAction * viewWindow = new QAction(tr("Развернуть окно"), this);
    QAction * quitAction = new QAction(tr("Выход"), this);

    /* подключаем сигналы нажатий на пункты меню к соответсвующим слотам.
     * Первый пункт меню разворачивает приложение из трея,
     * а второй пункт меню завершает приложение
     * */
    connect(viewWindow, &QAction::triggered, this, &QWidget::show);
    connect(quitAction, &QAction::triggered, this, &MainWindow::slotExit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayMenu->addAction(viewWindow);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    /* Устанавливаем контекстное меню на иконку
     * и показываем иконку приложения в трее
     * */
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    /* Также подключаем сигнал нажатия на иконку к обработчику
     * данного нажатия
     * */
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
}

void MainWindow::setToolBarClient()
{
    QSqlQuery q;

    q.prepare("SELECT client_id, UPPER(name) FROM clients WHERE isactive = 'true' ORDER BY cnt DESC, client_ID");
    if(!q.exec()){
        qCritical(logCritical()) << "Не возможно получить список клиентов" << q.lastError().text();
        return;
    }
    while(q.next()){
         DynamiicButton *button = new DynamiicButton(q.value(0).toUInt(),this);
         button->setText(q.value(1).toString());
         button->setFlat(true);
         button->setStyleSheet("font-size: 16px;"
                               "font-weight: bold;"
                               "font-variant: small-caps;"
                               "font-family: serif;"
                               "padding-left:10px;"
                               "padding-right:10px;"
                               "min-width: 140px;"
                               "max-width: 1400px;"
                               "min-height: 20px;"
                               "max-height: 20px;");
         ui->toolBarClients->addSeparator();
         ui->toolBarClients->addWidget(button);

          connect(button,&QAbstractButton::clicked,this,&MainWindow::slotGetNumberButton);
     }
    ui->toolBarClients->addSeparator();
     q.finish();

     QWidget *spacerWidget = new QWidget(this);
     spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     spacerWidget->setVisible(true);
     QPushButton *pbNewClient = new QPushButton(this);
     pbNewClient->setStyleSheet("font-size: 16px;"
                              "font-weight: bold;"
                              "font-variant: small-caps;"
                              "font-family: serif;"
                              "padding-left:10px;"
                              "padding-right:10px;"
                              "min-width: 140px;"
                              "max-width: 140px;"
                              "min-height: 20px;"
                              "max-height: 20px;");
     pbNewClient->setText("Новый клиент");
     pbNewClient->setFlat(true);

     ui->toolBarClients->addWidget(spacerWidget);
     ui->toolBarClients->addSeparator();
     ui->toolBarClients->addWidget(pbNewClient);
     ui->toolBarClients->addSeparator();
     connect(pbNewClient,&QAbstractButton::clicked,[]() {
         QSqlRecord r;
         r.clear();
         AddClientDialog *addClnDlg = new AddClientDialog(&r);
         addClnDlg->exec();
     });

}



void MainWindow::closeEvent(QCloseEvent *event)
{
    QSqlQuery q;
        bool toTray;
        q.prepare("select o.optionvalue from options o where o.option_id=:optionID");
        q.bindValue(":optionID",GlobalSettings::OPT_MIMIMAZE_TO_TRAY);
        if(!q.exec()){
            qCritical(logCritical()) << "Не возможно получть значение параметра " << GlobalSettings::OPT_MIMIMAZE_TO_TRAY
                                     << q.lastError().text();
        }
        q.next();
        toTray = q.value(0).toBool();
        /* Если окно видимо и чекбокс отмечен, то завершение приложения
         * игнорируется, а окно просто скрывается, что сопровождается
         * соответствующим всплывающим сообщением
         */
        if(this->isVisible() && !q.isValid()){
            // Отображаем MessageBox c вопросом о закрытии
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Закрыть программу"));
            msgBox.setText(tr("<b>Свернуть программу в трей вместо закрытия?</b><p>(Изменить параметр можно в меню <b>Настройка</b>)."));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.addButton(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            QCheckBox dontShowCheckBox(tr("Запомнить мой выбор."));
            dontShowCheckBox.blockSignals(true);
            msgBox.addButton(&dontShowCheckBox, QMessageBox::ResetRole);
            int32_t userReply = msgBox.exec();
            toTray = (userReply == QMessageBox::Yes) ? true : false;
            if(dontShowCheckBox.isChecked()){
                q.prepare("INSERT INTO OPTIONS (OPTION_ID, OPTIONVALUE, COMMENT) "
                          "VALUES (:optionID, :optionValue, :optComment)");
                q.bindValue(":optionID", GlobalSettings::OPT_MIMIMAZE_TO_TRAY);
                q.bindValue(":optionValue",toTray);
                q.bindValue(":optComment", "Сворачивать программу в трей");
                q.exec();
            }
        }
        if(this->isVisible() && toTray && !toClose){
            event->ignore();
            this->hide();
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

            trayIcon->showMessage("Toolkit Hotline",
                                  tr("Приложение свернуто в трей. Для того чтобы, "
                                      "развернуть окно приложения, щелкните по иконке приложения в трее"),
                                  icon,
                                  2000);
        }
}
void MainWindow::on_actionParametrs_triggered()
{
    OptionsDialog *optDlg = new OptionsDialog(this);
    optDlg->setWindowTitle(this->windowTitle()+" - [Настройка]");
    optDlg->exec();

}

void MainWindow::on_actionClientsList_triggered()
{
    ClientsListDialog *clnLstDlg = new ClientsListDialog(this);
    clnLstDlg->setWindowTitle(this->windowTitle()+" - [Список клиентов]");
    clnLstDlg->exec();

}

void MainWindow::slotGetNumberButton()
{
    DynamiicButton *button = static_cast<DynamiicButton*>(sender());
    QSqlQuery *q = new QSqlQuery();
    const int clientID = button->getButtonID();
    q->prepare("UPDATE clients SET cnt = cnt +1 WHERE client_id=:clientid");
    q->bindValue(":clientid",clientID);
    if(!q->exec()) qCritical(logCritical()) << "Не возможно выполнить запрос " << q->lastQuery() << q->lastError().text();
    if (m_clnInfDlg)
    {
        m_clnInfDlg->deleteLater();
    }
    m_clnInfDlg = new ClientInfoDialog(clientID);
    m_clnInfDlg->setAttribute(Qt::WA_DeleteOnClose);
    this->setCentralWidget(m_clnInfDlg);
//    m_clnInfDlg->show();

}
