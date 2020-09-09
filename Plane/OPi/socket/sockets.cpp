#include "easywsclient.hpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately

int main()
{
    ... using easywsclient::WebSocket;
    WebSocket::pointer ws = WebSocket::from_url("ws://localhost:8080");
    assert(ws);
    while (true)
    {
        ws->poll();
        ws->send("hello");
        ws->dispatch(handle_message);
        // ...do more stuff...
    }
    ... delete ws; // alternatively, use unique_ptr<> if you have C++11
    return 0;
}