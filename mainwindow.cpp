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
        resize(1600,1000);    //  Just to work with for now.
        return;
}

MainWindow::~MainWindow()
{
        return;
}

void MainWindow::GetScreenData()
{
    QString sT;
    QSize   oSize;

        QRect oRect = this->screen()->availableGeometry();

        iScreenHeight       = oRect.height();
        iScreenWidth        = oRect.width();

        oSize = size();
        sT = QString("Available Height = %1 Width %2 --- Current Size = %3 %4").arg(oRect.height()).arg(oRect.width())
                    .arg(oSize.width()).arg(oSize.height());
        statusBar()->showMessage(sT);

        return;
}

bool MainWindow::LoadSettings()
{
//    QString sKey("EditorFont");

        pSettings->sync();
        *pKeyList = pSettings->allKeys();

        sDBName     = pSettings->value("DBName").toString().toStdString();
        sDBHost     = pSettings->value("DBHost").toString().toStdString();
        sDBAddress  = pSettings->value("DBAddress").toString().toStdString();
        sDBPort     = pSettings->value("DBPort").toString().toStdString();
        sDBUser     = pSettings->value("DBUser").toString().toStdString();
        sDBPassword = pSettings->value("DBPassword").toString().toStdString();

//        oEditorFont = pSettings->value(sKey).value<QFont>();
//        bFullScreenFlag = pSettings->value("FullScreen").toBool();

        return true;
}

bool MainWindow::OnCreate()
{
        InitObject();
        return true;
}

bool MainWindow::InitObject()
{
        pSettings   =   new QSettings(this);
        pKeyList    =   new QStringList();

        pStatusBar = statusBar();   //  Create a status bar.
        UpdateStatus("Start Initialization");
        LoadSettings();             //  Must be written by WriterData first.
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

        pSelectedTypeLabel    =   new QLabel("Selected Item");
        pSelectedType         =   new QLabel(" ");
        pSelectedType->setVisible(false);
        pSelectedType->setFrameStyle(QFrame::Panel |  QFrame::Sunken);
        pSelectedType->setLineWidth(2);   //  Add some depth to our sunken lables.
        pSelectedType->setVisible(false);

//  Create Listbox.
        pReportList         =   new QListWidget();
        pStoryList          =   new QListWidget();
        pTypeList           =   new QListWidget();

        return true;
}

bool MainWindow::TypeListHandler()
{
        switch(iSelectedReport)
        {
            case    1:
                HandleCharList();
                break;
            case    2:
                HandleSceneList();
                break;

                default:
                    break;
            }

        return true;
}

bool MainWindow::HandleCharList()
{
        if(oCharacterFlag.Flip())
        {
            pTypeList->setVisible(true);
            FillCharacterList();
        }
        else
        {
            pReportListLayout->invalidate();
            pTypeList->setVisible(false);
        }
        return true;
}

