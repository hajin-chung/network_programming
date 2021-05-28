#pragma once

#define USER_NAME_MAX_LEN 30
#define USER_STATUS_ONLINE 1
#define USER_STATUS_OFFLINE 0

struct USER {
    int id;
    char name[USER_NAME_MAX_LEN];
    int status;
    int sock;
};
