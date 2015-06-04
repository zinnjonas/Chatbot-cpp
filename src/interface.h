/*
 * =====================================================================================
 *
 *       Filename:  interface.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 20:37:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  interface_INC
#define  interface_INC

#include <string>
#include <vector>
#include "plugin.h"

using namespace std;

#ifdef WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT
#endif

#define RED "FF0000"
#define GREEN "00FF00"
#define BLUE "0000FF"
#define YELLOW "CCCC00"

typedef Plugin* (*GetPluginFunc)();

struct PluginDetails
{
    const string file_name;
    const string class_name;
    const string plugin_name;
    const string command;
    GetPluginFunc initFunc;
};

struct _ScheduleDetails
{
    bool continuous;
    struct tm timestamp;
    struct tm intervall;
    string user;
    string func;
    vector<string> argv;
    int num_reput;
};
typedef  struct _ScheduleDetails ScheduleDetails;

#define PLUGIN( class_type, plugin_name, command) \
    extern "C" {                                  \
        PLUGIN_EXPORT Plugin* getPlugin()         \
        {                                         \
            static class_type singleton;          \
            return &singleton;                    \
        }                                         \
        PLUGIN_EXPORT PluginDetails exports =     \
        {                                         \
            __FILE__,                             \
            #class_type,                          \
            plugin_name,                          \
            command,                              \
            getPlugin,                            \
        };                                        \
    }

#endif   /* ----- #ifndef interface_INC  ----- */

