//req.h : FOR REQUEST FROM CLIENT

#define USERNAME_MAX_LEN 100
#define ROOMNAME_MAX_LEN 100
#define ROOMPASS_MAX_LEN 20
#define CONTENTS_MAX_LEN 512

#define REQ_ROOM_CREATE_CODE 0
#define REQ_ROOM_DELETE_CODE 1 
#define REQ_ROOM_CONNECT_CODE 2 
#define REQ_SEND_CHAT_CODE 3
#define REQ_ROOM_INVITE_CODE 4
#define REQ_REGISTER_CODE 5

//Set Main Struct
struct Register {
	char name[USERNAME_MAX_LEN];
};

struct RoomCreate {
	int user_id;
	char room_name[ROOMNAME_MAX_LEN];
	char password[ROOMPASS_MAX_LEN];
};

struct RoomDelete {
	int user_id;
	char room_id;
	char password[ROOMPASS_MAX_LEN];
};

struct RoomConnect {
	int user_id;
	char room_id;
	char password[ROOMPASS_MAX_LEN];
};

struct SendChat {
	int type;
	int user_id;
	int room_id;
	char contents[CONTENTS_MAX_LEN];
};

struct RoomInvite {
	int user_id;
	int room_id;
};

void user_register(char* buf);
void room_create(char* buf);
void room_delete(char* buf);
void room_connect(char* buf);
void room_invite(char* buf);
void send_chat(char* buf);

void handle_request(int sock, int uid);
