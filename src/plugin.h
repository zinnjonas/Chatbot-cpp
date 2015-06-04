/*
 * =====================================================================================
 *
 *       Filename:  plugin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 20:33:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  plugin_INC
#define  plugin_INC


#include <string>
#include <dlfcn.h>
#include <vector>

#include "user.h"

using namespace std;

/*
 * =====================================================================================
 *        Class:  Plugin
 *  Description:  Interface for the plugins
 * =====================================================================================
 */
class Plugin
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Plugin (bool intern = false) { m_intern =intern;m_handle = nullptr;}                       /* constructor */
        virtual ~Plugin(){}

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */
        virtual bool onCommand( const string command, const vector<string> arguments, const string username) = 0;

        virtual void set_handle( void* handle){ m_handle = handle;}
        virtual void* get_handle() { return m_handle;}
        bool get_intern() { return m_intern;}

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        void* m_handle;
        bool m_intern;

}; /* -----  end of class Plugin  ----- */


#endif   /* ----- #ifndef plugin_INC  ----- */

