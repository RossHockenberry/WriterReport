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
        pWindowLayout       =   new QVBoxLayout();  //  Okay, don't set a parent here?
        pReportListLayout   =   new QHBoxLayout;
        pReportDetailLayout =   new QHBoxLayout;
        pButtonLayout       =   new QHBoxLayout;

//  Create Buttons.
        pCloseButton        =   new QPushButton("Close");
        pCancelButton       =   new QPushButton("Cancel");
        pExecuteButton      =   new QPushButton("Execute");

//  Create Labels.
        pSelectedStoryLabel     =   new QLabel("Selected Story");
        pSelectedStory          =   new QLabel(" ");
        pSelectedStory->setFrameStyle(QFrame::Panel  | QFrame::Sunken);
        pSelectedStory->setLineWidth(2);

        pSelectedReportLabel    =   new QLabel("Selected Report");
        pSelectedReport         =   new QLabel(" ");
        pSelectedReport->setFrameStyle(QFrame::Panel |  QFrame::Sunken);
        pSelectedReport->setLineWidth(2);   //  Add some depth to our sunken lables.

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
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pStoryList->addItem(oVariant.toString());
            vStorys.push_back(stData);              //  In the container.
        }

        return true;
}

bool MainWindow::FillReportList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vReports.clear();
        QSqlQuery oQ("SELECT id , name FROM reports ORDER BY id");
        while(oQ.next())
        {
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pReportList->addItem(oVariant.toString());
            vReports.push_back(stData);              //  In the container.
        }

        return true;
}

bool MainWindow::GetPlaceList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vPlaces.clear();        //  In case we have to recall???
        QSqlQuery oQ("SELECT place_id , name FROM place ORDER BY place_id");
        while(oQ.next())
        {
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pReportList->addItem(oVariant.toString());
            vPlaces.push_back(stData);              //  In the container.
        }

        return true;
}

bool MainWindow::GetIdeasList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vPlaces.clear();        //  In case we have to recall???
        QSqlQuery oQ("SELECT place_id , name FROM ideas ORDER BY ideas_id");
        while(oQ.next())
        {
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pReportList->addItem(oVariant.toString());
            vIdeas.push_back(stData);              //  In the container.
        }

        return true;
}

