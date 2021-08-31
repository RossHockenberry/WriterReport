#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include "myinclude.h"

class MainWindow;
class ReportWindow : public QWidget
{
    Q_OBJECT
public:
    explicit        ReportWindow(QTextDocument * ,QWidget *parent = nullptr);
    virtual         ~ReportWindow();
    bool            OnCreate();


signals:

private:
    void            GetScreenData();        //  Get the screen data.
    bool            InitObject();

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
