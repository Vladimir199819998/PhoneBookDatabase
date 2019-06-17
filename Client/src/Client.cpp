#include "Client.h"


Client::Client() {
    sess = new Session;
    sess->connection = false;
    sess->options = DEFAULT;
    sess->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (sess->client_socket < 0) {
        perror("client socket");
        exit(0);
    }
    sess->client_addr.sin_family = AF_INET;
    sess->client_addr.sin_port = htons(6000);
    sess->client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

void Client::bootstrap() {
    if (connect(sess->client_socket, (struct sockaddr *) &sess->client_addr, sizeof(sess->client_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    sess->connection = true;

}


void Client::loop() {

    while (sess->connection) {
        do {
            cout << "Options provided ..." << endl;
            cout << ADD_USER << " : " << "Add a user." << endl;
            cout << EDIT_USER << " : " << "Edit a user." << endl;
            cout << DELETE_USER << " : " << "Remove a user." << endl;
            cout << QUIT << " : " << "Quit." << endl;
            getline(cin, user_input_buffer);
            int option = atoi(user_input_buffer.c_str());
            this->setOption(option);
            if (this->getOption() < ADD_USER | this->getOption() > QUIT) {
                cout << "Invalid option, try again ..." << endl;
            }
        } while (this->getOption() < ADD_USER | this->getOption() > QUIT);

        if (this->getOption() == QUIT) {
            cout << "Bye ... ";
            exit(EXIT_SUCCESS);
        }

        string buffer = to_string(this->getOption());
        char option_to_transfer[2];
        option_to_transfer[1] = '\0';
        strcpy(option_to_transfer, buffer.c_str());
        send(sess->client_socket, option_to_transfer, sizeof(option_to_transfer), 0);
        int bytes_read = recv(sess->client_socket, response, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            cout << "Disconnecting from the server ...\n";
            sess->connection = false;
        } else {
            cout << response << endl;
        }
    }
}

