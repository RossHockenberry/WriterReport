/*
    Class to show reports on the screen.
    Receives a reference to a QTextDocument that
    contains the report.
    Ross Edward Hockenberry
    09/24/2021
*/

#include "myinclude.h"
#include "mlocal.h"

//  Have to initialize out reference value here of course.
//  May want to copy the textdoc if we decide to leave report windows
//  laying around.
ReportWindow::ReportWindow(QTextDocument & rTextDocument,QWidget *parent)
                            : QWidget(parent) ,  rText(rTextDocument)
{

        return;
}

ReportWindow::~ReportWindow()
{
        return;
}

bool ReportWindow::OnCreate()
{

        return true;
}
