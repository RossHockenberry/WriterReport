#include "myinclude.h"
#include "mlocal.h"


//using namespace std;

MDataObject::MDataObject()
{
}

MDataObject::MDataObject(std::string & rTemp)
{
//    string sTString = rTemp;

//  Fill out the data structure.
//        stDatabase.pszConnInfo = NULL;
        stDatabase.pszConnInfo = rTemp.c_str();      //  Sets the initial config string to send the database.
//  Pull the database name from the string.

//  Query string for getting PRIMARY KEY - at least the first part of it.  We'll append the later part as needed.
        sQueryPrimaryKey.append("SELECT  kc.ordinal_position,kc.column_name FROM information_schema.table_constraints tc, ");
        sQueryPrimaryKey.append("information_schema.key_column_usage kc WHERE tc.constraint_type = 'PRIMARY KEY' ");

//        sDatabaseName = ("writers_database");        //  Okay, for right now I'm cheating.
        OpenDatabaseConnection();
}

MDataObject::~MDataObject()
{
//  Let the function decide if the database is open.
        CloseDatabaseConnection();
}

void MDataObject::OpenDatabaseConnection()
{
// Make a connection to the database
//    POUT(stDatabase.pszConnInfo);
        stDatabase.conn = PQconnectdb(stDatabase.pszConnInfo);
/* Check to see that the backend connection was successfully made */
        if (PQstatus(stDatabase.conn) != CONNECTION_OK)
        {
            PQfinish(stDatabase.conn);
            stDatabase.bOpenFlag = false;       //  Should still be false anyway.
            return;
        }

//  Set open flag true.
        stDatabase.bOpenFlag = true;

//  Get initial information.
        ReadDatabaseSchema();
        ClearDatabaseData();
        return;
}

void MDataObject::CloseDatabaseConnection()
{
        if(stDatabase.bOpenFlag)
        {
            PQfinish(stDatabase.conn);
            PQclear(stDatabase.res);
        }
       return;
}

