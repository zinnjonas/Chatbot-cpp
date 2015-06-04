//
// Created by Jonas on 31.05.2015.
//

#include "clock.h"

#include <ctime>
#include <string>
#include <string.h>
#include <locale>
#include <curl/curl.h>
#include <json/json.h>
#include <bits/stl_tempbuf.h>
#include <sstream>

PLUGIN(ClockPlugin, "Clock Plugin", "r:alarm r:alarm_cmd r:weather")

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_buffer( void* contents, size_t size, size_t nmemb, void* buffer)
{
    size_t realsize = size * nmemb;

    memcpy( buffer, contents, realsize);

    return realsize;
}

size_t write_data( void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL)
    {
        /*  out of memory! */
        cout << "not enough memory (realloc returned NULL)\n";
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


ClockPlugin::ClockPlugin()
{
    locale::global(std::locale("de_DE.utf8"));
    cout << "Clock Plugin created" << endl;
}

ClockPlugin::~ClockPlugin()
{

    cout << "Clock Plugin destroyed" << endl;
}

bool ClockPlugin::onCommand(const string command, const vector<string> args, const string username)
{

    if( command.compare("alarm") == 0)
    {
        ScheduleDetails task = generate_task(args);
        task.func = "alarm_cmd";
        task.user = username;
        register_task(task);
    }
    else if(command.compare("weahter"))
    {
        string weath = "Das Wetter heute\n";
        weath += weather();
        cout << weath << endl;
        //send_msg(username, weather());
        vector<string> sentence = split(weath);
        for( string s : sentence)
            say(s);
    }
    else if( command.compare("alarm_cmd") == 0)
    {
        time_t now;
        struct tm* local;
        time(&now);
        local = localtime(&now);
        char buffer[1024];

        if( local->tm_min == 0)
            strftime(buffer, 1024, "Es ist %A der %#d. %B um %H Uhr.", local);
        else if( local->tm_min == 1)
            strftime(buffer, 1024, "Es ist %A der %#d. %B um eine Minute nach %H Uhr.", local);
        else if( local->tm_min == 30)
        {
            local->tm_hour++;
            mktime(local);
            strftime(buffer, 1024, "Es ist %A der %#d. %B um halb %H.", local);
        }
        else if( local->tm_min == 45)
        {
            local->tm_hour++;
            mktime(local);
            strftime(buffer, 1024, "Es ist %A der %#d. %B um virtel vor %H.", local);
        }
        else
            strftime(buffer, 1024, "Es ist %A der %e. %B um %M Minuten nach %H Uhr.", local);

        for( size_t i = 0; i < strlen(buffer)-2; i++)
        {
            if( buffer[i] == '0' && isdigit(buffer[i+1]))
            {
                buffer[i] = ' ';
            }
        }
        string weath = weather();
        say("Guten Morgen mein Herr und Gebieter");
        say(buffer);
        vector<string> sentence = split(weath);
        for( string s : sentence)
            say(s);
    }

    return true;
}

vector<string> ClockPlugin::split(string sentence)
{
    vector<string> elems;
    stringstream ss(sentence);
    string item;
    while (getline(ss, item, '\n')) {
        elems.push_back(item);
    }
    return elems;
}

string ClockPlugin::weather()
{
    CURL* curl;
    CURLcode res;

    char buffer[512];
    string output;

    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if( curl )
    {
        curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt( curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q=Konstanz&units=metric&APPID=7da784b814713eed598edbbefd5da019");
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_buffer);
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void*)buffer);

        res = curl_easy_perform(curl);

        if( res != CURLE_OK)
        {
            cerr <<  "curl_easy_perform() failed: " <<  curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return "nichts gefunden";
        }


        curl_easy_setopt( curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/forecast?q=Konstanz&units=metric&APPID=7da784b814713eed598edbbefd5da019");
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void*)&chunk);

        res = curl_easy_perform(curl);

        if( res != CURLE_OK)
        {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return "";
        }

        Json::Reader reader;
        Json::Value weather;
        Json::Value weather_now;

        reader.parse(buffer, weather_now);
        reader.parse(chunk.memory, weather);
        Json::StyledWriter writer;


        time_t sunrise = weather_now["sys"]["sunrise"].asInt();
        time_t sunset  = weather_now["sys"]["sunset"].asInt();

        float temp = weather_now["main"]["temp"].asFloat();
        int humidity = weather_now["main"]["humidity"].asInt();

        string desc = weather_now["weather"][0]["description"].asString();
        int id = weather_now["weather"][0]["id"].asInt();
        string desc_de = get_desc(id);

        cout << "sunrise: " <<  asctime(localtime(&sunrise)) << endl;
        cout << " sunset: " <<  asctime(localtime(&sunset)) << endl;
        cout << "temp: " << temp << " humidity: " << humidity << "%%" << endl;
        cout << "weather: " <<  desc.c_str() << endl;
        cout << "weather: " <<  desc_de.c_str() << endl;


        output = "Sonnenaufgang um ";
        char time_buffer[512];
        strftime(time_buffer, 512, "%H Uhr und %M Minuten", localtime(&sunrise));
        output += time_buffer;
        output += " und Sonnenuntergang um ";
        strftime(time_buffer, 512, "%H Uhr und %M Minuten", localtime(&sunset));
        output += time_buffer;
        output += "\n";
        sprintf(time_buffer, "Aktuell sind es %d ° bei %d%% Luftfeuchtigkeit\n", (int)temp, humidity);
        output += time_buffer;
        output += "Bei aktueller Wetterlage von ";
        output += desc_de;
        output += "\n";


        time_t now;
        time(&now);
        struct tm* tomorrow = localtime(&now);
        tomorrow->tm_hour = 0;
        tomorrow->tm_min = 0;
        tomorrow->tm_sec = 0;
        tomorrow->tm_mday+= 1;
        time_t tom = mktime( tomorrow);

        float temp_max = temp,temp_min = temp;
        vector<string> weath;

        for( Json::Value val : weather["list"])
        {
            int timestamp = val["dt"].asInt();
            if( (now <= timestamp) || (timestamp < tom))
            {
                float te = val["main"]["temp"].asFloat();

                if( te > temp_max)
                    temp_max = te;
                else if( te < temp_min)
                    temp_min = te;
                weath.push_back(writer.write(val["weather"][0]["description"]));
            }
        }

        cout << "temp_min: " << temp_min << " temp_max: " <<temp_max << endl;
        sprintf(time_buffer, "Tagestemperaturen zwischen %d ° und %d °\n", (int)temp_min, (int)temp_max);
        output += time_buffer;

        int count_rain = 0;
        for(string s : weath)
        {
            if( s.find("rain") != string::npos)
            {
                count_rain++;
            }
        }

        if( count_rain >= 5)
        {
            output += "Es könnte heute noch regnen!\n";
            cout << "Es könnte heute noch regnen!\n";
        }

        curl_easy_cleanup(curl);
    }

    free(chunk.memory);

    curl_global_cleanup();

    return output;
}

