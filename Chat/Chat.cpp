
#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <unordered_map>

using namespace std;

void to_lower(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

string to_lower2(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
string user() {
    string question;
    cout << "[USER]: ";
    getline(cin, question);//в переменную вопрос вместо первого слова дается вся строка введенная
    question = to_lower2(question);
    return question; //с помощью фции получаем текст который будет ввводить юзер в нижнем регистре
}

void bot(string text) {
    cout << "[BOT]: " << text << endl;
}

// фция будет выводить текст бота на экран

int main()
{
    unordered_map<string, string> database = {
        {"hello","heyooo, have a nice cock"},
        {"how are you","I'm in the getto RATATATAA. As you can see I'm great."},
        {"what is your name","meeeeh,POWERBOT for impotents"},

    };


    string question;


    bot("SUP! WELCOME to our SUPER POUPER POWERBOT for impotents, ask any questions!");

    while (question.compare("exit") != 0) {
        question = user();
        for (auto entry : database) {
            auto expression = regex(".*" + entry.first + ".*");
            if (regex_match(question, expression)) {
                bot(entry.second);
            }

        }
    }
    bot("Nice to see ya! Byyyyeeeee:)");














    /*
    if (regex_match(question,regex(".*hello.*"))) {
        bot("HEY DUDE, how it's going?");
    }
    //регекс - смотрит чтоб в выражении присутствовало слово хеллоу, тоесть можно добавлять знаки
    if (regex_match(question, regex(".*how are you.*"))) {
        bot("Literally good;) To be honest I'm drunk...");
    }

    if (regex_match(question, regex(".*what is your name.*"))) {
        bot("My name is SUPER POUPER POWERBOT for impotents");
    }*/

    //получить вопрос
    //найти ответ
    //вівести ответ
}