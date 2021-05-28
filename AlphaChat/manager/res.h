//res.h : FOR RESPONSE TO CLIENT

#define ERROR_LOG_MAX_LEN 100
#define USERCNT_MAX_LEN 1000
#define USERNAME_MAX_LEN 100
#define ROOMCNT_MAX_LEN 1000
#define ROOMNAME_MAX_LEN 100

//Set Initial Sub Struct
struct Request_Code {
	int code;
	char error_log[ERROR_LOG_MAX_LEN];
};

struct RoomDesc {
	int room_id;
	char roomname[ROOMNAME_MAX_LEN];
};

struct UserDesc {
	int user_id;
	char username[USERNAME_MAX_LEN];
	int status;
};


//Set Main Struct
struct RoomList {
	int room_cnt;
	struct RoomDesc room_list[ROOMCNT_MAX_LEN];
};

struct UserStatusList {
	int room_num;
	struct UserDesc user_list[USERCNT_MAX_LEN];
};