//---------------------------------------------------------------------------
//  Read in the database table and field info.  Also get the primary keys.
///--------------------------------------------------------------------------
bool MDataObject::ReadDatabaseSchema()
{
    std::string szString , szTemp , sLocalTableName;
    int iTemp = 0;
    bool bViewFlag = false;         //  For telling tables from views.

//  Query for Number of tables.
        szString.clear();
        szString = "SELECT count(*) FROM information_schema.tables WHERE table_schema = 'public'";
        SendDatabaseQuery(szString.c_str());

//  Bail if error.
        if(CheckDatabaseError())
            return false;

        stDataSchema.iNumberTables = GetCount();

//  Get the Table names.
        szString.clear();
        szString = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'";
        SendDatabaseQuery(szString.c_str());
        for(iIndex = 0; iIndex < stDataSchema.iNumberTables ; iIndex++)
        {
            stDataSchema.vTableSchema.push_back(stTableSchemaBase());       //  Push a new struct on the vector with default constructor.
                                                                            //  resize will allocate the vector but apparntly not correctly.
            szString = PQgetvalue(stDatabase.res,iIndex,0);                 //  Get the Table Name.
            stDataSchema.vTableSchema[iIndex].sTableName = szString;        //  Set name in vector of structs.
        }

//  Okay, now let's get the information for the Tables starting with record count.
        for(iIndex = 0 ; iIndex < stDataSchema.iNumberTables ; iIndex++)
        {
            std::string sTemp;      //  Just for this scope.
            sTemp = "Select count(*) FROM " + stDataSchema.vTableSchema[iIndex].sTableName;
            SendDatabaseQuery(sTemp.c_str());
            stDataSchema.vTableSchema[iIndex].iRecords = GetCount();
        }

//-----------------------------------------------------
//  Okay, now let's read in the field names.

        szString.clear();
        for(iIndex = 0 ;  iIndex <  stDataSchema.iNumberTables ; iIndex++)
        {
//  Get the number of columns.  I know, I should be able to do this in one SQL statement.
            szString = "Select count(*) from information_schema.columns where table_name = '" + stDataSchema.vTableSchema[iIndex].sTableName + "'";
            SendDatabaseQuery(szString.c_str());
            iTemp = GetCount();
            stDataSchema.vTableSchema[iIndex].iColumns = iTemp - 1;

//  Get the column names.
            szString = "Select column_name from information_schema.columns where table_name = '" + stDataSchema.vTableSchema[iIndex].sTableName + "'";
            SendDatabaseQuery(szString.c_str());
            for(iIndex_2 = 0 ; iIndex_2 < iTemp ; iIndex_2++)
            {
                szString = PQgetvalue(stDatabase.res,iIndex_2,0);
                stDataSchema.vTableSchema[iIndex].vColumnName.push_back(szString);
            }
        }
//  Need to figure out what to do if there is no primary key.
//-----------------------------------------------------
//  Okay, now let's read in the primary keys.

        szString.clear();                                           //  Clear the string.
        for(iIndex = 0 ; iIndex < stDataSchema.iNumberTables ; iIndex++)
        {
            szString.clear();
            szTemp = sQueryPrimaryKey;                              //  Assemble first part of string.
            szString.append("and tc.table_name = '");
            sLocalTableName = stDataSchema.vTableSchema[iIndex].sTableName;     //  Copy to a local string to work with.
            szString.append(sLocalTableName);                                   //  Use copy.
            if(sLocalTableName.find("view") == 0)
                bViewFlag = true;                                               //  Temporary solution at best.
//POUT(stDataSchema.vTableSchema[iIndex].sTableName);
            szString.append("' and kc.table_name = '");
            szString.append(sLocalTableName);                                   //  Use the copy again.
            szString.append("'");
            szTemp.append(szString);

            if(!bViewFlag)
            {
                SendDatabaseQuery(szTemp.c_str());

                if((iTemp = GetCount()) > 0)
                {
                         Q_UNUSED(iTemp);
                    //  Primary key column position.
                    szString = PQgetvalue(stDatabase.res,0,0);
                    stDataSchema.vTableSchema[iIndex].iPrimaryKey = stoi(szString) - 1;         //  Sub 1 to get column position.
                    szString = PQgetvalue(stDatabase.res,0,1);
                    stDataSchema.vTableSchema[iIndex].sPrimaryKeyName = szString.c_str();       //  Primary Key field name.
                }
            }
            bViewFlag = true;
        }
        return true;
}

void MDataObject::ClearResultStructure(stResultData & rS)
{
        rS.iRowsReturned = -1;
        rS.iColumnsReturned = -1;
        rS.vRowNumber.clear();
        rS.vDirty.clear();
        rS.vColumns.clear();
        return;
}

int MDataObject::GetCount()
{
        if(PQresultStatus(stDatabase.res) != PGRES_TUPLES_OK)
            return -1;

        return atoi(PQgetvalue(stDatabase.res,0,0));

}

int MDataObject::GetRowCount()
{
        if(PQresultStatus(stDatabase.res) != PGRES_TUPLES_OK)
        {
            return -1;  //  Error.
        }
        return std::stoi(PQcmdTuples(stDatabase.res));
}

int MDataObject::GetColumnCount()
{
         if (PQresultStatus(stDatabase.res) != PGRES_TUPLES_OK)
         {
             return -1;  //  Error.
         }
         return PQnfields(stDatabase.res);
}

bool MDataObject::ClearDatabaseData()
{
//  Nothing to do here yet.
        return true;
}

