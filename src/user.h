/*
 * =====================================================================================
 *
 *       Filename:  user.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 02:56:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  user_INC
#define  user_INC

#include <string>
#include <iostream>

using namespace std;

enum Rights {
    reads,
    writes,
    moderator,
    build
};
/* ----------  end of enum Rights  ---------- */

string rights_to_string( Rights rights);
Rights string_to_rights( string rights);

typedef enum Rights Rights;

/*
 * =====================================================================================
 *        Class:  User
 *  Description:  
 * =====================================================================================
 */
class User
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        User ();                               /* constructor */
        User (string jabber, string name, string first_name, string last_name);

        /* ====================  ACCESSORS     ======================================= */
        int get_id();
        string get_jabber();
        string get_username();
        string get_firstname();
        string get_lastname();
        Rights get_rights();
        bool is_blocked();

        /* ====================  MUTATORS      ======================================= */
        void set_id( int id);
        void set_username( string name);
        void set_firstname( string name);
        void set_lastname( string name);
        void set_rights( Rights rights);
        void set_blocked( bool blocked);

        /* ====================  OPERATORS     ======================================= */
        void load ( int id, string jabber, string name, string first_name, string last_name, Rights rights, bool blocked);
        void print();

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        int m_id;
        string m_jabber;
        string m_name;
        string m_first_name;
        string m_last_name;
        Rights m_rights;
        bool m_blocked;

}; /* -----  end of class User  ----- */


#endif   /* ----- #ifndef user_INC  ----- */
