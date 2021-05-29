//res.h : FOR RESPONSE TO CLIENT
#include "const.h"

#define RES_RESPONSE_CODE 0
#define RES_ROOM_LIST_CODE 1
#define RES_USER_LIST_CODE 2

//Set Initial Sub Struct
struct RoomDesc {
	int room_id;
	char roomname[ROOM_NAME_MAX_LEN];
};

struct UserDesc {
	int user_id;
	char username[USER_NAME_MAX_LEN];
	int status;
};


//Set Main Struct
struct ResponseCode {
	int code;
	char error_log[LOG_MAX_LEN];
};

struct RoomList {
	int room_cnt;
	struct RoomDesc list[ROOM_MAX];
};

struct UserList {
	int user_cnt;
	struct UserDesc list[USER_MAX];
};

void send_to_user(int uid, char* buf, int size);

void response_code(int uid, int code, char* log, int size);
void res_room_list(int uid);
void res_user_list(int uid);