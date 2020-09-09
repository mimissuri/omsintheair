#include <cpprest/ws_client.h>
using namespace std;
using namespace web;
using namespace web::websockets::client;

websocket_client client;

void connect()
{
    cout << "Trying to connect"
         << "\n";
    client.connect("ws://echo.websocket.org").wait();
    cout << "After to connect"
         << "\n";
}

string send_msg()
{
    websocket_outgoing_message out_msg;
    out_msg.set_utf8_message("test"); /*
    client.send(out_msg).wait(); 
    client.receive().then([](websocket_incoming_message in_msg) {
                        return in_msg.extract_string();
                    })
        .then([](string body) {
            cout << body << endl; // test
        })
        .wait();
    client.close();*/
}