/*
 * =====================================================================================
 *
 *       Filename:  user.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.04.2015 02:56:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "user.h"

string rights_to_string( Rights rights)
{
    switch( rights)
    {
        case reads:
            return "read";
        break;
        case writes:
            return "write";
        break;
        case moderator:
            return "moderator";
        break;
        case build:
            return "build";
        break;
    }

    return "fail";
}

Rights string_to_rights( string rights)
{

    Rights r = reads;

    if( rights.compare("0") == 0 || rights.compare("r") == 0)
        r = reads;
    else if( rights.compare("1") == 0 || rights.compare("w") == 0)
        r = writes;
    else if( rights.compare("2") == 0 || rights.compare("m") == 0)
        r = moderator;
    else if( rights.compare("3") == 0 || rights.compare("b") == 0)
        r = build;

    return r;
}

User::User()
{

}

User::User( string jabber, string name, string first_name, string last_name)
{
    m_id = -1;
    m_name = name;
    m_first_name = first_name;
    m_last_name = last_name;
    m_rights = reads;
    m_blocked = true;
}

int User::get_id()
{
    return m_id;
}

string User::get_jabber()
{
    return m_jabber;
}

string User::get_username()
{
    return m_name;
}

string User::get_firstname()
{
    return m_first_name;
}

string User::get_lastname()
{
    return m_last_name;
}

Rights User::get_rights()
{
    return m_rights;
}

bool User::is_blocked()
{
    return m_blocked;
}

void User::set_id( int id)
{
    m_id = id;
}

void User::set_username( string name)
{
    m_name = name;
}

void User::set_firstname( string name)
{
    m_first_name = name;
}

void User::set_lastname( string name)
{
    m_last_name = name;
}

void User::set_rights( Rights rights)
{
    m_rights = rights;
}

void User::set_blocked( bool blocked)
{
    m_blocked = blocked;
}

void User::load( int id, string jabber, string name, string first_name, string last_name, Rights rights, bool blocked)
{
    m_id = id;
    m_jabber = jabber;
    m_name = name;
    m_first_name = first_name;
    m_last_name = last_name;
    m_rights = rights;
    m_blocked = blocked;
}

void User::print()
{
    cout << m_id << ";" << m_jabber << ";" << m_name << ";" << m_first_name << ";" << m_last_name << ";" << rights_to_string(m_rights) << ";" << m_blocked << endl;
}

