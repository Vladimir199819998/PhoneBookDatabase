#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;

#define BUFFER_SIZE 256


typedef int U_OPTION;
class Client
{
private:
    bool connection;
    char establish[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    string user_input_buffer;
    int option;
    const char check_message[24] = "Checking connection ...";
    int client_socket;
    struct sockaddr_in client_addr;
public:
    static const U_OPTION ADD_USER = 1;
    static const U_OPTION EDIT_USER = 2;
    static const U_OPTION DELETE_USER = 3;
    static const U_OPTION QUIT= 4;
    void bootstrap();
    void loop();
    bool connected() {
        return connection;
    }
    Client();
    ~Client() {
        close(client_socket);
        delete this;
    };
};

#endif //CLIENT_CLIENT_H









