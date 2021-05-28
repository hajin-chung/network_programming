#pragma once

#define USER_MAX 1000
#define USER_NAME_MAX_LEN 100
#define USER_STATUS_ONLINE 1
#define USER_STATUS_OFFLINE 0

struct USER {
    int id;
    char name[USER_NAME_MAX_LEN];
    int status;
    int sock;
};

struct USER users[USER_MAX];
int users_cnt;

int new_user_id();