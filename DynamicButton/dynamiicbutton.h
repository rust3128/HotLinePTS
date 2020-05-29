#ifndef DYNAMIICBUTTON_H
#define DYNAMIICBUTTON_H

#include <QCommandLinkButton>
#include <QPushButton>

class DynamiicButton : public QPushButton
{
public:
    explicit DynamiicButton(uint clientID, QWidget *parent = nullptr);
    uint getButtonID();
private:
    uint buttonID=0;
};

#endif // DYNAMIICBUTTON_H
