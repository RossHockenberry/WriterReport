/*
    Ross Edward Hockenberry (c)08/25/2021.
    Version .01 r .01
    Main program of WriterReport.
    This will handle creating reports from our database.
    Initially each report will be opened in its own scrollable window
        with the option to print or save.
    Later we'll add capability of handling everything off to the reportwindow
        class to handle reports where it's to big to pass in memory. That will
        be awhile.
    08/25/2021  Removed mydataclass.  Don't need it I'm using the QSql stuff.
*/

#include "myinclude.h"
#include "mlocal.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        pParent =   static_cast<MainWindow *>(parent);

        resize(800,400);    //  Just to work with for now.
        return;
}

MainWindow::~MainWindow()
{
        return;
}

bool MainWindow::OnCreate()
{
        InitObject();
        return true;
}

bool MainWindow::InitObject()
{
        pStatusBar = statusBar();   //  Create a status bar.
        UpdateStatus("Start Initialization");
        if(!OpenDatabase())         //  No point sticking around yet.
            close();
        InitWidgets();
        SetSlots();
        SetLayouts();

//  Now let's put in some data.
        FillStoryList();
        FillReportList();
        return true;
}

bool MainWindow::InitWidgets()
{
        UpdateStatus("Initializing Widgets");

        pCentralWidget      =   new QWidget(this);
//  Create Layouts.
        pWindowLayout       =   new QVBoxLayout(this);
        pReportListLayout   =   new QHBoxLayout;
        pReportDetailLayout =   new QHBoxLayout;
        pButtonLayout       =   new QHBoxLayout;

//  Create Buttons.
        pCloseButton        =   new QPushButton("Close");
        pCancelButton       =   new QPushButton("Cancel");
        pExecuteButton      =   new QPushButton("Execyte");

//  Create Labels.
        pSelectedStoryLabel     =   new QLabel("Selected Story");
        pSelectedStory          =   new QLabel(" ");
        pSelectedReportLabel    =   new QLabel("Selected Report");
        pSelectedReport         =   new QLabel(" ");

//  Create Listbox.
        pReportList         =   new QListWidget();
        pStoryList          =   new QListWidget();

        return true;
}

bool MainWindow::SetLayouts()
{
        setCentralWidget(pCentralWidget);

        UpdateStatus("Setting up Screen Layout");

        pWindowLayout->addLayout(pReportListLayout);
        pWindowLayout->addLayout(pReportDetailLayout);
        pWindowLayout->addLayout(pButtonLayout);

//  Report List Layout.
        pReportListLayout->addWidget(pStoryList);
        pReportListLayout->addWidget(pReportList,Qt::AlignTop);

//  Report Detail Layout.
        pReportDetailLayout->addWidget(pSelectedStoryLabel);
        pReportDetailLayout->addWidget(pSelectedStory);
        pReportDetailLayout->addWidget(pSelectedReportLabel);
        pReportDetailLayout->addWidget(pSelectedReport);

//  Button Layout
        pButtonLayout->addWidget(pExecuteButton);
        pButtonLayout->addWidget(pCancelButton);
        pButtonLayout->addWidget(pCloseButton);
        pCentralWidget->setLayout(pWindowLayout);           //  Set the entire layout stack.

        return true;
}

bool MainWindow::SetSlots()
{
//  Button slots.
        connect(pCloseButton , &QPushButton::clicked , this , &MainWindow::CloseButton);
        connect(pCancelButton , &QPushButton::clicked , this , &MainWindow::CancelButton);
        connect(pExecuteButton , &QPushButton::clicked , this , &MainWindow::ExecuteButton);

//  List slots.
        connect(pReportList , &QListWidget::itemDoubleClicked , this , &MainWindow::ReportDoubleClicked);
        connect(pStoryList , &QListWidget::itemDoubleClicked , this , &MainWindow::StoryDoubleClicked);
        return true;
}

void MainWindow::UpdateStatus(const char * pString , ulong iTime)
{
        if(iTime > 5)
            iTime = 5;      //  No long waits.
        QObject().thread()->sleep(iTime);
        pStatusBar->showMessage(pString);
        return;
}

bool MainWindow::FillStoryList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vStorys.clear();        //  In case we have to recall???
        QSqlQuery oQ("SELECT story_id , name FROM story");
        while(oQ.next())
        {
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pStoryList->addItem(oVariant.toString());
            vStorys.push_back(stData);              //  In the container.
        }

        return true;
}

//-----------------------------------------------------------------------
//  Database support functions.
//-----------------------------------------------------------------------
bool MainWindow::OpenDatabase()
{
        UpdateStatus("Opening Database",3);

//  For now we'll just set things manually.
        sDBName         =   "writers_database";
        sDBHost         =   "marai";
        sDBAddress      =   "192.168.1.3";
        sDBPort         =   "5432";
        sDBUser         =   "pi";
        sDBPassword     =   "pi";
        sDBDriver       =   "QPSQL";

        oDb   =   QSqlDatabase::addDatabase(sDBDriver.c_str()); // Set the database driver.

        oDb.setHostName(sDBAddress.c_str());                    //  Use the ip for now.
        oDb.setDatabaseName(sDBName.c_str());
        oDb.setPort(atoi(sDBPort.c_str()));
        oDb.setUserName(sDBUser.c_str());
        oDb.setPassword(sDBPassword.c_str());

        pDatabase = &oDb;

        oOpenFlag.Set(oDb.open());
        GetDatabaseTables();
        return true;
}

bool MainWindow::CloseDatabase()
{
        if(oOpenFlag.Is())
            pDatabase->close();
        return true;
}

bool MainWindow::GetDatabaseTables()
{
    QStringList oSL;

        oSL = pDatabase->tables();
        iNumberTables = oSL.length();
        return true;
}

int MainWindow::TableCount()
{
        return iNumberTables;
}

bool MainWindow::IsDatabaseError()
{

        return false;
}

std::string MainWindow::ReturnDatabaseError()
{
    std::string sTemp;          //  String to handle passing error.

        return sTemp;
}

//-----------------------------------------------------------------------
//  Report Support Functions
//-----------------------------------------------------------------------
bool MainWindow::ClearReportData()
{

        return true;
}

bool MainWindow::FillReportList()
{
    stReportType    stType;

        stType.sCode = "SSL";
        stType.sName = "Story Scene List";
        vReports.push_back(stType);

        stType.sCode = "CL";
        stType.sName = "Character List";
        vReports.push_back(stType);

        for(auto & it : vReports)
            pReportList->addItem(it.sName.c_str());

        return true;
}

//-----------------------------------------------------------------------
//  Reports
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//  Button Handlers.
//-----------------------------------------------------------------------
void MainWindow::CloseButton()
{
        close();
        return;;
}

void MainWindow::CancelButton()
{

        return;;
}

void MainWindow::ExecuteButton()
{

        return;;
}

//------------------------------------------------------------------------
//  Other handlers.
//------------------------------------------------------------------------
void MainWindow::StoryDoubleClicked(QListWidgetItem * pItem)
{
    Q_UNUSED(pItem);

        return;
}

void MainWindow::ReportDoubleClicked(QListWidgetItem * pItem)
{
    Q_UNUSED(pItem);
        return;
}




