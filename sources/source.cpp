// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <any>
#include <fstream>

//using namespace std;
using std::any;
using std::vector;
using std::cout;
using std::string;
using std::map;
using std::ifstream;
using std::any_cast;
//#include "json.hpp"
//using json=nlohmann::json;

class Json {
public:
    string _data;
    vector<string> _keys;
    vector<any> _meanings;
    vector<any> _array_meanings;
    unsigned int _current_pos = 0;
    unsigned int _smeschenie = 0;

    Json() {}

explicit Json(const string &s) {
        _data = s;
    }

    map<string, any> _field;

    bool is_object() {
        int i = 0;
        while (_data[i] != '{' && _data[i] != '[' && _data[i] != '\"'&&
        (_data[i] <= '0' || _data[i] >= '9')) {
            i++;
        }
        if (_data[i] == '{') {
            return true;
        } else { return false; }
    }

    bool is_array() {
        int i = 0;
        while ( _data[i] != '{' && _data[i] != '[' && _data[i]
        != '\"'&&(_data[i] <= '0' || _data[i] >= '9')){
            i++;
        }
        if (_data[i] == '[') {
            return true;
        } else { return false; }
    }
    string get_string()
    //считывает с начала строки,до " а потом перескакивает его
    {
        string buf;
        while (_data[_current_pos] != '\"') {
            buf.push_back(_data[_current_pos]);
            _current_pos++;
        }
        _current_pos++;
        return buf;
    }

    int get_number()
    //считывает число и доводит курсор до знака разделения или конца (, } ])
    {
        string meaning;
        while (_data[_current_pos] <= '0' || _data[_current_pos] >= '9') {
            _current_pos++;
        }
        while (_data[_current_pos] >= '0' && _data[_current_pos] <= '9') {
            meaning.push_back(_data[_current_pos]);
        }
        while (_data[_current_pos] != ',' && _data[_current_pos] != '}'
        && _data[_current_pos] != ']') {
            _current_pos++;
        }
        return atoi(meaning.c_str());
    }
    string get_key()//находит ключ и смещает значение курсора до : +1
    {
        while (_data[_current_pos] != '\"'){
            _current_pos++;
        }
        _current_pos++;
        string key = get_string();
        cout << key << " ";
        while (_data[_current_pos] != ':') {
            _current_pos++;
        }
        _current_pos++;
        return key;
    }

    void find_end_meaning_object()//перемещает курсор до , ] или }
    {
        while (_data[_current_pos] != ',' && _data[_current_pos] != '}'
        && _data[_current_pos] != ']') {
            _current_pos++;
        }
    }


    int get_num_meaning() {
        string buf;
        buf.push_back(_data[_current_pos]);
        _current_pos++;
        while (_data[_current_pos] >= '0' && _data[_current_pos] <= '9') {
            buf.push_back(_data[_current_pos]);
            _current_pos++;
        }
        return atoi(buf.c_str());
    }

    any get_the_meaning() {
        any meaning;
        while (_data[_current_pos] != '{' && _data[_current_pos]
        != '[' && _data[_current_pos] != '\"' &&
               (_data[_current_pos] <= '0' || _data[_current_pos]
               >= '9') && _data[_current_pos] != 't' &&
               _data[_current_pos] != 'f') {
            _current_pos++;
        }
        if (_data[_current_pos] >= '0' && _data[_current_pos] <= '9') {
            meaning = get_num_meaning();
            return meaning;
        }
        if (_data[_current_pos] == '\"') {
            _current_pos++;
            meaning = get_string();
            find_end_meaning_object();
            return meaning;
        }
        if (_data[_current_pos] >= 'a' && _data[_current_pos] <= 'z') {
            string buf;
            while (_data[_current_pos] >= 'a' && _data[_current_pos] <= 'z') {
                buf.push_back(_data[_current_pos]);
                _current_pos++;
            }
            if (buf == "true") {
                meaning = true;
            }
            if (buf == "false") {
                meaning = false;
            }
            find_end_meaning_object();
            return meaning;
        }
        if (_data[_current_pos] == '{') {
            Json tmp(_data.substr(_current_pos, _data.size() - _current_pos));
            meaning = tmp.parse();
            _current_pos += tmp._current_pos;
            if ((_data.find(",", _current_pos) <
            _data.find("}", _current_pos + 1)) &&
                (_data.find("}", _current_pos + 1) != string::npos)) {
                while (_data[_current_pos] != ',') {
                    _current_pos++;
                }
            }
            return meaning;
        }

        if (_data[_current_pos] == '[') {
            Json tmp(_data.substr(_current_pos, _data.size() - _current_pos));
            meaning = tmp.parse();
            _current_pos += tmp._current_pos;
            if ((_data.find(",", _current_pos) <
            _data.find("]", _current_pos + 1)) &&
                (_data.find(",", _current_pos) != string::npos)) {
                while (_data[_current_pos] != ',') {
                    _current_pos++;
                }
            }
            return meaning;
        }
        return meaning;
    }

