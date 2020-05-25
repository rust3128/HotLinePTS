#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "GlobalSettings/globalsettings.h"
#include "OptionsDialog/optionsdialog.h"
#include "Clients/clientslistdialog.h"


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