//-----------------------------------------------------
//  Read the data from a query.
//-----------------------------------------------------
bool MDataObject::ReadDatabaseData(stResultData & rResult)
{
    std::string szString;
    std::vector<std::string> vTemp;

        rResult.iColumnsReturned   = stDatabase.number_columns = GetColumnCount();
        rResult.iRowsReturned      = stDatabase.number_records = GetRowCount();

// Read the requested data.
        (rResult.vColumns).resize(stDatabase.number_records);                  // Should be able to avoid this.
        for(iIndex=0;iIndex < stDatabase.number_records;iIndex++)
            for(iIndex_2=0;iIndex_2 < stDatabase.number_columns;iIndex_2++)
            {
                szString = PQgetvalue(stDatabase.res,iIndex,iIndex_2);
                rResult.vColumns[iIndex].push_back(szString);                  //  Figure out a better way latter.
            }
        return true;
}

void MDataObject::GetDatabaseData(std::string &szString)
{
    Q_UNUSED(szString);

        return;
}
bool MDataObject::SendDatabaseQuery(const char * pString)
{
        stDatabase.res = PQexec(stDatabase.conn,pString);
        if(CheckDatabaseError())
        {
            return false;
        }

//  !!!!!!!!!!!!!!   Need to reget records count for table if update or delete.
        return true;
}

bool MDataObject::SendDatabaseQuery(const std::string & rS)
{
    stDatabase.res = PQexec(stDatabase.conn,rS.c_str());
    if(CheckDatabaseError())
    {
        return false;
    }
    return true;
}

bool MDataObject::CheckDatabaseError()
{
        sErrorString.clear();                   //  Clear the previous error.
        stDatabase.pgStatus = PQresultStatus(stDatabase.res);
        char * pError = PQresultErrorMessage(stDatabase.res);
        sErrorString = pError;
        if(strlen(pError) != 0   || stDatabase.pgStatus == PGRES_FATAL_ERROR)
        {
//            SimpleMessageBox smb;
//            smb.ErrorBox(sErrorString.c_str());
            return true;
        }

        return false;
}

void MDataObject::ReturnErrorString(std::string & rArg)
{
        rArg = sErrorString;
        return;
}

//  Get the schema data from database.
int MDataObject::GetNumberTables()
{
        return stDataSchema.iNumberTables;
}

void MDataObject::ClearResult()
{
         PQclear(stDatabase.res);
        return;
}

//----------------------------------------------------------------------------------------------------
//  Member functions that return database data for use in controls etc.
//
//----------------------------------------------------------------------------------------------------
//  Get and return query results.
bool MDataObject::ReturnQueryData(std::string& rQuery,stResultData& rResult)
{
        assert(!rQuery.empty());

        SendDatabaseQuery(rQuery.c_str());
        ClearResultStructure(rResult);
        ReadDatabaseData(rResult);
        return false;                   //  No or bad results.
}

bool MDataObject::ReturnQueryData(const std::string & rQuery, std::vector<stTypeData> & vQuery)
{
    int iIndex;
    stTypeData stType;
    stResultData stResult;

        SendDatabaseQuery(rQuery.c_str());
        ReadDatabaseData(stResult);

        if(!CheckDatabaseError())
        {
//            for(iIndex = 0 ; iIndex < GetRowCount() ; iIndex++)
            for(iIndex = 0 ; iIndex < stResult.iRowsReturned ; iIndex++)
            {
                stType.iKey = atoi(PQgetvalue(stDatabase.res,iIndex,0));
                stType.sKeyString = PQgetvalue(stDatabase.res,iIndex,1);
                vQuery.push_back(stType);
            }
        }
        else
        {

        }

        return false;
}

bool MDataObject::ReturnQueryData(const std::string & rQuery, std::vector<std::pair<int,int> > & vQuery)
{
    int iIndex;
    stTypeData stType;
    stResultData stResult;

        SendDatabaseQuery(rQuery.c_str());
        ReadDatabaseData(stResult);

        if(!CheckDatabaseError())
        {
            for(iIndex = 0 ; iIndex < GetRowCount() ; iIndex++)
            {
                vQuery.push_back(std::make_pair(0,0));
                vQuery[iIndex].first   = atoi(PQgetvalue(stDatabase.res,iIndex,0));
                vQuery[iIndex].second  = atoi(PQgetvalue(stDatabase.res,iIndex,1));
            }
        }

        return false;
}

