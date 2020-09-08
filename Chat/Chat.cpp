
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

//client-->server
//SET_NAME=Mike - установить имя
//MESSAGE_TO= id,message - отправить сообщение пользователю ID=id

//server->client
//NEW_USER,Mike,1221

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
                    ws->subscribe("broadcast");
                    ws->subscribe("user#"+ to_string(data->user_id));
                 },
                .message = [](auto* ws,string_view message,uWS::OpCode opCode) {
                     UserConnection* data = (UserConnection*)ws->getUserData();
                     cout << "New message " << message<<" User ID "<< data->user_id<<endl;
                     //что делать при получении сообщения(когда пользователь из существующего подключения отправляет сообщение)
                     auto begining = message.substr(0,9);
                     if (begining.compare("SET_NAME=") == 0) {
                         //пользователь прислал свое имя
                         auto name = message.substr(9);
                         data->user_name = new string(name);
                         cout << "User set their name ID = " << data->user_id << " Name = " << (*data->user_name) << endl;
                         string broadcast_message = "NEW_USER, "+ (*data->user_name)+", "+ to_string(data->user_id);
                         ws->publish("broadcast",string_view(broadcast_message), opCode, false);
                     }
                     auto is_message_to = message.substr(0, 11);
                     if (is_message_to.compare("MESSAGE_TO=") == 0) {
                         //кто-то послал сообщение другому пользователю
                         auto rest = message.substr(11);//id,message
                         int position = rest.find(",");
                         if (position != -1) {
                             auto id_string = rest.substr(0, position);
                             auto user_message = rest.substr(position + 1);
                             ws->publish("user#" + string(id_string), user_message, opCode, false);
                         }
                     }

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