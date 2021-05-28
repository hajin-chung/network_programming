#define USER_NAME_MAX_LEN 30
#define USER_PASS_MAX_LEN 30

struct USER {
    int id;
    char name[USER_NAME_MAX_LEN];
    char password[USER_PASS_MAX_LEN];
};