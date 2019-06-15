#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <driver.h>
#include <statement.h>
#include <mysql_connection.h>
#include <fcntl.h>
#include "Client.h"
using namespace std;


#define PASSWORD "root"
#define USERNAME "root"

class Server
{
private:
    int server_socket;
    int client_socket;
    int queue = 10;
    char confirmation[64] = "Connection established! You are now able to send a request!";
    char request[3];
    bool listening;
    int opt = 1;
    set<int> clients;
    struct sockaddr_in server_addr;
public:
    void bootstrap();
    void loop();
    void add_user();
    void edit_user();
    void delete_user();
    Server();
    ~Server() {
        delete this;
    };
};
#endif //SERVER_SERVER_H
