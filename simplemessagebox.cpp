#include "simplemessagebox.h"

SimpleMessageBox::SimpleMessageBox()
{

}

bool SimpleMessageBox::AskYesNo()
{
    QMessageBox qmb;

        qmb.setText("Commit has not been done.");
        qmb.setInformativeText("Do you wish to quite?");
        qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(qmb.exec() == QMessageBox::Yes)
            return true;                    //  Answer is yes.

        return false;
}

bool SimpleMessageBox::AskDelete()
{
    QMessageBox qmb;

        qmb.setText("Record Delete");
        qmb.setInformativeText("Do you wish to DELETE record?");
        qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(qmb.exec() == QMessageBox::Yes)
            return true;                    //  Answer is yes.

        return false;
}

bool SimpleMessageBox::AskSave()
{
    QMessageBox qmb;

        qmb.setText("Data has been modified.");
        qmb.setInformativeText("Do you wish to save?");
        qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(qmb.exec() == QMessageBox::Yes)
            return true;                    //  Answer is yes.

        return false;
}

void SimpleMessageBox::ErrorBox(const char * pS)
{
    QMessageBox qmb;

        qmb.setText(pS);
        qmb.setStandardButtons(QMessageBox::Ok);
        qmb.exec();
        return;
}
