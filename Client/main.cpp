#include "Client.h"

int main() {

    Client* client = new Client();
    client->bootstrap();
    client->loop();
    cout << "Connection has been ended. Exiting from program.";
    return 0;
}