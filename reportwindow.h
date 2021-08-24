#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include "myinclude.h"

class ReportWindow : public QWidget
{
    Q_OBJECT
public:
    explicit        ReportWindow(QTextDocument & ,QWidget *parent = nullptr);
    virtual         ~ReportWindow();
    virtual bool    OnCreate();
    void            GetScreenData();        //  Get the screen data.

signals:

private:
    QTextDocument   &   rText;
    QTextEdit       *   pMainView;
    QVBoxLayout     *   pLayout;
};

#endif // REPORTWINDOW_H
