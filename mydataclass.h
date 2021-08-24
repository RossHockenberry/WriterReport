#ifndef MYDATACLASS_H
#define MYDATACLASS_H

#include "myinclude.h"
#include "myflag.h"

//  Data structures used everywhere.
struct stResultData
 {
     int                                      iRowsReturned;      //  Number of rows and columns returned.
     int                                      iColumnsReturned;   //  Need to find out what these are if error.
     std::vector<int>                         vRowNumber;         //  Row of the corresponding column data.
     std::vector<bool>                        vDirty;             //  True if it needs to be rewritten.
     std::vector<std::vector<std::string>>    vColumns;           //  Actual column content.
 };

 typedef std::vector<std::pair<int,std::string>> vResultQuery;

 struct stTypeData
 {
     int             iKey;
     std::string     sKeyString;
     int             iPosKey;            //  Position in table or subset of table.
 };

 struct stColumnData
 {
     std::string                 sColumnName;
     int                         iDataType;
 };

 struct stTableData
 {
     std::string                 sTableName;
     int                         iNumberColumns;
     int                         iPrimaryKey;    //  PK of the table.  Assuming single column or now.
     std::vector<stColumnData>   vColumnData;    //  Column data.

 };

//----------------------------------------------------------------------------------
//  Class begins.
class MyDataClass : public QObject
{
    Q_OBJECT
public:
    explicit MyDataClass(std::string DB_String = nullptr , QObject *parent = nullptr);
    virtual ~MyDataClass();

//  Object Functions.
    void            InitObject();
    bool            SetDatabaseItems();
    bool            OnCreate();                 //  Where all the init stuff gets called.
//  Open, Close and Info functions.
    void            SetDatabaseInfo();      //  Set connection string stuff.
    bool            OpenDatabase();
    bool            CloseDatabase();
    bool            GetDatabaseTables();
    int             NumberDatabaseTables() {return iNumberTables; }
    bool            IsDatabaseOpen() { return oOpenFlag.Is(); }

//  Handle queries.
//  Might use a signal to say if someone changed something in database.
signals:

private:
//  Qt Database stuff.

//    QSql            oSql;
    QSqlDatabase    *   pDatabase;
    QSqlQuery       *   pQuery;
    QSqlError       *   pError;
    QSqlField       *   pField;
    QSqlRecord      *   pRecord;

//    QSqlResult      oResult;

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
    myflag          oOpenFlag;
    int             iNumberTables = -1;         //  -1 means not open yet.


};

#endif // MYDATACLASS_H
