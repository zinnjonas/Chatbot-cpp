/*
 * =====================================================================================
 *
 *       Filename:  Sendsocket.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.04.2015 21:44:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
*/
#include "sendsocket.h"

SendSocket::SendSocket( ISocketHandler& h) : TcpSocket(h)
{
}

SendSocket::SendSocket( ISocketHandler& h, const string& data) : TcpSocket(h), m_data(data)
{
}

SendSocket::~SendSocket()
{

}

void SendSocket::OnConnect()
{
    SetLineProtocol();
    if( m_data.empty())
        Send("Random text.\n");
    else
        Send( m_data + "\n");
}

