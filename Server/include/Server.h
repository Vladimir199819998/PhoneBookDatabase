#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <vector>
#include <sstream>
#include <algorithm>
#include <driver.h>
#include <statement.h>
#include <mysql_connection.h>
#include "Client.h"
using namespace std;
#define PASSWORD "root"
#define USERNAME "root"

typedef struct ServerSession
{
    int server_socket;
    struct sockaddr_in server_addr;
    map<CLIENT_SOCK_DESC, PERS_DATA> clients;
    int queue;
    char request[256];
    int opt = 1;
    bool listening = false;
    bool is_data_valid(CLIENT_SOCK_DESC desc) {
        if (clients[desc].is_missing()) {
            return false;
        }
        return true;
    }
} ServerSession;


class Server
{
private:
    ServerSession *sess;
public:
    void bootstrap();
    void loop();
    void add_user();
    void edit_user();
    void delete_user();
    Server();
    ~Server() {
        delete sess;
        delete this;
    };
};
#endif //SERVER_SERVER_H
