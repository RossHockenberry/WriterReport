#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class MyDataClass;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool        OnCreate();

//  General Functions.
    bool        InitObject();
    bool        InitWidgets();
    bool        SetSlots();

private:
    QStatusBar      *   pStatusBar;

    MyDataClass     *   pDb;

public slots:


};
#endif // MAINWINDOW_H
