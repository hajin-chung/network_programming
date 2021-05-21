/*
 *
 *  채팅 어플
 *  
 *  - Features
 *      - Private Chatting
 *      - Public Chatting
 *      - Group Chatting
 *
 *  - Protocol
 *      - Chat
 *          - Head
 *              - RoomId : room addr
 *              - length : length of message
 *          - Body
 *              - actual chat data
 *      - User
 *          - UserId : user identifier
 *          - Name : name of user
 *          - UserSock
 *      - Room
 *          - Users[]
 *          - RoomId : chat room identifier
 *          - History[]
 *              - Chat
 *
 *  - Commands
 *      - get_room_list
 *      - get_user_data
 *
 *  - Scenario
 *      1. User connects with name
 *      2. Give new user 'UserId', 'UserSock' and Server adds User to Users array
 *      3. User makes a room
 *      4. User sends a 'Chat' to room
 *      5. Server gets 'Chat'
 *      6. Server opens Chat and view RoomId
 *      7. Server open Room of RoomId, view Users and send Chat to Users
 *
 */

#define USER_NAME_LEN 50
#define ROOM_USER_CNT 50
#define ROOM_HISTORY_LEN 50

#define BUF_SIZE 8190

#define MSG_SIZE 1000
#define TITLE_SIZE 50

#define CMD_INIT_CLI 1
#define CMD_GET_MSG 2
#define CMD_GET_ROOM 5
#define CMD_MAKE_ROOM 3
#define CMD_MAKE_MSG 6

struct CHAT_HEAD {
    int room_id;
    int length;
};

struct CHAT_BODY {
    char message[MSG_SIZE];
};

struct CHAT {
    struct CHAT_HEAD head;
    struct CHAT_BODY body;
};

struct USER {
    int id;
    char name[USER_NAME_LEN];
    int sock; 
};

struct ROOM {
    int room_id;
    int user_num;
    char title[TITLE_SIZE];
    struct USER users[ROOM_USER_CNT];
    struct CHAT history[ROOM_HISTORY_LEN];
};

void itoa(char* dest, int src);
