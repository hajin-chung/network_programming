#ifndef _CHAT_H
#define _CHAT_H

#include "chat.h"

#endif

#define ROOM_TITLE_MAX_LEN 30
#define ROOM_HISTORY_LEN 5

struct ROOM {
    int id;
    char title[ROOM_TITLE_MAX_LEN];
    struct CHAT history[ROOM_HISTORY_LEN];
};