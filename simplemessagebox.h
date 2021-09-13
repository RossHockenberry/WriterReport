#ifndef SIMPLEMESSAGEBOX_H
#define SIMPLEMESSAGEBOX_H

#include "myinclude.h"

class SimpleMessageBox
{
//    Q_OBJECT
public:
    SimpleMessageBox();

    bool            AskYesNo();
    bool            AskSave();
    bool            AskDelete();
    void            ErrorBox(const char *);
};

#endif // SIMPLEMESSAGEBOX_H
