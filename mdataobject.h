#ifndef MDATAOBJECT_H
#define MDATAOBJECT_H

//  Class header will go here after I write it.
//  Ross Edward Hockenberry 03/04/2021

#include <iostream>
#include <vector>
#include <string>
#include "myinclude.h"
#include "libpq-fe.h"

//-----------------------------------------------------------------------------------------------------------
// Database data structures.
//-----------------------------------------------------------------------------------------------------------
    struct stTableSchemaBase
    {
        std::string                             sTableName;          //  Name of the table.
        std::vector<std::string>                vColumnName;         //  Names of the column.
        std::vector<int>                        vColumnDataType;     //  Field type of the column.
        int                                     iPrimaryKey;         //  Primay Key Column Position.
                                                                     //  For now we're assuming 1 column - need to change that.
        std::string                             sPrimaryKeyName;     //  Name of the PRIMARY KEY column
        int                                     iRecords;            //  Number of Records.  This will changee. Update?
        int                                     iColumns;            //  Number of Columns in Tables.
    };

    struct stDatabaseSchema
    {
        int                              iNumberTables;         //  Number of tables in database.
        std::vector<stTableSchemaBase>   vTableSchema;          //  It's possible I'm begining to like vectors.
    };

    struct stDB
    {
        const char  *           pszConnInfo;
        bool                    bOpenFlag   =   false;
        int                     number_records;
        int                     number_columns;
        PGconn      *           conn;
        PGresult    *           res;
        ExecStatusType          pgStatus;
    };

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

//----------------------------------------------------------------------------------------------------------
//  End of Database structures.
//----------------------------------------------------------------------------------------------------------

class MDataObject
{
public:
//  Structure to contain database info

//  Define Table Datatypes;
    const int     DATATYPE_INT        =      0x01;
    const int     DATATYPE_VARCHAR    =      0x02;
    const int     DATATYPE_TEXT       =      0x04;

    std::string  sQueryPrimaryKey;


    stDB                stDatabase;                 //  General database info.
    stDatabaseSchema    stDataSchema;               //  Holds the database info.
                                                    //  Table names, column names and type, primary keys.
//    stResultData        stResult;                 //  Used to pass back results.
    std::string         sErrorString;               //  String of the last error.

//  Store the broken down results of a query.

    const char  *conninfo;
    std::string sDatabaseName;      //  Just a place to keep the database name.
    int         iIndex;             //  Index counters.
    int         iIndex_2;
    bool        bDupFlag = false;   //  Someone tried to add a dup to a unique constraint index.

        MDataObject();              //  Default constructor.
        MDataObject(std::string &); //  The one we'll actually use.

        ~MDataObject();             //  Destructor.

    //  Function declarations.
        void OpenDatabaseConnection();
        void CloseDatabaseConnection();
        bool ReadDatabaseSchema();          //  Get the database internals. Just name of Tables for now.
        int  GetCount();                    //  For use when we do a count query.
        bool ReadDatabaseData(stResultData &);

        int  GetColumnCount();              //  Used to get row and column counts - non SQl calls.
        int  GetRowCount();

        inline bool IsOpen(){ return stDatabase.bOpenFlag;};            //  What it says.
        inline bool IsDupFlag() {  bDupFlag = false; return true; }     //  Yes, we reset.
        inline bool SetDupFlag() { return bDupFlag = true; }

        bool SendDatabaseQuery(const char *);
        bool SendDatabaseQuery(const std::string &);
        bool ClearDatabaseData();
        bool CheckDatabaseError();
        void GetDatabaseData(std::string &);
        void ClearResult();
        void ClearResultStructure(stResultData &);

//  Get, edit or add certain data.  Table names included.
        bool ReturnTableNames(QStringList&);
        int  ReturnTableColumnCount(std::string &);         //  Get the number of columns in the table.
        bool ReturnColumnNames(std::string& ,QStringList&);
        int  ReturnTableRowCount(std::string &);
        int  ReturnPrimaryKey(std::string &);               //  Return the primary key column for a table.
        bool ReturnQueryData(std::string&,stResultData&);   //  Return query data in reference structure.
        bool ReturnQueryData(const std::string&,std::vector<stTypeData>&);  //  Return query data in vector of struct stTypeData.
        bool ReturnQueryData(const std::string &,std::vector<std::pair<int,int> >&);      //  Just another data format.
        int  ReturnSumQuery(const std::string &);           //  Return just a sum value.
        void ReturnErrorString(std::string&);               //  Get the error string if error.
        int  ReturnLastRecordKey(const std::string & , const std::string &);    //  To get the primary key for the last added record.
        int  ChangeTableRowCount(std::string&,int);         //  Increment or decrement the rows count for a table.
                                                            //  Quicker than reloading schema.
        
//  Database schema routines.
        bool GetDatabaseSchema();
        int  GetNumberTables();
//        bool UpdateTableRowCount(std::string &);
};

#endif // MDATAOBJECT_H