//  Used to get the primary key of the last record written.
int MDataObject::ReturnLastRecordKey(const std::string & sTableName,const std::string & sColumnName)
{
    std::string sQ;
    stResultData stResult;

        sQ = "SELECT MAX(" + sColumnName + ") FROM " + sTableName;
        SendDatabaseQuery(sQ.c_str());
        if(!CheckDatabaseError())
        {
            return atoi(PQgetvalue(stDatabase.res,0,0));
        }
        return -1;      //  Error or not found.
}

//-----------------------------------------------------------------------------------------------
int MDataObject::ReturnSumQuery(const std::string & rQuery)
{
    stResultData stResult;

        SendDatabaseQuery(rQuery.c_str());
        ReadDatabaseData(stResult);

        if(!CheckDatabaseError())
            return atoi(PQgetvalue(stDatabase.res,0,0));

        return -1;
}

//-----------------------------------------------------------------------------------------------

//  Table Names.
bool MDataObject::ReturnTableNames(QStringList& rqList)
{
        for(iIndex = 0 ; iIndex < stDataSchema.iNumberTables ; iIndex++)
        {
            rqList.append(stDataSchema.vTableSchema[iIndex].sTableName.c_str());
        }
        return true;        //  Default return - no errors.
}

//  Return column names for particular table.
bool MDataObject::ReturnColumnNames(std::string& rArg,QStringList& rqList)
{
    std::string  szTemp;

        for(iIndex = 0 ; iIndex <= stDataSchema.iNumberTables ; iIndex++)           //  Table Loop.
            if(stDataSchema.vTableSchema[iIndex].sTableName == rArg)                //  Name compare.
                for(iIndex_2 = 0 ; iIndex_2 <= stDataSchema.vTableSchema[iIndex].iColumns ; iIndex_2++)
                {                                                                   //  Load Column Names.
                    szTemp = stDataSchema.vTableSchema[iIndex].vColumnName[iIndex_2];
                    rqList.push_back(szTemp.c_str());
                }
        return true;        //  Default return - no errors.
}

int MDataObject::ReturnTableColumnCount(std::string& rArg)
{
        for(iIndex = 0 ; iIndex <= stDataSchema.iNumberTables ; iIndex++)           //  Table Loop.
            if(stDataSchema.vTableSchema[iIndex].sTableName == rArg)                //  Name compare.
                return stDataSchema.vTableSchema[iIndex].iColumns;
        return 0;
}

int MDataObject::ReturnTableRowCount(std::string &rArg)
{
        for(iIndex = 0 ; iIndex <= stDataSchema.iNumberTables ; iIndex++)           //  Table Loop.
            if(stDataSchema.vTableSchema[iIndex].sTableName == rArg)                //  Name compare.
                return stDataSchema.vTableSchema[iIndex].iRecords;

        return 0;
}

//  Return the PK (Primary Key) column for the included table.
int MDataObject::ReturnPrimaryKey(std::string & rArg)
{
        for(iIndex = 0 ; iIndex <= stDataSchema.iNumberTables ; iIndex++)           //  Table Loop.
            if(stDataSchema.vTableSchema[iIndex].sTableName == rArg)                //  Name compare.
                return stDataSchema.vTableSchema[iIndex].iPrimaryKey;

        return -1;                                                                  //  No key found. Can't be column 0.
}

int MDataObject::ChangeTableRowCount(std::string & rT, int iT)
{
    if(!(iT < -1) && !(iT > 1))
    {
        for(iIndex = 0 ; iIndex <= stDataSchema.iNumberTables ; iIndex++)           //  Table Loop.
            if(stDataSchema.vTableSchema[iIndex].sTableName == rT)                  //  Name compare.
                stDataSchema.vTableSchema[iIndex].iRecords +=iT;
    }
        return stDataSchema.vTableSchema[iIndex].iRecords;
}


