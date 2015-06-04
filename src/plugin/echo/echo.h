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


#ifndef  echo_INC
#define  echo_INC

class EchoPlugin : public Plugin
{
    public:
        EchoPlugin();
        virtual ~EchoPlugin();

        virtual bool onCommand( const string command, const vector<string> args, const string username); 
};

#endif   /* ----- #ifndef echo_INC  ----- */

