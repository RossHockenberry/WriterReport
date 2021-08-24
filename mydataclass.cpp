/*
    Okay, this is my data interface to the database class.
    Ross Hockenberry
    08/22/2021
*/
/*
    Supported driver names.
    QODBC       -   ODBC driver
    QPSQL       -   PostgreSQL
    QSQLITE     -   SQLite
    QMYSQL      -   MySQL
    QMARIADB    -   MariaDB
*/

#include "myinclude.h"
#include "mlocal.h"

MyDataClass::MyDataClass(std::string DB_String , QObject *parent) : QObject(parent)
{
        sDBConnection = DB_String;
        return;
}

MyDataClass::~MyDataClass()
{

        return;
}

bool MyDataClass::OnCreate()
{
        InitObject();
        SetDatabaseItems();
        OpenDatabase();

        return true;
}

void MyDataClass::InitObject()
{
//  Init all the database objects.
        pDatabase   =   new QSqlDatabase();
        pQuery      =   new QSqlQuery();
        pError      =   new QSqlError();
        pField      =   new QSqlField();
        pRecord     =   new QSqlRecord();
        return;
}

//  Set from settings - or right now in code.
bool MyDataClass::SetDatabaseItems()
{
//  For now we'll just set things manually.
        sDBName         =   "writers_database";
        sDBHost         =   "marai";
        sDBAddress      =   "192.168.1.3";
        sDBPort         =   "5432";
        sDBUser         =   "pi";
        sDBPassword     =   "pi";
        sDBDriver       =   "QPSQL";

         return true;
}

bool MyDataClass::OpenDatabase()
{
    QSqlDatabase oDb = *pDatabase;

        oDb   =   QSqlDatabase::addDatabase(sDBDriver.c_str());  // Set the database driver.
        oDb.setHostName(sDBAddress.c_str());    //  Use the ip for now.
        oDb.setDatabaseName(sDBName.c_str());
        oDb.setPort(atoi(sDBPort.c_str()));
        oDb.setUserName(sDBUser.c_str());
        oDb.setPassword(sDBPassword.c_str());

        oOpenFlag.Set(oDb.open());
        return true;
}

bool MyDataClass::CloseDatabase()
{
        if(oOpenFlag.Is())
            pDatabase->close();
        return true;
}

void MyDataClass::SetDatabaseInfo()
{

        return;
}

bool MyDataClass::GetDatabaseTables()
{

        return true;
}


