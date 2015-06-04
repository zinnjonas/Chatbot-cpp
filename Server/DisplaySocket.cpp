/*
 * =====================================================================================
 *
 *       Filename:  DisplaySocket.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.04.2015 20:37:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "DisplaySocket.h"

#define RSIZE TCP_BUFSIZE_READ

DisplaySocket::DisplaySocket( ISocketHandler& h) : TcpSocket(h)
{
    m_db.open("server");
}

void DisplaySocket::OnRead()
{
    // OnRead of TcpSocket actually reads the data from the socket
    // and moves it to the input buffer (ibuf)
    TcpSocket::OnRead();
    // get number of bytes in input buffer
    size_t n = ibuf.GetLength();
    if (n > 0)
    {
        char tmp[RSIZE]; // <--- tmp's here
        ibuf.Read(tmp,n);
        printf("Read %d bytes:\n",n);
        string stuff;
        for (size_t i = 0; i < n; i++)
        {
            printf("%c",isprint(tmp[i]) ? tmp[i] : '.');
            if( isprint(tmp[i]))
                stuff += tmp[i];
        }

        if( stuff.find("CreateTable: ") != string::npos)
        {
            stuff.erase(0, 13);
            m_db.create_table(stuff);
        }

        printf("\n");
    }
}

