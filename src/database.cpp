/*
 * =====================================================================================
 *
 *       Filename:  database.cpp
 *
 *    Description:  Implementation of the SQLite3 interface
 *
 *        Version:  1.0
 *        Created:  01.04.2015 02:20:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "database.h"

Database::Database(string sql_folder)
{
    m_sql_folder = sql_folder;
    m_open = false;
    m_zErrMsg  = 0;
}

Database::~Database()
{
    if( m_open)
        sqlite3_close(m_db);
   if( m_zErrMsg != NULL)
       sqlite3_free(m_zErrMsg);

}

string Database::get_error()
{
    return sqlite3_errmsg(m_db);
}

void Database::get_sql_from_file( string name, string& content)
{

    string file = m_sql_folder+"/" + name+ ".sql";
    ifstream input(file.c_str());    
    if( input.is_open())
    {
        input.seekg(0,ios::end);
        content.reserve(input.tellg());
        input.seekg(0, ios::beg);
    
        content.assign((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    }
    else
    {
        cout << "FILE NOT FOUND: "<< file << "!!" << endl;
    }
}

bool Database::open( string db)
{

    size_t pos = db.find(".db");
    if( pos == string::npos)
        db += ".db";

    if( sqlite3_open(db.c_str(), &m_db))
    {
        return false;
    }
    
    m_open = true;

    return true;
}

bool Database::exists( string table, bool create)
{

    Jsonwrapper exist;

    string sql;
    get_sql_from_file( "exists", sql);
    sql.replace( sql.find("$1"), 2, table.c_str());
    cout << sql << endl;

    int rc = sqlite3_exec( m_db, sql.c_str(), json_callback, (void*)&exist, &m_zErrMsg); 

    if( is_error(rc))
        return false;
    
    bool exists;

    if( exist.get_count() == 1 && exist.get_cell( 0, 0, exists))
    {   
        if( !exists && create)
            return this->create( table);
    }
    else
        return false;
    return true;
}

bool Database::create( string table)
{

    string sql;
    get_sql_from_file (table, sql);
    cout << sql << endl;
    int rc = sqlite3_exec( m_db, sql.c_str(), callback, nullptr, &m_zErrMsg);

    return !is_error(rc);   
}

bool Database::select_all( string table, Json::Value& data)
{

    Jsonwrapper wrapper;
    string sql;
    get_sql_from_file( "select_"+table, sql);
    cout << sql << endl;
    int rc = sqlite3_exec( m_db, sql.c_str(), json_callback,(void*) &wrapper, &m_zErrMsg);

    if( is_error(rc))
    {
        return false;
    }

    data = wrapper.get_content();

    return true;
}

bool Database::insert(string table, Json::Value values)
{

  string sql;
  get_sql_from_file( table+"_insert", sql);
  char replace[12];
  size_t i;
  for( i = 0; i < values.size(); i++)
  {
    sprintf(replace, "$%d", i + 1);
    sql.replace(sql.find(replace), (i/10 + 2), values.get(i, Json::Value::null).asCString());
    cout << sql << endl;
  }

  return true;
}


bool Database::is_error( int rc)
{
    if( rc != SQLITE_OK)
    {
        cout << "SQL error: " << m_zErrMsg << endl;
        sqlite3_free(m_zErrMsg);
        return true;
    }
    return false;
}

int Database::callback( void* data, int argc, char** argv, char** azColName)
{
    for( int i= 0; i < argc; i++)
    {
        cout << azColName[i] << ";" << argv[i] << endl;
    }
    return 0;
}

int Database::json_callback( void* data, int argc, char** argv, char** azColName)
{
    Jsonwrapper* json = (Jsonwrapper*)data;
    
    if( json->empty() )
    {
        Json::Value desc;
        for( int i = 0; i < argc; i++)
            desc.append(azColName[i]);
        json->set("desc", desc);
        json->clear_count();
    }
    Json::Value entry;
    for( int i = 0; i < argc; i++)
    {
        if( argv[i])
            entry.append(argv[i]);
        else
            entry.append(Json::Value::null);
    }
    
    json->set_entry(entry);
    return 0;
}

