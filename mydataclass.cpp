/*
    Okay, this is my data interface to the database class.
    Ross Hockenberry
    08/22/2021
*/

#include "mydataclass.h"

MyDataClass::MyDataClass(std::string DB_String , QObject *parent) : QObject(parent)
{    


        return;
}

MyDataClass::~MyDataClass()
{

        return;
}

void MyDataClass::InitObject()
{

        return;
}

//  Set form passed string.
bool MyDataClass::SetDatabaseItems(std::string & rString)
{
        sDBName   =   rString;

        return true;
}

//  Set from settings - or right now in code.
bool MyDataClass::SetDatabaseItems()
{

        return true;
}

void MyDataClass::SetDatabaseInfo()
{

        return;
}

bool MyDataClass::OpenDatabase()
{

        return true;
}

bool MyDataClass::CloseDatabase()
{

        return true;
}


bool MyDataClass::GetDatabaseTables()
{

        return true;
}


