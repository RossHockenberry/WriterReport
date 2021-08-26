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
        OpenDatabase();
        return true;
}


bool MainWindow::InitWidgets()
{

        return true;
}

bool MainWindow::SetSlots()
{

        return true;
}

//-----------------------------------------------------------------------
//  Database support functions.
//-----------------------------------------------------------------------
bool MainWindow::OpenDatabase()
{
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

//-----------------------------------------------------------------------
//  Reports
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
