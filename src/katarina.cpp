#include "katarina.h"

#ifdef __WIN32__
    #include <string.h>
    int
    strcasecmp (const char *sz1, const char *sz2)
    {
      return _stricmp (sz1, sz2);
    }
#else
#include <strings.h>
#endif // __WIN32__


Katarina::Katarina()
{

}

Katarina::~Katarina()
{

}

void Katarina::load(const string file)
{
    ifstream input("Text/" + file + ".txt");

    if( input.is_open())
    {

        vector<string> sentences;

        while( !input.eof())
        {
            string s;
            getline( input, s);
            if( !s.empty())
                sentences.push_back(s);
        }

        input.close();

        if( sentences.empty())
            return;

        m_sentences.push_back(sentences);

        input.open( "Text/" + file + "_key.txt");

        if( input.is_open())
        {

            while( !input.eof())
            {
                string s;
                getline( input, s);
                if( !s.empty())
                {
                    Sentence sent;
                    sent.sentence = s;
                    sent.answers = &m_sentences.back();
                    m_keywords.push_back(sent);
                }
            }

            input.close();
        }
    }
}

string Katarina::get_sentence(const string& sentence, const string& user)
{
    string answer;
    vector<string> words;
    divide_in_words(sentence, words);

    for( string word : words)
    {
        for( Sentence keyword : m_keywords)
        {
            if(strcasecmp( word.c_str(),keyword.sentence.c_str()) == 0)
            {
                answer = grab_random_line(*keyword.answers);
                replace_placeholder(answer, user, '^');
                replace_placeholder(answer, keyword.sentence, '$');
                return answer;
            }
        }
    }

    /*take_phrase();
    answer = phrase[answerID].answer;
    phrase[answerID].call += 10;*/

    return "Habe nichts gefunden";
}

void Katarina::replace_placeholder( string& sentence, const string& replacement, const char repl)
{
    size_t pos = 0;
    string lookout = " ";
    lookout += repl;
    while( (pos = sentence.find(lookout, pos))!= string::npos)
    {
        pos++;
        if( (pos+1 < sentence.length() && sentence.at(pos+1) != '^')
          || (pos+1 >= sentence.length()))
            sentence = sentence.replace(pos, 1, replacement);
    }
}

string Katarina::grab_random_line( const vector<string>& sentence)
{
    return sentence.at(rand()%sentence.size());
}

void Katarina::divide_in_words(const string & sentence, vector<string> &words)
{
    int len = sentence.length();
    int pos = 0;
    int start;
    words.clear();
    while(pos < len)
    {
        while(pos < len && !is_char(sentence[pos]))
            pos++;
        start = pos;
        while(pos < len && is_char(sentence[pos]))
            pos++;
    words.push_back(sentence.substr(start, pos-start));
    }
}

bool Katarina::is_char(char c)
{
    if( c >= 'a' && c <= 'z')
        return true;
    if(c >= 'A' && c <= 'Z')
        return true;
    return false;
}
