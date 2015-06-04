/*
 * =====================================================================================
 *
 *       Filename:  Sendsocket.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.04.2015 21:44:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  sendsocket_INC
#define  sendsocket_INC

#include <Sockets/TcpSocket.h>
#include <string>

using namespace std;

/*
 * =====================================================================================
 *        Class:  SendSocket
 *  Description:  
 * =====================================================================================
 */
class SendSocket : public TcpSocket
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        SendSocket (ISocketHandler& h);                             /* constructor */
        SendSocket (ISocketHandler& h, const string& data);
        ~SendSocket();

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */
        void OnConnect();

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        string m_data;

}; /* -----  end of class SendSocket  ----- */


#endif   /* ----- #ifndef sendsocket_INC  ----- */

