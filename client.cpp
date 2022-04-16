//
// Created by FlagerLee on 2022/3/25.
//

#include "utils.h"


int main() {
    SOCKET udp_client = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_client == INVALID_SOCKET) {
        // create failed
        perror("Invalid socket initialization");
        printf("%d", WSAGetLastError());
        return 0;
    }

    sockaddr_in addr = {AF_INET, htons(CLIENT_PORT), {}, {}};
    if(bind(udp_client, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        // bind failed
        perror("Cannot bind udp socket");
        closesocket(udp_client);
        WSACleanup();
        return 0;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    char data[BUFF_SIZE];
    size_t len = sizeof(sockaddr_in);
    memset(data, 0, sizeof(data));

    sprintf(data, "TEST SOCKET");

    sendto(udp_client, data, BUFF_SIZE, 0, (sockaddr*)&server_addr, len);

}