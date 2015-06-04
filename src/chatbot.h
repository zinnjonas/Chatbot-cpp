#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <gloox/client.h>
#include <gloox/message.h>
#include <gloox/messagehandler.h>
#include <gloox/rostermanager.h>
#include <gloox/disco.h>
#include <gloox/xhtmlim.h>
#include <gloox/tag.h>
#include <gloox/presence.h>
#include <string>
#include <map>
#include <list>
#include <chrono>
#include <thread>

#include "connectionlistener.h"
#include "database.h"
#include "plugin.h"
#include "loadplugin.h"
#include "katarina.h"

using namespace gloox;
using namespace std;

extern ostream& operator<<(ostream& os, Message::MessageType type);
extern ostream& operator<<(ostream& os, const Message& stanza);

class ConnListener;

struct SCommand
{
    string command;
    vector<string> args;
};

typedef SCommand Command;

void extract_commands( const string::const_iterator first, const string::const_iterator last, vector<Command> &command);

class Bot : public MessageHandler {
public:
    Bot(string name, string password, int port, bool blocking = true);

    ~Bot();

    virtual void handleMessage( const Message& stanza, MessageSession* session = 0 );
    void handleCommand( string cmd, vector<string> args, string sender);

    void say(string text);

    bool is_connected();
    void ping();
    void schedule(time_t now);
    void retry();

    void set_update( bool state);
    bool get_update();

    void recieve(int timeout=20);

    void set_db( Database* db);
    void add_plugins( map<string, Command_Rights>* plugins, list<Plugin*> liste);
    void clear_plugins( );

    void insert_users( Json::Value desc, Json::Value users);
    Rights get_rights( string username);
    map<string, User> get_users();

    map<string, Command_Rights>* get_commands();

    void send_to( const string username, const string msg);
    void send_error(const string to, const string cmd);

    void list();

    void register_task( ScheduleDetails task);
    bool print( vector<ScheduleDetails>::iterator task);
    struct tm add_intervall( struct tm time, struct tm intervall);

private:
   Client* m_client;
   ConnListener* m_connListener;
   RosterManager* m_roster;
   Database* m_db;
   Katarina* m_katarina;
   map<string, User> m_users;
   map<string, Command_Rights>* m_plugins;
   vector<ScheduleDetails> m_tasks;
   std::list<Plugin*> m_list_plugins;
   bool m_update;
   bool m_blocking;
   bool m_clear;
};

extern void load_plugins();

#endif // CHATBOT_H_
