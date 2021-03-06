#ifndef SERVER_H
#define SERVER_H

#define REFRESH_SECONDS 1
#define INITIAL_SHIP_PERCENTAGE 10
#define SHIP_RENEWAL_PERCENTAGE 50

struct User {
    int id;
    int unique_ident;
	int last_read;
    char username[50];

    struct User *next;
};

typedef struct User User_t;

struct Planet {
    int id;
    int x;
    int y;
    int capacity;
    int ships;

    struct User *user;
    struct Planet *next;
};

typedef struct Planet Planet_t;

struct Attack {
    int ships;
    int time_left;

    struct Planet *planet_from;
    struct Planet *planet_to;

    struct User *attacker_user;
    struct User *defender_user;

    struct Attack *next;
    struct Attack *prev;
};

typedef struct Attack Attack_t;

struct Chat {
	int id;
    char time[9];
    char message[81];

    struct User *sender_user;
    struct User *receiver_user;

    struct Chat *next;
};

typedef struct Chat Chat_t;

Planet_t *first_planet = NULL;
User_t *first_user = NULL;
Attack_t *first_attack = NULL;
Chat_t *first_chat = NULL;
User_t *all_users = NULL;

int seconds_till_start = 0;
int max_players = 0;
int registered_players = 0;
int generated_planets = 0;
int game_speed = 0;
int max_planet_capacity = 0;
int min_planet_capacity = 0;
int chat_count = 0;

#endif