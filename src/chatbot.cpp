#include "chatbot.h"

#include <regex>

    void extract_commands( const string::const_iterator first,
                           const string::const_iterator last,
                           vector<Command> &command)
    {

        string current_word;
        Command cmd;
        bool in_quotes = false;
        bool in_tiks = false;
        bool is_command = true;

        for( string::const_iterator it = first; it != last; it++)
        {
            char c = *it;

            if( in_quotes || in_tiks || c != ' ')
            {
                if( c == '"' && ! in_tiks)
                    in_quotes = !in_quotes;
                else if( c == '\'' && !in_quotes)
                    in_tiks = !in_tiks;
                else
                    current_word.push_back(c);
            }
            else if( !current_word.empty())
            {
                if( is_command)
                {
                    cmd.command = current_word;
                    is_command = !is_command;
                } else if( current_word.compare("&&") == 0)
                {
                    command.push_back(cmd);
                    cmd = Command();
                    is_command = !is_command;
                }
                else
                {
                    cmd.args.push_back(current_word);
                }
                current_word = "";
            }
        }
        if( !current_word.empty())
        {
            if( is_command)
                cmd.command = current_word;
            else
                cmd.args.push_back(current_word);
        }

        command.push_back(cmd);
    }

    Bot::Bot(string name, string password, int port, bool blocking)
    {
        // user Data
        JID jid(name);
        // client holds the connection
        m_client = new Client( jid, password, port );
        m_client->registerStanzaExtension(new XHtmlIM());
        // register the new connection manager
        m_connListener = new ConnListener( this);
        // use this class as Message handler
        m_client->registerMessageHandler( this );
        m_client->registerConnectionListener(m_connListener);
        m_client->registerPresenceHandler( m_connListener);

        m_update = false;
        m_clear = false;
        m_blocking = blocking;

        // connect blocking to the xmpp server
        m_client->connect(blocking);

        m_katarina = new Katarina();
        m_katarina->load("Greetings");
    }

    Bot::~Bot()
    {
        delete m_client;
        delete m_connListener;
    }

    void Bot::handleMessage( const Message& stanza, MessageSession* session)
    {

        // recieve data from the message
        string sender = stanza.from().username();
        string msg = stanza.body();

        // debug output on the shell
        cout << "Received message: '" << msg << "' from: '" << sender << "'" << endl;

        // in case of empty msg don't do anything
        if( msg.empty())
            return;
        map<string, User>::iterator user_it;
        if( ( user_it = m_users.find(sender)) != m_users.end())
        {
            User user = user_it->second;

            // generate message to send back
            //Message echo_msg(Message::Chat, stanza.from(), stanza.body() );
            //m_client->send( echo_msg );
            vector<Command> cmds;
            extract_commands( msg.cbegin(), msg.cend(), cmds);
            for( Command cmd : cmds)
            {
                cout << cmd.command << endl;
                map<string, Command_Rights>::iterator command_rights_it = m_plugins->find(cmd.command);
                if( command_rights_it != m_plugins->end())
                {
                    Command_Rights c_r = command_rights_it->second;
                    if( user.get_rights() >= c_r.right)
                    {
                        c_r.library->onCommand( cmd.command, cmd.args, sender);
                    }
                    else
                        send_error( sender, cmd.command);
                }
                else
                {
                    //send_error( sender, cmd.command);
                    send_to( sender, m_katarina->get_sentence(msg,sender));
                }
            }
        }
        else
        {
            Message undefined( Message::Chat, stanza.from(), add_color( RED, "I Don't know you please talk the administrators"));
            m_client->send( undefined);
        }


    if( m_clear)
    {
        m_plugins->clear();
        for( Plugin* pl : m_list_plugins)
        {
            if( (!pl->get_intern()) && (pl->get_handle() != nullptr))
            {
                    dlclose(pl->get_handle());
            }
        }

        m_list_plugins.clear();
        load_plugins();
        m_clear = false;
    }
}

void Bot::handleCommand(string cmd, vector<string> args, string sender)
{
    // in case of empty msg don't do anything
    if( cmd.empty())
        return;
    map<string, User>::iterator user_it;
    if( ( user_it = m_users.find(sender)) != m_users.end())
    {
        User user = user_it->second;

        cout << cmd << endl;
        map<string, Command_Rights>::iterator command_rights_it = m_plugins->find(cmd);
        if( command_rights_it != m_plugins->end())
        {
            Command_Rights c_r = command_rights_it->second;
            if( user.get_rights() >= c_r.right)
            {
                c_r.library->onCommand( cmd, args, sender);
            }
            else
                send_error( sender, cmd);
        }
        else
        {
            // TODO: Iwas lustiges ausdenken
            send_to( sender, m_katarina->get_sentence("Ich bin dumm",sender));
        }
    }
}

