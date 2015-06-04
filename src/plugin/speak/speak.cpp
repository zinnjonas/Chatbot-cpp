//
// Created by Jonas on 30.05.2015.
//

#include "speak.h"
#include <string>
#include <string.h>
#include <iostream>

PLUGIN(SpeakPlugin, "Speak Plugin", "r:say r:say_en")

SpeakPlugin::SpeakPlugin()
{
    female = popen("festival_client", "w");
    if( female == NULL)
      cout << "ERROR starting festival client" << endl;

    male = popen("espeak", "w");
    if( male == NULL)
      cout << "ERROR starting espeak client" << endl;
}

SpeakPlugin::~SpeakPlugin()
{
    pclose(female);
    pclose(male);
}

void SpeakPlugin::replace_chars(string& input)
{
  size_t pos;

  while( (pos = input.find("Ä")) != string::npos)
  {
    input.replace(pos, 2, "Ae");
  }
  while( (pos = input.find("ä")) != string::npos)
  {
    input.replace(pos, 2, "ae");
  }
  while( (pos = input.find("Ü")) != string::npos)
  {
    input.replace(pos, 2, "Ue");
  }
  while( (pos = input.find("ü")) != string::npos)
  {
    input.replace(pos, 2, "ue");
  }
  while( (pos = input.find("Ö")) != string::npos)
  {
    input.replace(pos, 2, "Oe");
  }
  while( (pos = input.find("ö")) != string::npos)
  {
    input.replace(pos, 2, "oe");
  }
  while( (pos = input.find("ß")) != string::npos)
  {
    input.replace(pos, 2, "ss");
  }
  while( (pos = input.find("°")) != string::npos)
  {
    input.erase(pos, 1);
  }

  while( (pos = input.find("€")) != string::npos)
  {
    input.erase( pos, 3);
    if( pos >= input.length())
      pos = input.length() -1;

    size_t tpos = pos;

    if( isdigit(input.at(tpos)))
    {
      while( isdigit(input.at(++tpos)));
      input.insert( tpos, " EUR");
      continue;
    }

    if( isspace(input.at(tpos)))
    {
      tpos--;
      if( isdigit(input.at(tpos)))
      {
        input.insert(pos, " EUR");
        continue;
      }
      if( isspace(input.at(tpos)))
      {
        tpos--;
        if( isdigit(input.at(tpos)))
        {
          input.insert(pos, "EUR");
          continue;
        }
        tpos++;
      }
      tpos++;

      tpos++;
      if( isdigit(input.at(tpos)))
      {
        while( isdigit(input.at(++tpos)));
        input.insert( tpos, " EUR");
        continue;
      }
    }

    input.insert(pos, "EUR");
  }

  while( (pos = input.find("EUR EUR")) != string::npos)
    input.erase(pos, 4);
}

bool SpeakPlugin::onCommand(const string command, const vector<string> args, const string username)
{
    if( command.compare("say") == 0)
    {
        if( args.size() == 0)
            return true;

        if( female == NULL)
          send_msg( username, "ERROR starting festival client");

        string text = "(SayText \"";
        bool first = true;

        for( string s : args)
        {
          if( !first)
            text += " ";

          text += s;
          first = false;
        }

        text += "\")\n";

        replace_chars(text);

        fwrite( text.c_str(), 1, text.length(), female);
        fflush(female);
    }
    else if( command.compare("say_en") == 0)
    {
      if( args.size() == 0)
            return true;

        if( male == NULL)
          send_msg( username, "ERROR starting festival client");

        string text = "\"";
        bool first = true;

        for( string s : args)
        {
          if( !first)
            text += " ";

          text += s;
          first = false;
        }

        text += "\"\n";

        replace_chars(text);

        fwrite( text.c_str(), 1, text.length(), male);
        fflush(male);
    }

    return true;
}
