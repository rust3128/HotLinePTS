#include "mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBase/database.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>

// Умный указатель на файл логирования
static QScopedPointer<QFile>   m_logFile;

// Объявление обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Запуск только одной копии приложения
        QSystemSemaphore semaphore("HotlinePTS Semafore", 1);  // создаём семафор
        semaphore.acquire(); // Поднимаем семафор, запрещая другим экземплярам работать с разделяемой памятью

    #ifndef Q_OS_WIN32
        // в linux/unix разделяемая память не освобождается при аварийном завершении приложения,
        // поэтому необходимо избавиться от данного мусора
        QSharedMemory nix_fix_shared_memory("<uniq id 2>");
        if(nix_fix_shared_memory.attach()){
            nix_fix_shared_memory.detach();
        }
    #endif

        QSharedMemory sharedMemory("HotlinePTS Memoty");  // Создаём экземпляр разделяемой памяти
        bool is_running;            // переменную для проверки ууже запущенного приложения
        if (sharedMemory.attach()){ // пытаемся присоединить экземпляр разделяемой памяти
                                    // к уже существующему сегменту
            is_running = true;      // Если успешно, то определяем, что уже есть запущенный экземпляр
        }else{
            sharedMemory.create(1); // В противном случае выделяем 1 байт памяти
            is_running = false;     // И определяем, что других экземпляров не запущено
        }
        semaphore.release();        // Опускаем семафор

        // Если уже запущен один экземпляр приложения, то сообщаем об этом пользователю
        // и завершаем работу текущего экземпляра приложения
        if(is_running){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle("Hotline PTS");
            msgBox.setText(QObject::tr("Приложение <b>Workplace Hotline</b> уже запущено.<br>"
                                       "Вы можете запустить только один экземпляр приложения."
                                       "<p>Активируйте приложение в системном лотке."));
            msgBox.exec();
            return 1;
        }

    // Устанавливаем файл логирования
    m_logFile.reset(new QFile("HotlinePTS.log"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);

#ifndef QT_NO_TRANSLATION
    //Определяем имя языкового файла в зависимости от текущей локали
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    //Создаем и пытаемся загрузить
    QTranslator *translator = new QTranslator(&a);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
    else
        qWarning(logWarning()) << "Не удалось загрузить языковый файл.";
#endif

    DataBase *db = new DataBase();
    if(!db->connectDB()){
        qCritical(logCritical()) << "Аварийное завершение работы.";
        return 1;
    }



    qInfo(logInfo()) << "Запуск программы.";
    MainWindow w;
    w.show();
    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:
        out << "[INF] ";
        console << "Info:     " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << endl;
        break;
    case QtDebugMsg:
        out << "[DBG] " ;
        console << "Debug:    " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << endl;
        break;
    case QtWarningMsg:
        out << "[WRN] ";
        console << "Warning:  " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] "  << endl;
        break;
    case QtCriticalMsg:
        out << "[CRT] ";
        console << "Critical: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << endl;
        break;
    case QtFatalMsg:
        out << "[FTL] ";
        console << "Fatality: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << endl;
        break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }

    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg;
    out << "  ***[" << context.function << " File: " << context.file << " Line: " << context.line << "]***" << endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}
