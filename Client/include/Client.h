#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

using namespace std;

#define BUFFER_SIZE 256


typedef int CLIENT_SOCK_DESC;

typedef struct PersData
{
    string username;
    string password;
    string schema;
    int option;
    PersData() {
        username = "";
        password = "";
        schema = "";
        option = NULL;
    }
    bool is_missing() {
        return (username != "" & password != "" & schema != "");
    }



} PERS_DATA;

typedef enum UserOptions {
    DEFAULT = 0,
    ADD_USER,
    EDIT_USER,
    DELETE_USER,
    QUIT
} UserOptions;

typedef struct Session {
    int client_socket;
    struct sockaddr_in client_addr;
    UserOptions options;
    bool connection;
    PERS_DATA personal_data;

} Session;

class Client {
private:
    Session *sess;
    char response[BUFFER_SIZE];
    string user_input_buffer;
public:

    int getOption() {
        return sess->personal_data.option;
    }
    void setOption(int option) {
        sess->personal_data.option = option;
    }
    void bootstrap();

    void loop();

    Client();

    ~Client() {
        close(sess->client_socket);
        delete sess;
        delete this;
    };
};

#endif //CLIENT_CLIENT_H









