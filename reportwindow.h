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
    virtual bool    OnCreate();


signals:

private:
    void            GetScreenData();        //  Get the screen data.
    bool            InitObject();
    bool            SetLayout();
    void            SetText();

    QTextDocument   *   pParentDoc;
    MainWindow      *   pParent;
    QTextDocument   *   pReport;

    QTextEdit       *   pMainView;
    QVBoxLayout     *   pLayout;
};

#endif // REPORTWINDOW_H
