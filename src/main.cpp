//A basic gloox tutorial by Anders Schau Knatten
//Read more at http://blog.knatten.org/2012/03/23/basic-gloox-tutorial/
//To compile on Linux: g++ -o bot bot.cpp -lgloox -lpthread
#include <iostream>
#include <string>
#include <Sockets/SocketHandler.h>
#include <iniReader.h>
#include <dlfcn.h>
#include <ctime>

#include "chatbot.h"
#include "sendsocket.h"
#include "database.h"
#include "interface.h"
#include "defaultplugin.h"

#ifdef _WIN32

char garg[MAX_PATH];

#endif // _WIN32

ostream& operator<<(ostream& os, Message::MessageType type) {
    switch (type) {
        case Message::Chat:
            os << "Chat";
            break;
        case Message::Error:
            os << "Error";
            break;
        case Message::Groupchat:
            os << "Groupchat";
            break;
        case Message::Headline:
            os << "Headline";
            break;
        case Message::Normal:
            os << "Normal";
            break;
        case Message::Invalid:
            os << "Invalid";
            break;
        default:
            os << "unknown type";
            break;
    }
    return os;
}

ostream& operator<<(ostream& os, const Message& stanza) {
    os << "type:'" << stanza.subtype() <<  "' from:'" << stanza.from().full() << "' body:'" << stanza.body() << "'";
    return os;
}

void init_db(Database& db)
{
  // TODO Jonas: place this in an init method
  if( !db.open("user"))
  {
    cout << "ERROR: " << db.get_error() << endl;
    exit(-1);
  }

  db.exists( "jabber", true);
  db.exists( "user", true);
  db.exists( "right", true);
  db.exists( "block", true);
  db.exists( "clock", true);
}

Bot* bot;
map<string, Command_Rights> plugins;
list<Plugin*> liste;

void load_plugins()
{
    list_commands( defaultPluginInfo, new DefaultPlugin(), plugins, liste);

    load_plugins( "./plugins/", plugins, liste);
    bot->add_plugins( &plugins,liste);
}


void clear_all()
{
    bot->clear_plugins();
}

string get_list_command( string user)
{
    map<string, Command_Rights>* commands = bot->get_commands();
    Rights rights = bot->get_rights(user);
    string help = "";
    for( map<string, Command_Rights>::iterator it = commands->begin(); it != commands->end(); it++)
    {
        if( it->second.right <= rights) {
            help += it->first;
            help += ", ";
        }

    }
    return help;
}

string get_users()
{
    map<string, User> users = bot->get_users();
    string user_string = "";
    for( map<string, User>::iterator it = users.begin(); it != users.end(); it++)
    {
        user_string += it->first;
        user_string += ", ";
    }

    return user_string;
}

void send_msg( const string username, const string msg)
{
    if( username.empty())
        cout << msg << endl;
    else
        bot->send_to( username, msg);
}

string add_color( const string color, const string msg)
{
    string temp;
    temp = "<span style='color: #";
    temp += color;
    temp += ";'>";
    temp += msg;
    temp += "</span>";

    return temp;
}

void register_task( ScheduleDetails task)
{
    bot->register_task(task);
}

struct tm set_interval( int hour, int min, int sec, int day, int month, int year)
{
    struct tm intervall;

    intervall.tm_hour = hour;
    intervall.tm_min = min;
    intervall.tm_sec = sec;
    intervall.tm_mday = day;
    intervall.tm_mon = month;
    intervall.tm_year = year;

    return intervall;
}

ScheduleDetails generate_schedule( bool continuous, string func, vector<string> argv, struct tm time,
                                   struct tm intervall, int repution = -1)
{
    ScheduleDetails detail;
    detail.continuous = continuous;
    detail.func = func;
    detail.intervall = intervall;
    detail.timestamp = time;
    detail.num_reput = repution;

    return detail;
}

bool is_digit(string text)
{
    string::const_iterator it = text.begin();
    while (it != text.end() && isdigit(*it)) ++it;
    return !text.empty() && it == text.end();
}

