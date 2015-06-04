/*
 * =====================================================================================
 *
 *       Filename:  echo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 20:48:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "../../interface.h"
#include "../../plugin.h"
#include "../botplugin.h"


#ifndef  system_INC
#define  system_INC

class SystemPlugin : public Plugin
{
    public:
        SystemPlugin();
        virtual ~SystemPlugin();

        virtual bool onCommand( const string command, const vector<string> args, const string username);

        bool date();
        bool time();

        void pull();
        void make();

        void calls();

    protected:
        string m_username;
        vector<string> m_args;
};

#endif   /* ----- #ifndef system_INC  ----- */

