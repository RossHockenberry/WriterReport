#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myinclude.h"
#include "mlocal.h"



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

//  Database Functions.
    bool        OpenDatabase();
    bool        CloseDatabase();
    bool        GetDatabaseTables();
    int         TableCount();
    bool        IsDatabaseError();
    std::string ReturnDatabaseError();

private:
    QStatusBar      *   pStatusBar;
    QTextDocument   *   pHeader;
    QTextDocument   *   pReport;
    QTextDocument   *   pFooter;

//  Database stuff.
    QSqlDatabase    oDb;        //  No copy constructor problem.
    QSqlDatabase    *   pDatabase;

//  Database Connection Info.
    std::string     sDBConnection;      //  String with all the connection info.
    std::string     sDBName;
    std::string     sDBHost;
    std::string     sDBAddress;
    std::string     sDBPort;
    std::string     sDBUser;
    std::string     sDBPassword;
    std::string     sDBDriver;

//  Other Database stuff.
    MyFlag          oOpenFlag;
    int             iNumberTables   = -1;         //  -1 means not open yet.
    int             iReturnRows     = -1;

public slots:


};
#endif // MAINWINDOW_H
