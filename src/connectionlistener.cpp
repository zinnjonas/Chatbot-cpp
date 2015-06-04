#include "connectionlistener.h"

ConnListener::ConnListener( Bot* bot)
{
    m_bot = bot;
}

void ConnListener::onConnect()
{
    cout << "ConnListener::onConnect()" << endl;
    m_bot->list();
}

void ConnListener::onDisconnect(ConnectionError e)
{
    cout << "ConnListener::onDisconnect() " << e << endl;
    if( e == 7)
        m_bot->retry();
}

bool ConnListener::onTLSConnect( const CertInfo& info)
{
    cout << "ConnListener::onTLSConnect()" << endl;
    return true;
}

void ConnListener::onStreamEvent( StreamEvent event)
{
    cout << "ConnListener::onStreamEvent()" << endl;
}

void ConnListener::onResourceBindError( const string& resource)
{
    cout << "ConnListener::onresourceBinError(" << resource << ")" << endl;
}

void ConnListener::handlePresence( const Presence& presence)
{
}

