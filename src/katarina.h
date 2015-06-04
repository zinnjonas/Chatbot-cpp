#ifndef KATARINA_H_
#define KATARINA_H_

#include <string>
#include <vector>
#include <list>
#include <fstream>

using namespace std;

struct SSentence
{
    string sentence;
    vector<string>* answers;
};

typedef struct SSentence Sentence;

class Katarina
{
    public:
        Katarina();
        virtual ~Katarina();

        void load(const string file);

        string get_sentence(const string& sentence, const string& user);

    protected:

        void replace_placeholder(string& sentence, const string& replacement, const char rpl);

        string grab_random_line(const vector<string>& sentence);

        void divide_in_words(const string & sentence, vector<string>& words);

        bool is_char(char c);

    private:

        vector<Sentence> m_keywords;
        list<vector<string> > m_sentences;
        string m_user;
};

#endif // KATARINA_H_

