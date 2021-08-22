#ifndef SIMPLEMESSAGEBOX_H
#define SIMPLEMESSAGEBOX_H

#include <QMessageBox>
#include <QObject>

class SimpleMessageBox
{
//    Q_OBJECT
public:
    SimpleMessageBox();

    bool            AskYesNo();
    bool            AskSave();
    bool            AskDelete();                //  Should make these there own classes or combine them.
    void            ErrorBox(const char *);     //  Attempt to save dulicate record.
                                                //  Mainly name field.
};

#endif // SIMPLEMESSAGEBOX_H
