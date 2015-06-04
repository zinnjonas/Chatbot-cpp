/*
 * =====================================================================================
 *
 *       Filename:  echo.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 20:47:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "echo.h"

#include <iostream>

using namespace std;

PLUGIN(EchoPlugin, "Echo Plugin", "r:echo")

EchoPlugin::EchoPlugin()
{
    cout << "created: EchoPlugin" << endl;
}

EchoPlugin::~EchoPlugin()
{
    cout << "destrpyed: EchoPlugin" << endl;
}

bool EchoPlugin::onCommand( const string command, const vector<string> args, const string username)
{
    cout << command << ";" << username << endl;
    for( string arg : args)
        cout << "\t" << arg << endl;
    cout << endl;

    string msg = add_color( RED, command);

    for( string arg : args)
    {
        msg += " ";
        msg += arg;
    }

    send_msg(username, msg);

    return true;
}