    static Json parse_from_file(const string &path_file) {
        ifstream fin(path_file);
        char c;
        string data;
        c = fin.get();
        while (!fin.eof()) {
            data.push_back(c);
            c = fin.get();
        }
        cout << data;
        fin.close();
        Json tmp;
        tmp._data = data;
        tmp.parse();
        return tmp;
    }

    void object_parse() {
        any meaning;
        string key;
        while (_data[_current_pos] != '}') {
            key = get_key();
            //cout<<key<<": ";
            _keys.push_back(key);
            meaning = get_the_meaning();
            _meanings.push_back(meaning);
            find_end_meaning_object();
        }
        if (_data.find(",", _current_pos) != string::npos) {
            _current_pos++;
        } else {
            if (_data.find("]", _current_pos) != string::npos) {
                _current_pos++;
            } else {
                if (_data.find("}", _current_pos + 1) != string::npos) {
                    _current_pos++;
                }
            }
        }
    }

    void array_parse() {
        while (_data[_current_pos] != ']') {
            _current_pos++;
            if (_data[_current_pos] != ']')
            { _array_meanings.push_back(get_the_meaning()); }
            find_end_meaning_object();
        }
        if (_data.find(",", _current_pos) != string::npos) {
            _current_pos++;
        } else {
            if (_data.find("}", _current_pos) != string::npos) {
                _current_pos++;
            } else {
                if (_data.find("]", _current_pos + 1) != string::npos) {
                    _current_pos++;
                }
            }
        }
    }

    Json parse() {
        if (this->is_array()) {
            cout << "it's array";
            array_parse();
        }
        if (this->is_object()) {
            cout << "it's object ";
            object_parse();
        }
        return *(this);
    }

    static Json parse(const string &s) {
        Json tmp;
        tmp._data = s;
        tmp.parse();
        return tmp;
    }

    any &operator[](const string &key) {
        if (this->is_object()) {
            int i;
            for (i = 0; i <(int)_keys.size(); i++) {
                if (_keys[i] == key) { break; }
            }
            return _meanings[i];
        }
        return _meanings[0];
    }

    any &operator[](const int &index) {
        if (this->is_array()) {
            return _array_meanings[index];
        }
        return _meanings[index];
    }
};


int main() {
    Json test;
   // test = Json::parse_from_file("/home/sergei/Desktop/test.json");
    //cout << any_cast<string>(test["firstname"]);
    Json hello("{\n"
               "    \"lastname\" : \"Ivanov\",\n"
               "    \"firstname\" : \"Ivan\",\n"
               "    \"age\" : 25,\n"
               "    \"islegal\" : false,\n"
               "    \"marks\" : [\n"
               "    \t4,5,5,5,2,3\n"
               "    ],\n"
               "    \"address\" : {\n"
               "    \t\"city\" : \"Moscow\",\n"
               "        \"street\" : \"Vozdvijenka\"\n"
               "    }\n"
               "}");

    hello.parse();
    auto hi = any_cast<Json>(hello["marks"]);
    if (!any_cast<bool>(hello._meanings[3]))
    {
        //cout<<any_cast<string>(hi._meanings[1])<<endl;
        cout << any_cast<int>(hi[4]) << std::endl;
        cout << any_cast<int>(hello["age"]) << std::endl;
    }

    Json hey("[\n"
             "    {\"ticker\":\"S\","
             " \"description\": \"Futures contract for USD/RUB\"},\n"
             "    [1 , [1]] ,\n"
             "    {\"ticker\":\"GAZP-9.15\",  "
             "\"description\": \"Futures contract for GAZPROM shares\"}"
             " ]");
    hey.parse();
    Json hola = any_cast<Json>(hey[1]);
    Json her = any_cast<Json>(hola[1]);
    cout << any_cast<int>(her[0]) << std::endl;

    return 0;
}
