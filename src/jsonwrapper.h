/*
 * =====================================================================================
 *
 *       Filename:  jsonwrapper.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03.04.2015 16:50:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  jsonwrapper_INC
#define  jsonwrapper_INC

#include <string>
#include <json/json.h>

using namespace std;

/*
 * =====================================================================================
 *        Class:  Jsonwrapper
 *  Description:  
 * =====================================================================================
 */
class Jsonwrapper
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Jsonwrapper ();                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */
        string get_send_content();
        Json::Value get_content();
        bool get_description(int index, string& desc);
        bool get_cell( int index, int col, string& entry);
        bool get_cell( int index, int col, bool& entry);
        bool get_cell( int index, int col, double& entry);
        bool get_cell( int index, int col, int& entry);

        bool empty();
        int get_count();

        friend ostream& operator<<(ostream& os, Jsonwrapper& jw);

        /* ====================  MUTATORS      ======================================= */
        void set( const char* key, const char* value);
        void set( const char* key, Json::Value value);
        void set_entry( Json::Value value);
        void clear_count();

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        Json::Value     m_value;
        int             m_count;

}; /* -----  end of class Jsonwrapper  ----- */


#endif   /* ----- #ifndef jsonwrapper_INC  ----- */
