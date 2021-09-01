/*
    Class to show reports on the screen.
    Receives a reference to a QTextDocument that
    contains the report.
    Ross Edward Hockenberry
    09/24/2021
*/

#include "myinclude.h"
#include "mlocal.h"

ReportWindow::ReportWindow(QTextDocument * pDocument , std::string sName, QWidget *parent)
            : QWidget(parent , Qt::Window) , pParentDoc(pDocument) ,
              pParent(dynamic_cast<MainWindow *>(parent))
{
        setWindowTitle(sName.c_str());
        resize(1000 , 1000);    //  Just a starting size.
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
        pMainView->setAcceptRichText(false);

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
        pMainView->setHtml(pReport->toMarkdown());
//        pMainView->setMarkdown(pReport->toPlainText());
        return true;
}

bool ReportWindow::PrintReport()
{
//    QString         sBuild = pMainView->toPlainText();
    QString         sBuild = pMainView->toHtml();
    QTextDocument   oText; //(sBuild);

        QPrinter oPrinter(QPrinter::PrinterResolution);
        oPrinter.setOutputFormat(QPrinter::NativeFormat);
        QPrintDialog oPrintDialog(&oPrinter,this);
        oPrintDialog.exec();
        oPrinter.setPageOrientation(QPageLayout::Landscape);
        oPrinter.setDuplex(QPrinter::DuplexLongSide);
        oText.setHtml(sBuild);
        oText.print(&oPrinter);
        return true;
}

