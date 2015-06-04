/*
 * =====================================================================================
 *
 *       Filename:  jsonwrapper.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03.04.2015 16:50:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "jsonwrapper.h"

Jsonwrapper::Jsonwrapper()
{
}

string Jsonwrapper::get_send_content()
{
    Json::FastWriter writer;
    return writer.write(m_value);
}

Json::Value Jsonwrapper::get_content()
{
    return m_value;
}

bool Jsonwrapper::get_description( int index, string& desc)
{
    Json::Value lDesc = m_value.get("desc", Json::Value::null);
    if( lDesc == Json::Value::null)
        return false;
    desc = lDesc[index].asString();
    return true;
}

bool Jsonwrapper::get_cell( int index, int col, string& entry)
{
    char buffer[20];
    sprintf( buffer, "%d", index);
    Json::Value lEntry = m_value.get(buffer, Json::Value::null);
    if( lEntry == Json::Value::null)
        return false;
    entry = lEntry[col].asString();
    return true;
}

bool Jsonwrapper::get_cell( int index, int col, bool& entry)
{
    string output;
    if( !get_cell( index, col, output))
        return false;
    
    entry = (output.at(0) == '1');

    return true;
}

bool Jsonwrapper::get_cell( int index, int col, double& entry)
{
    string output;
    if( !get_cell( index, col, output))
        return false;

    entry = stod(output);
    return true;
}

bool Jsonwrapper::get_cell( int index, int col, int& entry)
{
    string output;
    if( !get_cell( index, col, output))
        return false;

    entry = stoi(output);
    return true;
}

bool Jsonwrapper::empty()
{
    return m_value.empty();
}

int Jsonwrapper::get_count()
{
    return m_count;
}

ostream& operator<<( ostream& os, Jsonwrapper& jw)
{
    os << jw.get_content();
    return os;
}

void Jsonwrapper::set( const char* key, const char*  value)
{
    m_value[key] = value;
}

void Jsonwrapper::set( const char* key, Json::Value value)
{
    m_value[key] = value;
}

void Jsonwrapper::set_entry( Json::Value value)
{
    char buffer[20];
    sprintf(buffer, "%d", m_count++);
    m_value[buffer] = value;
}

void Jsonwrapper::clear_count()
{
    m_count = 0;
}

