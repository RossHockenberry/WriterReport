#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include "myinclude.h"
#include "myflag.h"

class MyFlag;
class MainWindow;
class ReportWindow : public QWidget
{
    Q_OBJECT
public:
                    ReportWindow(std::string , QTextDocument * , QWidget *parent = nullptr);
    virtual         ~ReportWindow();
    bool            OnCreate();


signals:

private:
    void            GetScreenData();        //  Get the screen data.
    bool            InitObject();

//  Widgets and stuff.
    MyFlag          oDPFlag;        //  Default printer flag.

    QTextDocument   *   pParentDoc;
    MainWindow      *   pParent;
    QTextDocument   *   pReport;

    QPushButton     *   pCloseButton;
    QPushButton     *   pPrintButton;

    QTextEdit       *   pMainView;

    QVBoxLayout     *   pLayout;
    QHBoxLayout     *   pButtonLayout;

 public slots:
    bool            PrintReport();
};

#endif // REPORTWINDOW_H
