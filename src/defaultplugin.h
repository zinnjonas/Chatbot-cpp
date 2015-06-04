//
// Created by Jonas on 16.04.2015.
//

#ifndef CHATBOT_CPP_DEFAULTPLUGIN_H
#define CHATBOT_CPP_DEFAULTPLUGIN_H

#include <string>

#include "plugin.h"

using namespace std;


const string defaultPluginInfo = "b:clear b:users r:help b:restart";

extern void send_msg( const string username, const string msg);
extern void clear_all();
extern string get_list_command( string user);
extern string get_users();
#ifdef _WIN32
extern char garg[MAX_PATH];
#endif //_WIN32

class DefaultPlugin : public Plugin{

    public:

        DefaultPlugin();
        virtual ~DefaultPlugin();

        virtual bool onCommand( const string command, const vector<string> args, const string username);

        void clear();
        void help();
        void users();
        void restart();

    protected:

        vector<string> m_args;
        string m_username;

};


#endif //CHATBOT_CPP_DEFAULTPLUGIN_H