ScheduleDetails generate_task( vector<string> argv)
{

    vector<string> args;

    time_t now;

    time(&now);

    struct tm timestamp = *localtime(&now);
    mktime(&timestamp);
    cout << asctime(&timestamp);
    int reput = -1;
    size_t pos;
    string last_word = "";
    bool continuous = false;
    bool keyword = false;
    int intervall_day = 0, intervall_mon = 0, intervall_year = 0,
            intervall_hour = 0, intervall_min = 0, intervall_sec = 0;

    for( string s : argv)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if( (s.compare("mo") == 0) || (s.compare("mon") == 0) || (s.compare("montag") == 0)
            ||  (s.compare("di") == 0) || (s.compare("die") == 0) || (s.compare("dienstag") == 0)
            ||  (s.compare("mi") == 0) || (s.compare("mie") == 0) || (s.compare("mitwoch") == 0)
            ||  (s.compare("do") == 0) || (s.compare("don") == 0) || (s.compare("donnerstag") == 0)
            ||  (s.compare("fr") == 0) || (s.compare("fre") == 0) || (s.compare("freitag") == 0)
            ||  (s.compare("sa") == 0) || (s.compare("sam") == 0) || (s.compare("samstag") == 0)
            ||  (s.compare("so") == 0) || (s.compare("son") == 0) || (s.compare("sonntag") == 0)
                )
        {

            int wday = 0;
            if( (s.compare("mo") == 0) || (s.compare("mon") == 0) || (s.compare("montag") == 0))
                wday = 1;
            else if( (s.compare("di") == 0) || (s.compare("die") == 0) || (s.compare("dienstag") == 0))
                wday = 2;
            else if( (s.compare("mi") == 0) || (s.compare("mie") == 0) || (s.compare("mitwoch") == 0))
                wday = 3;
            else if( (s.compare("do") == 0) || (s.compare("don") == 0) || (s.compare("donnerstag") == 0))
                wday = 4;
            else if( (s.compare("fr") == 0) || (s.compare("fre") == 0) || (s.compare("freitag") == 0))
                wday = 5;
            else if( (s.compare("sa") == 0) || (s.compare("sam") == 0) || (s.compare("samstag") == 0))
                wday = 6;

            int daydif = wday - timestamp.tm_wday;
            cout << daydif << endl;
            if( daydif < 0)
            {

                timestamp.tm_mday += 7+daydif;
            }
            else if( daydif == 0)
                timestamp.tm_mday += 7;
            else
                timestamp.tm_mday += daydif;
        }
        else if( (pos = s.find('.')) != string::npos)
        {
            string day = s.substr(0, pos);
            size_t year_pos;
            int year = timestamp.tm_year;
            string month;
            if( (year_pos = s.find('.', pos+1)) != string::npos)
            {
                month = s.substr(pos+1, year_pos-pos-1);
                year = atoi( s.substr(year_pos+1).c_str())-1900;
            }
            else
                month = s.substr(pos+1, year_pos-pos-1);
            cout << day << "." << month << "." << year << endl;
            timestamp.tm_mday = atoi(day.c_str());
            timestamp.tm_mon = atoi(month.c_str())-1;
            timestamp.tm_year = year;

        }
        else if( (pos = s.find(':')) != string::npos)
        {
            string hour = s.substr(0, pos);
            size_t sec_pos;
            string min;
            string sec = "0";
            if( (sec_pos = s.find(':', pos+1)) != string::npos)
            {
                min = s.substr(pos+1, sec_pos-pos-1);
                sec = s.substr(sec_pos+1);
            }
            else
                min = s.substr(pos+1);

            cout << hour << ":" << min << ":" << sec << endl;

            timestamp.tm_hour = atoi(hour.c_str());
            timestamp.tm_min  = atoi(min.c_str());
            timestamp.tm_sec  = atoi(sec.c_str());
        }
        else if( (pos = s.find('x')) != string::npos)
        {
            reput = atoi(s.substr(0, s.length()-1).c_str());
        }
        else if( s.compare("morgen") == 0)
        {
            timestamp.tm_mday += 1;
        }
        else if( s.find("übermorgen") != string::npos)
        {
            timestamp.tm_mday += 2;
        }
        else if( s.find("uhr") != string::npos)
        {
            if( last_word.find(':') == string::npos)
            {
                if( is_digit(last_word))
                {
                    timestamp.tm_hour = atoi(last_word.c_str());
                    timestamp.tm_min = 0;
                    timestamp.tm_sec = 0;
                }
            }
        }
        else if( s.find("mittag") != string::npos)
        {
            timestamp.tm_hour = 12;
            timestamp.tm_min = 0;
            timestamp.tm_sec = 0;
        }
        else if( s.find("abends") != string::npos)
        {
            timestamp.tm_hour = 6;
            timestamp.tm_min = 0;
            timestamp.tm_sec = 0;
        }
        else if( s.find("nachmittag") != string::npos)
        {
            timestamp.tm_hour = 14;
            timestamp.tm_min = 0;
            timestamp.tm_sec = 0;
        }
        else if( s.find("früh") != string::npos)
        {
            timestamp.tm_hour = 6;
            timestamp.tm_min = 0;
            timestamp.tm_sec = 0;
        }
        else if( s.find("morgens") != string::npos)
        {
            timestamp.tm_hour = 8;
            timestamp.tm_min = 0;
            timestamp.tm_sec = 0;
        }
        else if( (s.compare("wöchentlich") == 0)
                 || (s.compare("jeden") == 0)
                 || (s.compare("jeweils") == 0))
        {
            intervall_day = 7;
            continuous = true;
        }
        else if( (s.compare("jede") == 0)
                 || (s.compare("alle") ==0))
        {
            keyword = true;
        }
        else if( (s.compare("min") == 0) || (s.compare("minute") == 0)
                 || (s.compare("sek") == 0) || (s.compare("sec") == 0) || (s.compare("sekunde") == 0)
                 || (s.compare("std") == 0) || (s.compare("stunde") == 0))
        {
            if( keyword)
            {
                if( is_digit(last_word))
                {
                    if( (s.compare("min") == 0) || (s.compare("minute") == 0))
                    {
                        intervall_min = atoi(last_word.c_str());
                    }
                    else if( (s.compare("sek") == 0) || (s.compare("sec") == 0) || (s.compare("sekunde") == 0))
                    {
                        intervall_sec = atoi(last_word.c_str());
                    }
                    else if( (s.compare("std") == 0) || (s.compare("stunde") == 0))
                    {
                        intervall_hour = atoi(last_word.c_str());
                    }
                }
                else
                {
                    if( (s.compare("min") == 0) || (s.compare("minute") == 0))
                    {
                        intervall_min = 1;
                    }
                    else if( (s.compare("sek") == 0) || (s.compare("sec") == 0) || (s.compare("sekunde") == 0))
                    {
                        intervall_sec = 1;
                    }
                    else if( (s.compare("std") == 0) || (s.compare("stunde") == 0))
                    {
                        intervall_hour = 1;
                    }
                }
                continuous = true;
            }
        }

        last_word = s;
    }

    mktime(&timestamp);
    cout << asctime(&timestamp);

    return generate_schedule(continuous, "Wecker", args, timestamp,
           set_interval(intervall_hour,intervall_min,intervall_sec,intervall_day,intervall_mon,intervall_year)
            ,reput);
}

