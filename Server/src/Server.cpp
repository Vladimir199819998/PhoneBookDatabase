#include <Client.h>
#include "Server.h"

Server::Server() {
    sess = new ServerSession;
    sess->listening = false;
    sess->clients.clear();
    sess->queue = 5;
    sess->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (sess->server_socket < 0) {
        perror("socket");
        exit(0);
    }
    fcntl(sess->server_socket, F_SETFL, O_NONBLOCK);
    setsockopt(sess->server_socket, SOL_SOCKET, SO_REUSEADDR, &sess->opt, sizeof(sess->opt));
    sess->server_addr.sin_family = AF_INET;
    sess->server_addr.sin_port = htons(6000);
    sess->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

}

void Server::bootstrap() {
    if (bind(sess->server_socket, (struct sockaddr *) &sess->server_addr, sizeof(sess->server_addr)) < 0) {
        perror("bind");
        exit(1);
    }
    listen(sess->server_socket, sess->queue);
}

void Server::loop() {
    while (sess->listening) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(sess->server_socket, &readset);

        for (map<CLIENT_SOCK_DESC, PERS_DATA>::iterator it = sess->clients.begin(); it != sess->clients.end(); it++) {
            FD_SET(it->first, &readset);
        }
        struct timeval timeout;
        timeout.tv_sec = 60;
        timeout.tv_usec = 0;

        int mx = max(sess->server_socket, max_element(sess->clients.begin()->first, sess->clients.end()->first));
        int res = select(mx + 1, &readset, NULL, NULL, &timeout);
        if (res <= 0) {
            cout << "Connection timed out ...";
            sess->listening = false;
            exit(EXIT_SUCCESS);
        }
        if (FD_ISSET(sess->server_socket, &readset)) {
            int client_socket = accept(sess->server_socket, NULL, NULL);
            if (client_socket < 0) {
                perror("accept");
            }
            fcntl(client_socket, F_SETFL, O_NONBLOCK);
            PERS_DATA data;
            if (sess->clients.find(client_socket) == sess->clients.end()) {
                sess->clients[client_socket] = data;
            }
        }

        for (map<CLIENT_SOCK_DESC, PERS_DATA>::iterator it = sess->clients.begin(); it != sess->clients.end(); it++) {
            if (FD_ISSET(it->first, &readset)) {
                int bytes_read = recv(it->first, sess->request, sizeof(sess->request), 0);
                switch (bytes_read) {
                    case 0:
                        close(it->first);
                        sess->clients.erase(it->first);
                        break;
                    case 1:
                        if (!sess->is_data_valid(it->first)) {
                            char notification[] = "Ok. Before exercising your option, you must send your name, password and set a database schema ...";
                            send(it->first, notification, sizeof(notification), 0);
                            break;
                        } else {
                            sess->clients[it->first].option = atoi(sess->request);
                        }
                        break;
                    default:
                        int option = sess->clients[it->first].option;
                        stringstream ss(sess->request);
                        ss >> sess->clients[it->first].username >> sess->clients[it->first].password >> sess->clients[it->first].schema;
                        switch (option) {
                            case ADD_USER:
                                add_user();
                                break;
                            case EDIT_USER:
                                edit_user();
                                break;
                            case DELETE_USER:
                                delete_user();
                                break;
                        }

                }


            }

        }
    }
}


void Server::add_user() {
    sql::Driver *driver;
    sql::Connection *conn;
    sql::Statement *stat;
    sql::ResultSet *res;

    driver = get_driver_instance();
    conn = driver->connect("tcp://127.0.0.1:3306", USERNAME, PASSWORD);
    conn->setSchema("phonebook");


}
