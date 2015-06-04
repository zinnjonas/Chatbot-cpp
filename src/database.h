/*
 * =====================================================================================
 *
 *       Filename:  database.h
 *
 *    Description: Conntrol of the SQLite3 interface
 *
 *        Version:  1.0
 *        Created:  01.04.2015 02:20:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  database_INC
#define  database_INC

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "jsonwrapper.h"

using namespace std;

/*
 * =====================================================================================
 *        Class:  Database
 *  Description:  
 * =====================================================================================
 */
class Database
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Database (string sql_folder);            /* constructor */
        ~Database ();                            /* destructor */

        /* ====================  ACCESSORS     ======================================= */
        string get_error();                     /* get last error */
        void get_sql_from_file( string name, string& content);

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */
        bool open( string db);                  /* open a database */
        bool exists( string table, bool create = true);
        bool create( string table);
        bool select_all( string table, Json::Value& data);
        bool insert( string table, Json::Value values);

        bool is_error( int rc);

    protected:
        /* ====================  DATA MEMBERS  ======================================= */
        static int callback(void* data, int argc, char **argv, char **azColName);
        static int json_callback( void* data, int argc, char** argv, char** azColName);

    private:        
        /* ====================  DATA MEMBERS  ======================================= */
        sqlite3* m_db;
        bool     m_open;
        char*    m_zErrMsg;
        string   m_sql_folder;


}; /* -----  end of class Database  ----- */


#endif   /* ----- #ifndef database_INC  ----- */

