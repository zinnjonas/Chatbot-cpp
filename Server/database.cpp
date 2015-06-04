/*
 * =====================================================================================
 *
 *       Filename:  database.cpp
 *
 *    Description:  Implementation of the SQLite3 interface
 *
 *        Version:  1.0
 *        Created:  01.04.2015 02:20:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "database.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

Database::Database()
{
    m_open = false;
}

Database::~Database()
{
    if( m_open)
        sqlite3_close(m_db);
}

string Database::get_error()
{
    return sqlite3_errmsg(m_db);
}

bool Database::open( string db)
{

    size_t pos = db.find(".db");
    if( pos == string::npos)
        db += ".db";

    if( sqlite3_open(db.c_str(), &m_db))
    {
        return false;
    }
    
    m_open = true;

    return true;
}

bool Database::is_open()
{
    return m_open;
}

bool Database::create_table(string command)
{

    if( !m_open)
        return false;
    char* zErrMsg = 0;
    int rc = sqlite3_exec( m_db, command.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK)
    {
        sqlite3_free(zErrMsg);
    }
    else
        cout << "Table created successfully " << endl;

    return true;
}