void call( string cmd, vector<string> args, string user)
{
    bot->handleCommand(cmd, args, user);
}

void say( string text)
{
    bot->say(text);
}

bool create_table( string sql)
{
  return true;
}

bool sql_command( string sql)
{
  return true;
}

int main(int argc, char** argv) {

    #ifdef _WIN32
    strcpy( garg, argv[0]);
    #endif // _WIN32

    if(parseIniFile("settings/bot.ini"))
    {
        string name = getOptionToString("name");
        string password = getOptionToString("password");
        int port = getOptionToInt("port");
        cleanupIniReader();


        Database db("sql");

        init_db(db);

        bot = new Bot(name, password, port, false);

        bot->set_db( &db);

        load_plugins();

        SocketHandler h;
        SendSocket sock(h, "Hallo World");
        sock.Open("localhost", 9001);
        h.Add(&sock);

        time_t now = time(0);
        time_t then = now + 60;
        time_t sec = now + 1;

        while( h.GetCount())
        {
            now = time(0);
            h.Select(0,250);
            bot->recieve(250);
            if( now >= then)
            {
                bot->ping();
                then = now + 60;
            }
            if( now >= sec)
            {
                bot->schedule(now);
                sec = now + 1;
            }
        }
    }
    else
    {
        cout << "schade" << endl;
    }

    return 0;
}
