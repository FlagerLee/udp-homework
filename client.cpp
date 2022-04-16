//
// Created by FlagerLee on 2022/3/25.
//

#include "utils.h"
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <iostream>
#include <condition_variable>

#define NUM_THREADS 4

char recv_buf[NUM_THREADS][BUFF_SIZE];
std::mutex socket_mutex, ptr_mutex;
std::condition_variable cv;

bool msg_arrived[NUM_THREADS];
bool finish_send[NUM_THREADS];

void run_test(SOCKET socket, int tid) {
    char payload[BUFF_SIZE];
    sprintf(payload, "ECHO REQUEST FROM THREAD %d", tid);
    uint payload_len = strlen(payload);
    const char identifier[] = {
            (char) (tid / 1000),
            (char) (tid / 100 % 10),
            (char) (tid / 10 % 10),
            (char) (tid % 10)
    };

    char buf[BUFF_SIZE];
    write_echo(buf, payload, identifier, (char) 0, payload_len);

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    uint buf_len = sizeof(server_addr);

    std::unique_lock<std::mutex> socket_lock(socket_mutex);
    sendto(socket, buf, BUFF_SIZE, 0, (sockaddr *) &server_addr, buf_len);
    finish_send[tid] = true;
    socket_lock.unlock();

    std::unique_lock<std::mutex> ptr_lock(ptr_mutex);
    cv.wait(ptr_lock, [=] { return msg_arrived[tid]; });
    if (strcmp(recv_buf[tid], payload) == 0) printf("test from thread %d success\n", tid);
    else printf("test from thread %d failed: expected \"%s\", received \"%s\"\n", tid, payload, recv_buf[tid]);
}

int main() {
    for (int i = 0; i < NUM_THREADS; i++) {
        msg_arrived[i] = false;
        finish_send[i] = false;
    }

    SOCKET udp_client = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_client == INVALID_SOCKET) {
        // create failed
        perror("Invalid socket initialization");
        return 0;
    }

    sockaddr_in addr = {AF_INET, htons(CLIENT_PORT), {}, {}};
    if (bind(udp_client, (sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        // bind failed
        perror("Cannot bind udp socket");
        return 0;
    }
    int nRecvBuf = 4 * 1024 * 1024;
    setsockopt(udp_client, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    char buf[BUFF_SIZE];
    uint len = sizeof(sockaddr_in);
    memset(buf, 0, sizeof(buf));

    std::thread pool[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) pool[i] = std::thread(run_test, udp_client, i);

    bool thread_finish_send = false;
    while (!thread_finish_send) {
        thread_finish_send = true;
        for (bool i: finish_send)
            if (!i) {
                thread_finish_send = false;
                break;
            }
    }

    char payload[BUFF_SIZE];
    char identifier[5];
    uint payload_len;

    identifier[4] = '\0';

    int num_msg = NUM_THREADS;
    while (num_msg != 0) {
        memset(buf, 0, sizeof(buf));
        recvfrom(udp_client, buf, BUFF_SIZE, 0, (sockaddr *) &server_addr, &len);
        check_echo(buf, payload, identifier, (char) 1, payload_len);
        int id = (int) identifier[0] * 1000 +
                 (int) identifier[1] * 100 +
                 (int) identifier[2] * 10 +
                 (int) identifier[3];
        msg_arrived[id] = true;
        memcpy(recv_buf[id], payload, strlen(payload));
        cv.notify_all();
        num_msg -= 1;
    }
    for (auto &i: pool) i.join();

    return 0;
}