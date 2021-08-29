#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myinclude.h"
#include "mlocal.h"
#include "myflag.h"
class MyFlag;

struct stReportType
{
    std::string     sCode;
    std::string     sName;
};

struct stTypeData
 {
     int             iKey;
     std::string     sKeyString;
     int             iPosKey;            //  Position in table or subset of table.
 };

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
    bool        SetLayouts();
    bool        SetSlots();
    void        UpdateStatus(const char *,ulong  iTime = 0);
    bool        FillStoryList();

//  Report Functions.
    bool        ClearReportData();
    bool        FillReportList();

//  Database Functions.
    bool        OpenDatabase();
    bool        CloseDatabase();
    bool        GetDatabaseTables();
    int         TableCount();
    bool        IsDatabaseError();
    std::string ReturnDatabaseError();

//  Actual Reports.
    bool        FullCharacterReport();

//  Some public data.
    MainWindow      *   pParent;

private:
//  Layouts.
    QVBoxLayout     *   pWindowLayout;
    QHBoxLayout     *   pReportListLayout;
    QHBoxLayout     *   pReportDetailLayout;
    QHBoxLayout     *   pButtonLayout;

//  Widgets.
    QWidget         *   pCentralWidget;
    QPushButton     *   pCloseButton;
    QPushButton     *   pExecuteButton;
    QPushButton     *   pCancelButton;

    QListWidget     *   pReportList;
    QListWidget     *   pStoryList;
    QLabel          *   pSelectedStory;
    QLabel          *   pSelectedReport;
    QLabel          *   pSelectedStoryLabel;
    QLabel          *   pSelectedReportLabel;

//  Misc.
    QStatusBar      *   pStatusBar;
    QTextDocument   *   pHeader;
    QTextDocument   *   pReport;
    QTextDocument   *   pFooter;

//  A vector container for report names and codes.
    std::vector<stTypeData>     vReports;
    std::string                 sSelectedReport;
    int                         iSelectedReport = 0;

    std::vector<stTypeData>     vStorys;
    std::string                 sSelectedStory;
    int                         iSelectedStory = 0;

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
    void            CloseButton();
    void            CancelButton();
    void            ExecuteButton();
    void            StoryDoubleClicked(QListWidgetItem *);
    void            ReportDoubleClicked(QListWidgetItem *);
};
#endif // MAINWINDOW_H