bool MainWindow::HandleSceneList()
{
        if(oCharacterFlag.Flip())
        {
            pTypeList->setVisible(true);
            FillCharacterList();
        }
        else
        {
            pReportListLayout->invalidate();
            pTypeList->setVisible(false);
        }
        return true;
}
bool MainWindow::HandleTypeList()
{
        if(oCharacterFlag.Flip())
        {
            pTypeList->setVisible(true);
            FillCharacterList();
        }
        else
        {
            pReportListLayout->invalidate();
            pTypeList->setVisible(false);
        }
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
        pReportListLayout->addWidget(pTypeList);
        pTypeList->setVisible(false);

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
        connect(pTypeList , &QListWidget::itemDoubleClicked , this , &MainWindow::TypeDoubleClicked);
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

void MainWindow::FillTypesList()
{

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

bool MainWindow::FillCharacterList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vCharacters.clear();
        QSqlQuery oQ("SELECT char_id , used_name FROM character ORDER BY used_name");
        while(oQ.next())
        {
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pTypeList->addItem(oVariant.toString());
            vCharacters.push_back(stData);              //  In the container.
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

bool MainWindow::GetSagasList()
{
    QVariant    oVariant;
    stTypeData  stData;

        vPlaces.clear();        //  In case we have to recall???
        QSqlQuery oQ("SELECT saga_id , name FROM saga ORDER BY saga_id");
        while(oQ.next())
        {
            stData.iPosKey = 0;
            oVariant = oQ.value(0);
            stData.iKey = oVariant.toInt();
            oVariant = oQ.value(1);
            stData.sKeyString = oVariant.toString().toStdString();
            pReportList->addItem(oVariant.toString());
            vSagas.push_back(stData);              //  In the container.
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
/*        sDBName         =   "writers_database";
        sDBHost         =   "marai";
        sDBAddress      =   "192.168.1.3";
        sDBPort         =   "5432";
        sDBUser         =   "pi";
        sDBPassword     =   "pi";*/
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

bool MainWindow::CreateReportWindow(QTextDocument & oTD , std::string sName)
{
        ReportWindow  *  pRW = new ReportWindow(sName , &oTD , this);
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
        pSelectedType->clear();
        iSelectedStory      = 0;
        iSelectedReport     = 0;
        iSelectedScene      = 0;
        iSelectedCharacter  = 0;
        pStoryList->setCurrentRow(0);
        pReportList->setCurrentRow(0);

        return;;
}

//  Okay, let's do the report.
//  Going to scrunch this case statement a little.
void MainWindow::ExecuteButton()
{
        if(iSelectedReport > 0)
        {
            switch(iSelectedReport)
            {
                case    1:
                    FullCharacterReport(iSelectedStory , iSelectedCharacter);
                    break;
                case    2:
                    FullSceneReport(iSelectedStory);
                    break;
                case    3:  FullPlaceReport();          break;
                case    4:  FullIdeasReport();          break;
                case    5:  FullWorldsReport();         break;
                case    6:  FullStoriesReport();        break;
                case    7:  FullSagasReport();          break;
                case    8:  FullThingsReport();         break;
                case    9:  FullDBHistoryReport();      break;
                case    10: SingleCharacterReport();    break;
                case    11: SingleSceneReport();        break;
                case    12: SingleTypeReport();         break;
                case    13: FullTypesReport();          break;
                case    14: CharactersInSceneReport();  break;
                case    15: CharactersInSceneReport();  break;
                case    16: SingleStoryReport();        break;

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
    std::string st;

        sSelectedReport = pItem->text().toStdString();

        for(auto & it: vReports)
            if(it.sKeyString == sSelectedReport)
            {
                iSelectedReport = it.iKey;
                st = sSelectedReport;
            }
        pSelectedReport->setText(sSelectedReport.c_str());

        TypeListHandler();
        return;
}

void MainWindow::TypeDoubleClicked(QListWidgetItem * pItem)
{
        sSelectedType = pItem->text().toStdString();

        for(auto & it : vReports)
            if(it.sKeyString == sSelectedType)
            {
                if(oCharacterFlag.Is())
                    iSelectedCharacter = it.iKey;
                if(oSceneFlag.Is())
                    iSelectedScene = it.iKey;
            }
        pSelectedType->setText(sSelectedType.c_str());
        return;
}

//------------------------------------------------------------------------
//  Actual Reports
//------------------------------------------------------------------------
bool MainWindow::FullCharacterReport(int iStory , int iChar)
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        QString oS(QString("SELECT DISTINCT  c.used_name ,"
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
                                " c.origin_date , "
                                " sex_pref.name "
                                " FROM scene AS s "
                                " INNER JOIN scene_char_link AS scl ON s.scene_id = scl.scene_id "
                                " INNER JOIN character AS c ON c.char_id = scl.char_id"
                                " INNER JOIN scene as sc ON sc.scene_id = scl.scene_id"
                                " INNER JOIN sex ON sex.id = c.sex"
                                " INNER JOIN sex_pref ON sex_pref.id = c.sex_pref"));
        if(iStory > 0)
        {
            oS += QString(" WHERE sc.story_id = %1").arg(iStory);
            if(iChar > 0)
                oS += QString(" AND c.char_id = %1").arg(iChar);
        }

POUT(oS.toStdString());
        QSqlQuery oQ(oS);
        sBuild.clear();

        while(oQ.next()) // && iC < 2)
        {

            sBuild +=   QString("<b> " + oQ.value("used_name").toString() + " </b>    Age = ");
            sBuild +=   oQ.value("age").toString() + "   Sex = ";
            sBuild +=   oQ.value("sex.name").toString() + "     Sexual Pref = ";
            sBuild +=   oQ.value("sex_pref.name").toString() + "      Full Name = ";
            sBuild +=   oQ.value("first_name").toString() + " ";
            sBuild +=   oQ.value("middle_name").toString() + " ";
            sBuild +=   oQ.value("last_name").toString() + "           Family Name = ";
            sBuild +=   oQ.value("family_name").toString() + "<br>";
            sBuild +=   oQ.value("physical_desc").toString() + "<br>";
            sBuild +=   oQ.value("description").toString() + "<br>";
            sBuild +=   oQ.value("skills").toString() + "<br>";
            sBuild +=   oQ.value("history").toString() + "<br>";
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Characters");
        return true;            //  Report finished.
}

bool MainWindow::FullSceneReport(int iStory)
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = QString("SELECT s.name , "
                                " p.name , "
                                " s.description ,"
                                " conditions ,"
                                " s.purpose ,"
                                " s.origin_date "
                                " FROM scene as s "
                                " INNER JOIN place as p ON p.place_id = s.place_id ");

        if(iStory > 0)
            sBuild +=   QString(" WHERE s.story_id = %1 ")
                                .arg(std::to_string(iStory).c_str());

        sBuild += "ORDER BY s.sequence ";


POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

        while(oQ.next())
        {
            sBuild +=   QString("<b>" + oQ.value("name").toString() + "</b><br>");
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Conditions = " + oQ.value("conditions").toString() + "<br>";
            sBuild +=   "Purpose = " + oQ.value("purpose").toString() + "<br>";
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        sBuild.clear();
        sBuild = "Scenes";
        if(iStory)
            sBuild += QString(" for Story %1").arg(sSelectedStory.c_str());
        CreateReportWindow(oReport , sBuild.toStdString());
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
                    " p.origin_date ,"
                    " p.parent_id "
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
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

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
                    " i.origin_date"
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
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
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
    QDateTime   oD;

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
            sBuild +=   "<b>" + oQ.value("name").toString();
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Special Things = " + oQ.value("special_things").toString() + "<br>";
            sBuild +=   "Origin Date = ";
            oD = oQ.value("origin_date").toDateTime();
            sBuild +=   oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Worlds");
        return true;
}

bool MainWindow::FullStoriesReport(int iStory)
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  s.name , "
                    " s.description , "
                    " s.origin_date , "
                    " s.outline , "
                    " s.saga_id ,"
                    " s.word_count "
                    " FROM story as s "
                    " ORDER BY s.story_id";

        if(iStory > 0)
            sBuild += QString(" WHERE sc.story_id = %1").arg(iStory);

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b> " + oQ.value("name").toString() + " </b>  ";
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Outline = " + oQ.value("outline").toString() + "<br>";
            QString     oT = ReturnTypeName(oQ.value("saga_id").toInt() , vSagas).c_str();
            sBuild +=   "Outline = " + oT + "<br>";
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Stories");
        return true;
}

bool MainWindow::FullSagasReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  s.name , "
                    " s.description , "
                    " s.origin_date , "
                    " s.history  "
                    " FROM saga as s "
                    " ORDER BY s.saga_id";

        QSqlQuery oQ(sBuild);
        sBuild.clear();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b> " + oQ.value("name").toString() + " </b> <br> ";
            sBuild +=   "Description = " + oQ.value("description").toString() + " <br>";
            sBuild +=   "History = " + oQ.value("history").toString() + "<br>";
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Sagas");
        return true;
}

bool MainWindow::FullThingsReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  t.name , "
                    " t.description , "
                    " t.origin_date , "
                    " t.purpose  "
                    " FROM thing as t "
                    " ORDER BY t.thing_id";

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b>" + oQ.value("name").toString() + " </b>  <br>";
            sBuild +=   "Description = " + oQ.value("description").toString() + "<br>";
            sBuild +=   "Purpose = " + oQ.value("purpose").toString() + "<br>";
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Things");
        return true;
}

bool MainWindow::FullDBHistoryReport()
{
    QString sBuild;
    QTextDocument   oReport;
    QTextCursor oCursor(&oReport);

        sBuild = "SELECT  d.version , "
                    " d.description , "
                    " d.origin_date  "
                    " FROM db_history as d "
                    " ORDER BY d.id";

POUT(sBuild.toStdString());
        QSqlQuery oQ(sBuild);
        sBuild.clear();

//  Build the report string.
        while(oQ.next())
        {
            sBuild +=   "<b> " + oQ.value("version").toString() + " </b>  <br>";
            sBuild +=   "Description = " + oQ.value("description").toString();
            QDateTime   oD = oQ.value("origin_date").toDateTime();
            sBuild +=   "Origin Date = " + oD.toString() + "<br>";
            sBuild +=   "<br>" + oSeperator + "<br>";

            oCursor.insertText(sBuild);
            sBuild.clear();
        }

        CreateReportWindow(oReport , "Wrter Application History");
        return true;
}

bool MainWindow::CharactersInSceneReport()
{

        return true;
}

bool MainWindow::CharactersInGroupReport()
{

        return true;
}

bool MainWindow::SingleTypeReport()
{

        return true;
}

bool MainWindow::FullTypesReport()
{

        return true;
}

bool MainWindow::SingleCharacterReport()
{

        return true;
}

bool MainWindow::SingleSceneReport()
{

        return true;
}

bool MainWindow::SingleStoryReport()
{

        return true;
}




