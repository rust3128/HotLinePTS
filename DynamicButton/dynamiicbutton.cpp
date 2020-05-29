#include "dynamiicbutton.h"

DynamiicButton::DynamiicButton(uint clientID, QWidget *parent) :
    QPushButton(parent)
{
    buttonID = clientID;
}

uint DynamiicButton::getButtonID()
{
    return buttonID;
}
