
#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <uwebsockets/App.h>
#include <thread>
using namespace std;

struct UserConnection {
    unsigned long user_id; 
    string* user_name;
    
};

//SET_NAME - установить имя



int main()
{
    atomic_ulong latest_user_id = 10;

    vector<thread*> threads(thread::hardware_concurrency());
    //создали список потоков

    transform(threads.begin(), threads.end(), threads.begin(), [&latest_user_id](auto* thr) {
        return new thread([&latest_user_id] () {
            //создает не сконфигурируемое приложение--конфигурируем сервер нашего чата,то ювс...
            //"ws://127.0.0.1/"
            uWS::App().ws<UserConnection>("/*", {
                //ЛЯМДА ФУНКЦИЯ
                .open = [&latest_user_id](auto* ws) {
                    //что делать при подключении пользователя
                    UserConnection* data = (UserConnection*)ws->getUserData();//выделяю место для данных
                    data->user_id = latest_user_id++;
                    cout << "New user connected id"<< data->user_id<<endl;
                 },
                .message = [](auto* ws,string_view message,uWS::OpCode opCode) {
                     UserConnection* data = (UserConnection*)ws->getUserData();
                     cout << "New message " << message<<" User ID "<< data->user_id<<endl;
                     //что делать при получении сообщения(когда пользователь из существующего подключения отправляет сообщение) 

                }
                }).listen(9999,
                    [](auto* token) {
                        if (token) {
                            cout << "Server started and listening on port 9999"<<endl;
                        }
                        else {
                            cout << "Server failed to start :("<<endl;
                        }
                    }).run();

        });
    });
    // помощью трансформ создаем каждый раз новый поток с нашим приложением
    for_each(threads.begin(), threads.end(), [](auto* thr) {thr->join(); });
    //для каждого из тредов мы их джоиним- заканчиваем, поток исчезает
    
}