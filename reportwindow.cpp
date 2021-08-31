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
            : QWidget(parent , Qt::Window) , pParentDoc(pDocument) ,
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
        pCloseButton    =   new QPushButton("Close");
        pPrintButton    =   new QPushButton("Print");
        pLayout         =   new QVBoxLayout(this);
        pButtonLayout   =   new QHBoxLayout();

        pLayout->addWidget(pMainView);
        pButtonLayout->addWidget(pPrintButton);
        pButtonLayout->addWidget(pCloseButton);

        pLayout->addLayout(pButtonLayout);
        pReport->setHtml(pParentDoc->toHtml());     //  Copy the report.

        connect(pCloseButton , &QPushButton::clicked , this , &ReportWindow::close);
        connect(pPrintButton , &QPushButton::clicked , this , &ReportWindow::PrintReport);
        pMainView->setMarkdown(pReport->toPlainText()); //Markdown());
        return true;
}

bool ReportWindow::PrintReport()
{
    QString         sBuild = pMainView->toPlainText();
    QTextDocument   oText(sBuild);

        QPrinter oPrinter(QPrinter::PrinterResolution);
        QPrintDialog oPrintDialog(&oPrinter,this);
        oPrintDialog.exec();
        oPrinter.setPageOrientation(QPageLayout::Landscape);
        oPrinter.setDuplex(QPrinter::DuplexLongSide);
        oText.print(&oPrinter);
        return true;
}

