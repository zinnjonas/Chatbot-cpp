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
#include <json/json.h>

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
        Database ();                             /* constructor */
        ~Database ();                            /* destructor */

        /* ====================  ACCESSORS     ======================================= */
        string get_error();                     /* get last error */

        /* ====================  MUTATORS      ======================================= */
        bool open( string db);                  /* open a database */
        bool is_open();

        bool create_table( string command);

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:        
        /* ====================  DATA MEMBERS  ======================================= */
        sqlite3* m_db;
        bool     m_open;


}; /* -----  end of class Database  ----- */


#endif   /* ----- #ifndef database_INC  ----- */

