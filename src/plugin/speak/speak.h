//
// Created by Jonas on 30.05.2015.
//

#ifndef CHATBOT_CPP_SPEAK_H
#define CHATBOT_CPP_SPEAK_H

//
// Created by Jonas on 21.04.2015.
//
#include "../../interface.h"
#include "../../plugin.h"
#include "../botplugin.h"

#include <cstdio>

using namespace std;

class SpeakPlugin : public Plugin {
    public:
    SpeakPlugin();

        virtual ~SpeakPlugin();

        void replace_chars(string& input);

        virtual bool onCommand(const string command, const vector<string> args, const string username);

    protected:

        FILE* female;
        FILE* male;
};

#endif //CHATBOT_CPP_SPEAK_H