std::string MainWindow::ReturnTypeName(int iTemp , std::vector<stTypeData> & vType)
{
        for(auto & it : vType)
        {
            if( it.iKey == iTemp)
                return it.sKeyString;
        }

        return "";
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

bool MainWindow::InsertDivider()
{

        return true;
}

bool MainWindow::CreateReportWindow(QTextDocument & oTD , std::string sName)
{
        ReportWindow  *  pRW = new ReportWindow(&oTD , sName , this);
        pRW->OnCreate();
        pRW->showMaximized();

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

//  Okay, let's cancel and clear everything.
void MainWindow::CancelButton()
{
        pSelectedStory->clear();
        pSelectedReport->clear();
        iSelectedStory  = 0;
        iSelectedReport = 0;
        pStoryList->setCurrentRow(0);
        pReportList->setCurrentRow(0);

        return;;
}

//  Okay, let's do the report.
void MainWindow::ExecuteButton()
{
        if(iSelectedReport > 0)
        {
            switch(iSelectedReport)         //  Adding the 1 to offset listwidget index.
            {
                case    1:
                    FullCharacterReport();
                    break;
                case    2:
                    FullSceneReport();
                    break;
                case    3:
                    FullPlaceReport();
                    break;
                case    4:
                    FullIdeasReport();
                    break;
                case    5:
                    FullWorldsReport();
                    break;
                default:
                break;
            }
        }
        return;
}

//------------------------------------------------------------------------
//  Other handlers.
//------------------------------------------------------------------------
void MainWindow::StoryDoubleClicked(QListWidgetItem * pItem)
{
        sSelectedStory = pItem->text().toStdString();

        for(auto & it : vStorys)
            if(it.sKeyString == sSelectedStory)
                iSelectedStory = it.iKey;

        pSelectedStory->setText(sSelectedStory.c_str());

        return;
}

void MainWindow::ReportDoubleClicked(QListWidgetItem * pItem)
{
        sSelectedReport = pItem->text().toStdString();

        for(auto & it : vReports)
            if(it.sKeyString == sSelectedReport)
                iSelectedReport = it.iKey;

        pSelectedReport->setText(sSelectedReport.c_str());
        return;
}

//------------------------------------------------------------------------
//  Actual Reports
//------------------------------------------------------------------------
bool MainWindow::FullCharacterReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        QString sS(QString("SELECT DISTINCT  c.used_name ,"
                                " c.first_name ,"
                                " c.middle_name ,"
                                " c.last_name , "
                                " c.age ,"
                                " sex.name ,"
                                " c.physical_desc ,"
                                " c.family_name ,"
                                " c.job ,"
                                " c.description ,"
                                " c.skills ,"
                                " c.personality ,"
                                " c.life_intent ,"
                                " c.seen_as ,"
                                " c.history ,"
                                " sex_pref.name "
                                " FROM scene AS s "
                                " INNER JOIN scene_char_link AS scl ON s.scene_id = scl.scene_id "
                                " INNER JOIN character AS c ON c.char_id = scl.char_id"
                                " INNER JOIN scene as sc ON sc.scene_id = scl.scene_id"
                                " INNER JOIN sex ON sex.id = c.sex"
                                " INNER JOIN sex_pref ON sex_pref.id = c.sex_pref"
                                " WHERE sc.story_id = %1").arg(iSelectedStory));
POUT(sS.toStdString());
        QSqlQuery oQ(sS);
        sBuild.clear();

        while(oQ.next()) // && iC < 2)
        {

            sBuild +=   QString("<b>" + oQ.value("used_name").toString() + "</b>    Age = ");
            sBuild +=   oQ.value("age").toString() + "   Sex = ";
            sBuild +=   oQ.value("sex.name").toString() + "     Sexual Pref = ";
            sBuild +=   oQ.value("sex_pref.name").toString() + "      Full Name = ";
            sBuild +=   oQ.value("first_name").toString() + " ";
            sBuild +=   oQ.value("middle_name").toString() + " ";
            sBuild +=   oQ.value("last_name").toString() + "           Family Name =";
            sBuild +=   oQ.value("family_name").toString() + "     \n";
            sBuild +=   oQ.value("physical_desc").toString() + " \n";
            sBuild +=   oQ.value("description").toString() + " \n";
            sBuild +=   oQ.value("skills").toString() + " \n";
            sBuild +=   oQ.value("history").toString();
            sBuild.append("<br>=============================================================<br>");

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Characters");
        return true;            //  Report finished.
}

bool MainWindow::FullSceneReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = QString("SELECT s.name , "
                                " p.name , "
                                " s.description ,"
                                " s.purpose "
                                " FROM scene as s "
                                " INNER JOIN place as p ON p.place_id = s.place_id "
                                "WHERE s.story_id = %1")
                                .arg(std::to_string(iSelectedStory).c_str());

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

        while(oQ.next())
        {
            sBuild +=   QString("<b>" + oQ.value("name").toString() + "</b><br>");
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Conditions = " + oQ.value("conditions").toString() + "<br>";
            sBuild +=   "Purpose = " + oQ.value("purpose").toString() + "<br>";
            sBuild.append("<br>=============================================================<br>");

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Scenes");
        return true;
}

bool MainWindow::FullPlaceReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  p.name , "
                    " p.description , "
                    " p.history , "
                    " pt.name , "
                    " p.parent_id"
                    " FROM place as p "
                    " INNER JOIN place_types as pt ON p.place_type = pt.id"
                    " ORDER BY p.place_type";

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();
        GetPlaceList();

//  Build the Report String.
        while(oQ.next())
        {
            sBuild +=   "<b>" + oQ.value("name").toString() + "</b>  ";
            sBuild +=   "Place Type = " + oQ.value("place_types.name").toString() + "<br>";
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "History = " + oQ.value("history").toString() + "<br>";
            QString oT = ReturnTypeName(oQ.value("parent_id").toInt() , vPlaces).c_str();
            sBuild +=   "Parent Place = " + oT + "<br>";
            sBuild.append("<br>=============================================================<br>");

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Places");
        return true;
}

bool MainWindow::FullIdeasReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  i.name , "
                    " i.description , "
                    " i.origin_date , "
                    " i.type , "
                    " i.parent_id"
                    " FROM ideas as i "
                    " ORDER BY i.idea_id";

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();
        GetIdeasList();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b> " + oQ.value("name").toString() + " </b>  ";
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Type = " + oQ.value("history").toString() + "<br>";
            sBuild +=   "Origin Date = " + oQ.value("history").toString() + "<br>";
            QString oT = ReturnTypeName(oQ.value("parent_id").toInt() , vIdeas).c_str();
            sBuild +=   "Parent Idea = " + oT + "<br>";
             sBuild.append("<br>=============================================================<br>");

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Ideas");
        return true;
}

bool MainWindow::FullWorldsReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  w.name , "
                    " w.description , "
                    " w.origin_date , "
                    " w.special_things "
                    " FROM world as w "
                    " ORDER BY w.world_id";

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b> " + oQ.value("name").toString() + " </b>  ";
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Special Things = " + oQ.value("history").toString() + "<br>";
            sBuild +=   "Origin Date = " + oQ.value("history").toString() + "<br>";
            sBuild.append("<br>=============================================================<br>");

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Worlds");
        return true;
}
