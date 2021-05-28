#define CHAT_MSG_MAX_LEN 30

struct CHAT {
    int type;
    int userid;
    int roomid;
    char msg[CHAT_MSG_MAX_LEN];
};