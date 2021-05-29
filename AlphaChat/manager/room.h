#pragma once

#define ROOM_STATUS_OFF 0 
#define ROOM_STATUS_ON 1

#include "const.h"
#include "req.h"
#include "user.h"

struct ROOM {
    int id;
    char name[ROOM_NAME_MAX_LEN];
    struct SendChat history[ROOM_HISTORY_LEN];
    int super_user_id;
    int users[ROOM_USER_MAX];
    int user_cnt;
    int status;
};

struct ROOM rooms[ROOM_MAX];
int rooms_cnt;

int new_room_id();
int room_contains_user(int room_id, int user_id);