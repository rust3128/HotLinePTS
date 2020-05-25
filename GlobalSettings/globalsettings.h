#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QString>

class GlobalSettings
{
public:
    GlobalSettings();
    static const QString CONFIG_FILE_NAME;


    ///Опции

    enum OPTIONS{
        OPT_MIMIMAZE_TO_TRAY =1000                  //Сворачивать в трей
    };
};

#endif // GLOBALSETTINGS_H
