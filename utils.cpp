//
// Created by FlagerLee on 2022/3/25.
//

#include "utils.h"

bool write_echo(char* buf, const char* payload, const char* identifier, const char action, uint payload_len) {
    if(payload_len > (BUFF_SIZE - 11)) payload_len = BUFF_SIZE - 11;
    if(int(action) != 0 && int(action) != 1) {
        // invalid action
        return false;
    }
    buf[0] = 'E';
    buf[1] = 'C';
    buf[2] = 'H';
    buf[3] = 'O';
    buf[4] = identifier[0];
    buf[5] = identifier[1];
    buf[6] = identifier[2];
    buf[7] = identifier[3];
    buf[8] = action;
    buf[9] = (char)(unsigned char)(payload_len / 256);
    buf[10] = (char)(unsigned char)(payload_len % 256);
    for(int i = 0; i < payload_len; i ++) buf[i + 11] = payload[i];
    return true;
}

bool check_echo(const char* buf, char* payload, char* identifier, const char action, uint& payload_len) {
    if(buf[0] != 'E' || buf[1] != 'C' || buf[2] != 'H' || buf[3] != 'O') {
        return false;
    }
    if(buf[8] != action) {
        return false;
    }
    identifier[0] = buf[4];
    identifier[1] = buf[5];
    identifier[2] = buf[6];
    identifier[3] = buf[7];
    payload_len = (uint)(unsigned char)buf[9] * 256 + (uint)(unsigned char)buf[10];
    if(payload_len > 65489) {
        return false;
    }
    for(int i = 0; i < payload_len; i ++) payload[i] = buf[i + 11];
    payload[payload_len] = '\0';
    return true;
}
