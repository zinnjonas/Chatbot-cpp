/*
 * =====================================================================================
 *
 *       Filename:  botplugin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08.04.2015 02:45:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  botplugin_INC
#define  botplugin_INC

#include "../interface.h"

void(*send_msg)(const string username, const string msg);
string(*add_color)(const string color, const string msg);
void (*register_task)( ScheduleDetails task);
ScheduleDetails (*generate_task)(vector<string>);
void (*call)(string, vector<string>, string);
void (*say)(string);
bool (*create_table)(string);
bool (*sql_command)(string);

extern "C" {

    PLUGIN_EXPORT void set_send_msg( void (*sendMsg)(const string, const string))
    {
        send_msg = sendMsg;
    }

    PLUGIN_EXPORT void set_add_color( string(*addColor)(const string, const string))
    {
        add_color = addColor;
    }

    PLUGIN_EXPORT void set_add_reg_task( void(*reg)(ScheduleDetails task))
    {
        register_task = reg;
    }

    PLUGIN_EXPORT void set_generate_schedule( ScheduleDetails (*generate)(vector<string> argv))
    {
        generate_task = generate;
    }

    PLUGIN_EXPORT void set_call( void (*call_func)(string, vector<string>, string))
    {
        call = call_func;
    }

    PLUGIN_EXPORT void set_say( void (*say_func)(string))
    {
        say = say_func;
    }

    PLUGIN_EXPORT void set_create_table( bool (*table)(string))
    {
      create_table = table;
    }

  PLUGIN_EXPORT void set_sql( bool (*sql_cmd)(string))
  {
    sql_command = sql_cmd;
  }
}

#endif   /* ----- #ifndef botplugin_INC  ----- */