string ClockPlugin::get_desc(int id)
{
    switch(id)
    {
        case 200:
            return "Gewitter mit leichtem Regen";
            break;
        case 201:
            return "Gewitter mit Regen";
            break;
        case 202:
            return "Gewitter mit viel Regen";
            break;
        case 210:
            return "leichtes Gewitter";
            break;
        case 211:
            return "Gewitter";
            break;
        case 212:
            return "schweres Gewitter";
            break;
        case 221:
            return "zerrissenes Gewitter";
            break;
        case 230:
            return "Gewitter mit leichtem Nieselregen";
            break;
        case 231:
            return "Gewitter mit Nebel";
            break;
        case 232:
            return "Gewitter mit schwerem Nebel";
            break;
        case 300:
            return "leichter intensiver Nebel";
            break;
        case 301:
            return "Nebel";
            break;
        case 302:
            return "starker intensiver Nebel";
            break;
        case 310:
            return "leichter intensiver Nebel mit Regen";
            break;
        case 311:
            return "Nebel mit Regen";
            break;
        case 312:
            return "starker intensiver Nebel mit Regen";
            break;
        case 313:
            return "Regenschauer mit Nebel";
            break;
        case 314:
            return "starke Regenschauer und Nebel";
            break;
        case 321:
            return "Nieselregen schauer";
            break;
        case 500:
            return "leichter Regen";
            break;
        case 501:
            return "normaler Regen";
            break;
        case 502:
            return "schwerer intensiver Regen";
            break;
        case 504:
            return "extremer Regen";
            break;
        case 511:
            return "Eisregen";
            break;
        case 520:
            return "leichter intensiver dauer regen";
            break;
        case 521:
            return "Regenschauer";
            break;
        case 522:
            return "starke heftige Regenschauer";
            break;
        case 531:
            return "vereinzelnte Regenschauer";
            break;
        case 600:
            return "etwas Schnee";
            break;
        case 601:
            return "Schnee";
            break;
        case 602:
            return "viel Schnee";
            break;
        case 611:
            return "Schneeregen";
            break;
        case 612:
            return "schneeregenschauer";
            break;
        case 615:
            return "leichter Regen und Schnee";
            break;
        case 616:
            return "Regen und Schnee";
            break;
        case 620:
            return "leichte Schneeschauer";
            break;
        case 621:
            return "Schneeschauer";
            break;
        case 622:
            return "heftige Schneeschauer";
            break;
        case 701:
            return "Gischt";
            break;
        case 711:
            return "Dampf";
            break;
        case 721:
            return "Dunst";
            break;
        case 731:
            return "Sand und Staub wehen";
            break;
        case 741:
            return "Nebel";
            break;
        case 751:
            return "Sand";
            break;
        case 761:
            return "Staub";
            break;
        case 762:
            return "Vulkanasche";
            break;
        case 771:
            return "Böhe";
            break;
        case 781:
            return "Tornado";
            break;
        case 800:
            return "klarer Himmel";
            break;
        case 801:
            return "ein paar Wolken";
            break;
        case 802:
            return "vereinzelte Wolken";
            break;
        case 803:
            return "bewölkt mit Aufhellungen";
            break;
        case 804:
            return "Wolken bedekt";
            break;
        case 900:
            return "Tornado";
            break;
        case 901:
            return "tropischer Sturm";
            break;
        case 902:
            return "Hurrican";
            break;
        case 903:
            return "extreme kälte";
            break;
        case 904:
            return "extreme hitze";
            break;
        case 905:
            return "extremer Wind";
            break;
        case 906:
            return "extrener Hagel";
            break;
        case 951:
            return "Ruhig";
            break;
        case 952:
            return "leichte Brise";
            break;
        case 953:
            return "angenehme Brise";
            break;
        case 954:
            return "mittlere Brise";
            break;
        case 955:
            return "frische Brise";
            break;
        case 956:
            return "steife Brise";
            break;
        case 957:
            return "starker Wind bis hin zum Sturm";
            break;
        case 958:
            return "Sturm";
            break;
        case 959:
            return "schwerer Sturm";
            break;
        case 960:
            return "Unwetter";
            break;
        case 961:
            return "starkes Unwetter";
            break;
        case 962:
            return "Hurrican";
            break;
    }
    return "";
}