#include "Client.h"


Client::Client() {
    this->connection = false;
}

void Client::bootstrap() {
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Cannot create client socket ... ");
        exit(1);
    }
    this->client_addr.sin_family = AF_INET;
    this->client_addr.sin_port = htons(3452);
    this->client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(client_socket, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
        perror("Cannot open connection with a server ...");
        exit(2);
    }
    send(client_socket, check_message, sizeof(check_message), 0);
    recv(client_socket, establish, BUFFER_SIZE, 0);
    if (strcmp(establish, "Bye") != 0) {
        this->connection = true;
    }
}


void Client::loop() {
    do {
        cout << "Options provided ..." << endl;
        cout << ADD_USER << " : " << "Add a user." << endl;
        cout << EDIT_USER << " : " << "Edit a user." << endl;
        cout << REMOVE_USER << " : " << "Remove a user." << endl;
        cout << QUIT << " : " << "Quit." << endl;
        getline(cin, user_input_buffer);
        option = atoi(user_input_buffer.c_str());
        if (option < ADD_USER | option > QUIT) {
            cout << "Invalid option, try again ..." << endl;
        }
    } while (option < ADD_USER | option > QUIT);

    if (option == QUIT) {
        cout << "Bye ... ";
        exit(EXIT_SUCCESS);
    }

    string buffer = to_string(option);
    char option_to_transfer[2];
    option_to_transfer[1] = '\0';
    strcpy(option_to_transfer, buffer.c_str());
    send(client_socket, option_to_transfer, sizeof(option_to_transfer), 0);
    recv(client_socket, response, BUFFER_SIZE, 0);
    if (strcmp(response, "Bye") != 0) {
        cout << response << endl;
    } else {
        connection = false;
    }
}




