#pragma once

#include "req.h"
#include "user.h"

#define ROOM_HISTORY_LEN 5
#define ROOM_USER_MAX 10

struct ROOM {
    int id;
    char name[ROOMNAME_MAX_LEN];
    struct SendChat history[ROOM_HISTORY_LEN];
    struct USER users[ROOM_USER_MAX];
};