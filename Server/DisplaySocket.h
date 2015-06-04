/*
 * =====================================================================================
 *
 *       Filename:  DisplaySocket.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.04.2015 20:37:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <Sockets/TcpSocket.h>
#include <Sockets/ISocketHandler.h>
#include <string>
#include "database.h"

using namespace std;

class DisplaySocket : public TcpSocket
{
    public:
        DisplaySocket( ISocketHandler&);

        void OnRead();

    private:
        Database m_db;
};