void Bot::say( string text)
{

    map<string, Command_Rights>::iterator command_rights_it = m_plugins->find("say");
    if( command_rights_it == m_plugins->end())
        return;

    vector<string> args;
    args.push_back(text);

    command_rights_it->second.library->onCommand("say", args, "");
}

bool Bot::is_connected()
{
    return (m_client->state() == StateConnected);
}

void Bot::ping()
{
    m_client->setPresence( Presence::Available,0);
}

void Bot::schedule(time_t now)
{
    for( vector<ScheduleDetails>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
    {
        if( difftime(mktime(&it->timestamp), now) <= 0)
        {
            if( print(it))
            {
                m_tasks.erase(it);
                if( it == m_tasks.end())
                    break;
            }
        }
    }
}

void Bot::retry()
{
    this_thread::sleep_for(chrono::minutes(1));
    m_client->connect(m_blocking);
}

void Bot::set_update( bool update)
{
    m_update = update;
}

bool Bot::get_update()
{
    return m_update;
}

void Bot::recieve( int timeout)
{
    m_client->recv(timeout);
}

void Bot::set_db( Database* db)
{
    m_db = db; 
    Json::Value users;
    m_db->select_all( "users", users);
    Json::Value desc = users["desc"];
    users.removeMember("desc");

    insert_users( desc, users);
}

void Bot::add_plugins( map<string, Command_Rights>* plugins, std::list<Plugin*> liste)
{
    m_plugins = plugins;
    m_list_plugins = liste;
}

void Bot::clear_plugins()
{
    m_clear = true;
}

void Bot::insert_users( Json::Value desc, Json::Value users)
{

    int index_id, index_name, index_first, index_last, index_rights, index_jabber, index_blocked;
    for( int i = 0; i < 7; i++)
    {
        string val = desc.get(i, Json::Value::null).asString();

        if( val.compare("id") == 0)
            index_id = i;
        else if( val.compare("alias") == 0)
            index_name = i;
        else if( val.compare( "first_name") == 0)
            index_first = i;
        else if( val.compare( "last_name") == 0)
            index_last = i;
        else if ( val.compare( "jabber") == 0)
            index_jabber = i;
        else if ( val.compare( "blocked") == 0)
            index_blocked = i;
        else if( val.compare( "rights") == 0)
            index_rights = i;
    }

    for( Json::ValueIterator it = users.begin(); it != users.end(); it++)
    {
        Json::Value temp = *it;
        User user;
        user.load(stoi(temp[index_id].asCString()), temp[index_jabber].asString(), temp[index_name].asString(), temp[index_first].asString(), temp[index_last].asString(), string_to_rights(temp[index_rights].asString()), temp[index_blocked].asString().at(0) == 1); 
        string jabber = temp[index_jabber].asString();
        string username = jabber.substr(0,jabber.find('@'));

        m_users[username] = user;
    }
}

Rights Bot::get_rights( string username)
{
    return m_users[username].get_rights();
}

map<string, User> Bot::get_users()
{
    return m_users;
}

map<string, Command_Rights>* Bot::get_commands()
{
    return m_plugins;
}

void Bot::send_to( const string username, const string msg)
{
    string to = m_users[username].get_jabber();
    Tag* message = new Tag("message");
    message->addAttribute( "to", to);
    message->addAttribute( "from", m_client->jid().full());
    message->addAttribute( "id", m_client->getID());
    message->addAttribute( TYPE, "chat");
    Tag* active = new Tag( message, "active");
    active->addAttribute( "xmlns", "http://jabber.org/protocol/chatstates");
    Tag* html = new Tag( message, "html");
    html->addAttribute( "xmlns", "http://jabber.org/protocol/xhtml-im");

    Tag* body = new Tag( html, "body");
    body->addAttribute( "xmlns", "http://www.w3.org/1999/xhtml");
    //body->setCData("<span style=color:#0000FF;'>Ah</span>hA");
    
    smatch m;
    regex e("<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>([^\n\r]*?)</\\1>");

    string temp_msg = msg;

    while((temp_msg.back() == '\n') || (temp_msg.back() == '\t') || (temp_msg.back() == ' '))
        temp_msg.pop_back();

    while( regex_search( temp_msg, m, e))
    {
        cout << m[0] << ";" << m[1] << ";" << m[2] << ";" << endl;
        size_t pos = m.position();
        if( pos != 0)
        {
            string temp_mgs =msg.substr( 0, pos);
            size_t br_pos = 0;
            size_t last_pos = 0;
            while( (br_pos = temp_msg.find('\n', br_pos)) != string::npos)
            {
                body->addCData(temp_msg.substr(0, br_pos));
                Tag(body,"br");
                br_pos++;
                last_pos = br_pos;
            }
            if( last_pos != string::npos)
                body->addCData(temp_msg.substr(last_pos));
        }
        
        Tag* temp = new Tag(body, m.str(1), m.str(3));
        string attributes = m.str(2);
        if( !attributes.empty() && attributes.length() > 0 )
        {
            string name = "";
            bool isname = true;
            string value = "";
            bool open;
            for( string::iterator i = attributes.begin(); i != attributes.end(); i++)
            {
                if( isname)
                {
                    if( *i != '=')
                    {
                        name += *i;
                    }
                    else
                    {
                        isname = false;
                        open=true;
                        value = "";
                    }
                }
                else
                {
                    if( *i == '\'' && open)
                    {
                        open = false;
                    }
                    else if( *i == '\'')
                    {
                        temp->addAttribute(name, value);
                        isname = true;
                        name = "";
                    }
                    else
                    {
                        value += *i;
                    }
                }
            }
        }
        temp_msg = m.suffix().str();
    }
    if( !temp_msg.empty())
    {
        string temp_mgs = temp_msg;
        size_t br_pos = 0;
        size_t last_pos = 0;
        while( (br_pos = temp_msg.find('\n', br_pos)) != string::npos)
        {
            body->addCData(temp_msg.substr(0, br_pos));
            new Tag(body, "br");
            br_pos++;
            if( br_pos == temp_msg.length())
                last_pos = string::npos;
            else
                last_pos = br_pos;
        }
        if( last_pos != string::npos)
            body->addCData(temp_msg.substr(last_pos));
    }

    m_client->send(message);
}

void Bot::send_error( const string to, const string cmd)
{
    send_to( to,  add_color( RED, "Command " + cmd + " not found or no rights to execute"));
}

void Bot::list()
{
    m_roster = m_client->rosterManager();

    Roster* r = m_roster->roster();

    for(map<string,RosterItem*>::iterator it = r->begin(); it != r->end(); it++)
    {
    }
}

void Bot::register_task(ScheduleDetails task)
{
  m_tasks.push_back(task);
  Json::Value json_task;
  char buffer[512];
  sprintf( buffer, "%d", task.continuous);
  json_task[0] = buffer;
  sprintf( buffer, "%ld", mktime( &task.timestamp));
  json_task[1] = buffer;
  sprintf( buffer, "%ld", mktime( &task.intervall));
  json_task[2] = buffer;
  json_task[3] = task.user;
  json_task[4] = task.func;
  string argv = "";
  bool first = true;
  for( string s : task.argv)
  {
    if( !first)
      argv += ";";
    else
      first = false;
    argv += s;
  }
  json_task[5] = argv;
  sprintf( buffer, "%d", task.num_reput);
  json_task[6] = buffer;

  m_db->insert("clock", json_task);
}

bool Bot::print(vector<ScheduleDetails>::iterator task)
{
    cout << "=====================================" << endl;
    cout << "continuous: " << task->continuous << endl;
    if( task->num_reput > 0)
    {
        task->num_reput--;
        if( task->num_reput == 0)
            task->continuous = false;
        cout << "reputation left: " << task->num_reput << endl;
    }
    cout << "timestamp: " << asctime( &task->timestamp);
    cout << "func: " << task->func << endl;
    for( string s : task->argv)
        cout << s << endl;
    cout << "=====================================" << endl;

    call(task->func, task->argv, task->user);

    if( !task->continuous)
        return true;

    task->timestamp = add_intervall( task->timestamp, task->intervall);
    cout << asctime(&task->timestamp);
    cout << "--------------------------" << endl;
    return false;
}

struct tm Bot::add_intervall( struct tm time, struct tm intervall)
{
    struct tm new_time;
    cout << asctime(&time);
    new_time.tm_hour = time.tm_hour + intervall.tm_hour;
    new_time.tm_min = time.tm_min + intervall.tm_min;
    new_time.tm_sec = time.tm_sec + intervall.tm_sec;
    new_time.tm_mday = time.tm_mday + intervall.tm_mday;
    new_time.tm_mon = time.tm_mon + intervall.tm_mon;
    new_time.tm_year = time.tm_year + intervall.tm_year;
    mktime(&new_time);
    cout << asctime(&new_time);
    return new_time;
}
