#ifndef CONNECTIONLISTENER_H_
#define CONNECTIONLISTENER_H_

#include <gloox/connectionlistener.h>
#include <gloox/presencehandler.h>
#include <iostream>

#include "chatbot.h"

using namespace std;
using namespace gloox;

class Bot;

class ConnListener : public ConnectionListener, public PresenceHandler {
    public:
        ConnListener(Bot* bot);
        virtual void onConnect();
        virtual void onDisconnect(ConnectionError e);
        virtual bool onTLSConnect(const CertInfo& info);
        virtual void onStreamEvent( StreamEvent event);
        virtual void onResourceBindError( const string& resource);

        virtual void handlePresence( const Presence& presence);

    private:
        Bot* m_bot;
};

#endif // CONNECTIONLISTENER_H_
