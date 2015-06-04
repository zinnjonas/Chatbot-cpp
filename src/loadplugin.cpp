/*
 * =====================================================================================
 *
 *       Filename:  loadplugin.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11.04.2015 03:11:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "loadplugin.h"

void list_commands( string cmds, Plugin* lib, map<string,Command_Rights>& cmd, list<Plugin*>& liste)
{
    string temp = cmds;
    size_t pos;
    while( (pos = temp.find(' ')) != string::npos)
    {
        cmd[temp.substr(2,pos-2)].right = string_to_rights(temp.at(0)+"\0");
        cmd[temp.substr(2,pos-2)].library = lib;
        temp = temp.substr(pos+1);
    }
    if( !temp.empty())
    {
        cmd[temp.substr(2)].right = string_to_rights(temp.at(0)+"\0");
        cmd[temp.substr(2)].library = lib;
    }
    liste.push_back(lib);
}

void load_plugins( string path, map<string, Command_Rights>& plugins, list<Plugin*>& liste)
{

    void(*sendMsg)(void(*)(const string, const string));
    void(*setColor)(string(*)(const string, const string));
    void(*setRegister)(void(*)(ScheduleDetails));
    void(*setGenerate)(ScheduleDetails(*)(vector<string>));
    void(*setCall)(void(*)(string,vector<string>, string));
    void(*setSay)(void(*)(string));
    void(*setCreateTable)(bool(*)(string));
    void(*setSqlCmd)(bool(*)(string));
    struct dirent* dp;

    DIR* dir = opendir( path.c_str());
    if( !dir)
    {
        cout << "opendir() failure; terminating" << endl;
        return;
    }

    while( (dp = readdir(  dir)) != nullptr)
    {
        if( !strcmp( &dp->d_name[strlen(dp->d_name)-3], ".so") || !strcmp( &dp->d_name[strlen(dp->d_name)-4], ".dll"))
        {
            string name = path;
            name += dp->d_name;

            cout << "Loading: " << name << endl;
            void* lib_handle = dlopen( name.c_str(), RTLD_LAZY);
            
            if( !lib_handle)
            {
               cerr << dlerror() << endl;
               exit(1);
            }

            char* error;
            PluginDetails* info;
            info = reinterpret_cast<PluginDetails*>(dlsym(lib_handle, "exports"));
           
            if( (error = dlerror()) != NULL)
            {
                cerr << error << endl;
                exit(2);
            }
            
            *(void**)(&sendMsg) = dlsym(lib_handle, "set_send_msg");
            if( !sendMsg)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            sendMsg(send_msg);

            *(void**)(&setColor) = dlsym(lib_handle, "set_add_color");
            if( !setColor)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setColor( add_color);

            *(void**)(&setRegister) = dlsym(lib_handle, "set_add_reg_task");
            if( !setRegister)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setRegister( register_task);

            *(void**)(&setGenerate) = dlsym(lib_handle, "set_generate_schedule");
            if( !setGenerate)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setGenerate( generate_task);

            *(void**)(&setCall) = dlsym(lib_handle, "set_call");
            if( !setCall)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setCall( call);

            *(void**)(&setSay) = dlsym(lib_handle, "set_say");
            if( !setSay)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setSay( say);

            *(void**)(&setCreateTable) = dlsym(lib_handle, "set_create_table");
            if( !setCreateTable)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setCreateTable( create_table);

            *(void**)(&setSqlCmd) = dlsym(lib_handle, "set_sql");
            if( !setSqlCmd)
            {
                cerr << dlerror() << endl;
                exit(3);
            }
            setSqlCmd( sql_command);

            cout << "Plugin info: " << "\nFile Name: " << info->file_name << endl;
            cout << "Class name: " << info->class_name << endl;
            cout << "plugin name: " << info->plugin_name << endl;
            cout << "Command: " << info->command << endl;

            Plugin* pl = info->initFunc();
            pl->set_handle(lib_handle);

            list_commands( info->command, pl, plugins,liste);

        }
    }
}


void unload_plugins( map<string, Command_Rights>& plugins)
{
    plugins.clear();
}
