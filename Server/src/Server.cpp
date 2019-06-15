#include "../include/Server.h"

Server::Server() {
    listening = false;
}

void Server::bootstrap() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Cannot create a server socket ...");
        exit(1);
    }
    fcntl(server_socket, F_SETFL, SOCK_NONBLOCK);
    server_addr.sin_family = AF_INET; server_addr.sin_port = htons(3452);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind server socket ...");
        exit(2);
    }
    int result = listen(server_socket, queue);
    if (result == 0) {
        listening = true;
    }
    fcntl(server_socket, O_NONBLOCK);
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    clients.clear();
}

void Server::loop() {
    while (listening) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(server_socket, &readset);

        for (set<int>::iterator it = clients.begin(); it != clients.end(); it++ ) {
            FD_SET(*it, &readset);
        }
        struct timeval timeout;
        timeout.tv_sec = 60;
        timeout.tv_usec = 0;

        int mx = max(server_socket, *max_element(clients.begin(), clients.end()));
        int res = select(mx + 1, &readset, NULL, NULL, &timeout);
        if (res <= 0) {
            cout << "Connection timed out ...";
            listening = false;
            exit(EXIT_SUCCESS);
        }
        if (FD_ISSET(server_socket, &readset)) {
            int client_socket = accept(server_socket, NULL, NULL);
            if (client_socket < 0) {
                perror("accept");
            }
            fcntl(client_socket, F_SETFL, O_NONBLOCK);
            clients.insert(client_socket);
        }

        for (set<int>::iterator it = clients.begin(); it != clients.end(); it++) {
            if (FD_ISSET(*it, &readset)) {
                int bytes_read = recv(*it, request, sizeof(request), 0);
                if (bytes_read <= 0) {
                    close(*it);
                    clients.erase(*it);
                    continue;
                }
                int option = atoi(request); // whether to add, edit or delete user.
                switch (option) {
                    case Client::ADD_USER:
                        add_user();
                        break;
                    case Client::EDIT_USER:
                        edit_user();
                        break;
                    case Client::DELETE_USER:
                        delete_user();
                        break;
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
