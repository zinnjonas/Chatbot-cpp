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
#include "system.h"

#include <iostream>

using namespace std;

PLUGIN(SystemPlugin, "System Plugin", "r:date r:time b:exit b:pull b:make r:call")

SystemPlugin::SystemPlugin()
{
    cout << "created: SystemPlugin" << endl;
}

SystemPlugin::~SystemPlugin()
{
    cout << "destroyed: SystemPlugin" << endl;
}

bool SystemPlugin::onCommand( const string command, const vector<string> args, const string username) {

    cout << "<" << command << ">" << endl;
    m_args = args;
    m_username = username;
    if (command.compare("date") == 0)
        return date();
    else if (command.compare("time") == 0)
        return time();
    else if( command.compare("exit") == 0)
        exit(0);
    else if( command.compare("pull") == 0)
        pull();
    else if( command.compare("make") == 0)
        make();
    else if( command.compare("call") == 0)
        calls();
    return false;
}

bool SystemPlugin::date()
{
    if( m_args.size() > 0)
    {
        send_msg( m_username, add_color( RED, "wrong number of args given"));
        return false;
    }

    FILE* in;
    string msg;
    char buffer[512];
    #ifdef __WIN32__
    if( !(in  = popen("date /T", "r")))
    #else
    if( !(in  = popen("date", "r")))
    #endif
        return false;

    while( fgets(buffer, sizeof(buffer), in) != NULL) {
        msg += buffer;
    }

    pclose(in);

    send_msg(m_username, msg);

    return true;
}

bool SystemPlugin::time()
{
    if( m_args.size() > 0)
    {
        send_msg( m_username, add_color( RED, "wrong number of args given"));
        return false;
    }

    FILE* in;
    string msg;
    char buffer[512];
#ifdef __WIN32__
    if( !(in  = popen("time /T", "r")))
#else
    if( !(in  = popen("time", "r")))
    #endif
        return false;

    while( fgets(buffer, sizeof(buffer), in) != NULL) {
        msg += buffer;
    }

    pclose(in);

    send_msg(m_username, msg);

    return true;
}

void SystemPlugin::pull()
{
    string msg = "";

    FILE* git = popen("git pull", "r");

    if( !git)
    {
        send_msg(m_username, add_color(RED, "Pipe to git went wrong"));
        return;
    }

    char tline[200];
    while( fgets(tline, 200, git) != NULL)
    {
        string line = tline;
        cout << line << endl;
        if( line.find("Already up-to-date.") != string::npos)
        {
            line.pop_back();
            msg = add_color(YELLOW, line);
            cout << msg << endl;
            say("Alles bereits auf dem neusten Stand.");

        }
        else
        {
            size_t plus = line.find('+');
            size_t minus = line.find('-');
            if (plus == string::npos && minus == string::npos)
                msg += line;
            else {
                size_t first;
                if (plus != string::npos) {
                    first = plus;
                }
                else {
                    first = minus;
                }
                string temp_line = line.substr(0, first);
                msg += temp_line;
                if (plus != string::npos) {
                    msg += add_color(GREEN, line.substr(plus, line.find_last_of('+') - plus));
                }

                if (plus != string::npos) {
                    msg += add_color(GREEN, line.substr(minus, line.find_last_of('-') - minus));
                }
            }
        }
    }

    pclose(git);

    send_msg(m_username, msg);
}

void SystemPlugin::make()
{
    string msg = "";

    FILE* build = popen("make -f Makefile -j4 -s", "r");

    if( !build)
    {
        send_msg(m_username, add_color(RED, "Pipe to git went wrong"));
        return;
    }

    char line[200];
    while( fgets(line, 200, build) != NULL)
        msg += line;

    pclose(build);

    send_msg(m_username, msg);
}

void SystemPlugin::calls()
{
    string command = m_args.front();
    m_args.erase(m_args.begin());
    call(command, m_args, m_username);
}