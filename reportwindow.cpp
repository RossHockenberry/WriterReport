/*
    Class to show reports on the screen.
    Receives a reference to a QTextDocument that
    contains the report.
    Ross Edward Hockenberry
    09/24/2021
*/

#include "myinclude.h"
#include "mlocal.h"

ReportWindow::ReportWindow(QTextDocument * pDocument , QWidget *parent)
            : QWidget(parent) , pParentDoc(pDocument) ,
              pParent(dynamic_cast<MainWindow *>(parent))
{
        return;
}

ReportWindow::~ReportWindow()
{
        return;
}

bool ReportWindow::OnCreate()
{
        InitObject();
        return true;
}

bool ReportWindow::InitObject()
{
        pMainView       =   new QTextEdit();
        pReport         =   new QTextDocument();

        pReport->setHtml(pParentDoc->toHtml());     //  Copy the report.
        return true;
}

void ReportWindow::SetText()
{
        pMainView->setMarkdown(pReport->toMarkdown());
//        pEditor->setFont(oEditorFont);
//        pCursor->movePosition(QTextCursor::End);    //  Put the cursor at the end.
//        pEditor->setTextCursor(*pCursor);
        return;
}
