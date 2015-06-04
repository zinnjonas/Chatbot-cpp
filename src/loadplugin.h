/*
 * =====================================================================================
 *
 *       Filename:  loadplugin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11.04.2015 03:11:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  loadplugin_INC
#define  loadplugin_INC

#include <string>
#include <string.h>
#include <map>
#include <list>
#include <sys/types.h>
#include <dirent.h>
#include "interface.h"
#include "plugin.h"
#include "user.h"

using namespace std;

struct SCommand_Rights
{
    Plugin* library;
    Rights right;
};

typedef struct SCommand_Rights Command_Rights;

extern void list_commands( string cmds, Plugin* lib, map<string,Command_Rights>& cmd, list<Plugin*>& liste);

extern void send_msg( const string username, const string msg);
extern string add_color( const string color, const string msg);
extern void register_task( ScheduleDetails task);
extern ScheduleDetails generate_task(vector<string>);
extern void call( string, vector<string>, string);
extern void say( string);
extern bool create_table( string);
extern bool sql_command( string);

extern void load_plugins( string path, map<string, Command_Rights>& plugins, list<Plugin*>& liste);
extern void unload_plugins( map<string, Command_Rights>& plugins);

#endif   /* ----- #ifndef loadplugin_INC  ----- */

