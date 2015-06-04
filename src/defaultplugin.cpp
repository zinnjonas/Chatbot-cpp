//
// Created by Jonas on 16.04.2015.
//

#include "defaultplugin.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

DefaultPlugin::DefaultPlugin() : Plugin(true)
{
    cout << "created: DefaultPlugin" << endl;
}

DefaultPlugin::~DefaultPlugin()
{
    cout << "destroyed: DefaultPlugin" << endl;
}

bool DefaultPlugin::onCommand(const string command, const vector<string> args, const string username)
{

    m_args = args;
    m_username = username;

    if( command.compare("clear") == 0)
        clear();
    else if( command.compare("help") == 0)
        help();
    else if( command.compare("users") == 0)
        users();
    else if( command.compare("restart") == 0)
        restart();

    return true;
}

void DefaultPlugin::clear()
{
    clear_all();
}

void DefaultPlugin::help()
{
    send_msg( m_username, get_list_command( m_username));
}

void DefaultPlugin::users()
{
    send_msg(m_username, get_users());
}

void DefaultPlugin::restart()
{
    char path[PATH_MAX];
    char dest[PATH_MAX];
    pid_t pid = getpid();
    sprintf(path, "/proc/%d/exe", pid);
    cout << path << endl;
    memset(dest, 0, sizeof *dest);
#ifndef _WIN32
    if (readlink(path, dest, PATH_MAX) == -1)
        perror("readlink");
#else

    strcpy( dest, garg);
#endif // _WIN32
    char* args[2];
    args[0] = dest;
    args[1] = NULL;

    if( execv(dest, args) == -1)
    {
        fprintf(stderr, "Failed to reexecute %s %d %s\n", dest, errno, strerror(errno));
    }
    exit(1);
}
