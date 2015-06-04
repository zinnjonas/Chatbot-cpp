//
// Created by Jonas on 31.05.2015.
//

#ifndef CHATBOT_CPP_CLOCK_H
#define CHATBOT_CPP_CLOCK_H


#include "../../interface.h"
#include "../../plugin.h"
#include "../botplugin.h"

class ClockPlugin : public Plugin
{
public:
    ClockPlugin();
    virtual ~ClockPlugin();

    virtual bool onCommand( const string command, const vector<string> args, const string username);

    vector<string> split(string sentence);

    string weather();
    string get_desc(int id);
};

#endif //CHATBOT_CPP_CLOCK_H
