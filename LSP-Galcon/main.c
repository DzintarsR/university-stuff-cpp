#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct User {
    int id;
    char username[50];
    char ip[15];

    struct User *next;
};

typedef struct User User_t;

struct Planet {
    int id;
    int user_id;
    int x;
    int y;
    int capacity;
    int ships;

    struct Planet *next;
};

typedef struct Planet Planet_t;

struct Attack {
    int planet_from;
    int planet_to;
    int ships;
    int attacker_user_id;
    int defender_user_id;
    int time_left;

    struct Attack *next;
};

typedef struct Attack Attack_t;

Planet_t *first_planet = NULL;
User_t *first_user = NULL;
Attack_t *first_attack = NULL;

Planet_t *get_planet_by_id(int planet_id) {
    Planet_t *f = first_planet;
    while (f != NULL) {
        if (f->id == planet_id) return f;
    }

    return NULL;
}

User_t *get_user_by_id(int user_id) {
    User_t *f = first_user;
    while (f != NULL) {
        if (f->id == user_id) return f;
    }

    return NULL;
}

int get_planet_count() {
    Planet_t *p = first_planet;
    int i = 0;
    while (p != NULL && ++i) p = p->next;
    return i;
}

int get_user_count() {
    User_t *p = first_user;
    int i = 0;
    while (p != NULL && ++i) p = p->next;
    return i;
}

int get_attack_count(int user_id) {
    Attack_t *p = first_attack;
    int i = 0;
    while (p != NULL) {
        if (p->defender_user_id == user_id) {
            i++;
        }
        p = p->next;
    }
    return i;
}

char *get_map_data() {
    Planet_t *p = first_planet;
    char buffer[255];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "M %d", get_planet_count());

    while (p != NULL) {
        sprintf(buffer, " %d_%d_%d_%d_%d_%d", 
            p->id, p->x, p->y, p->user_id, p->capacity, p->ships);
        strcat(command, buffer);

        p = p->next;
    }

    return command;
}

char *get_user_data() {
    User_t *u = first_user;
    char buffer[100];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "U %d", get_user_count());

    while (u != NULL) {
        sprintf(buffer, " %d_%s", u->id, u->username);
        strcat(command, buffer);

        u = u->next;
    }

    return command;
}

char *get_attack_data(char *user_ip) {
    User_t *user = first_user;
    while (user != NULL) {
        if (strcmp(user->ip, user_ip) == 0) {
            break;
        }
        user = user->next;
    }

    if (user == NULL) {
        return "A 0";
    }

    Attack_t *a = first_attack;
    char buffer[100];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "A %d", get_attack_count(user->id));

    while (a != NULL) {
        if (a->defender_user_id == user->id) {
            sprintf(buffer, " %d_%d_%d_%d_%d", 
                a->planet_to, a->ships, a->planet_from, a->attacker_user_id, a->time_left);
            strcat(command, buffer);
        }

        a = a->next;
    }

    return command;
}

char *join_game(char *username, char *user_ip) {

}

int main (int argc, char *argv[]) {
    first_planet = malloc(sizeof(struct Planet));
    first_planet->id = 1;
    first_planet->user_id = 1;
    first_planet->x = 10;
    first_planet->y = 15;
    first_planet->capacity = 100;
    first_planet->ships = 30;
    
    Planet_t *n = first_planet->next = malloc(sizeof(struct Planet));
    n->id = 2;
    n->user_id = 2;
    n->x = 20;
    n->y = 30;
    n->capacity = 70;
    n->ships = 50;
    n->next = NULL;

    //printf("%s\n", get_map_data());

    
    first_user = malloc(sizeof(struct User));
    first_user->id = 1;
    strcpy(first_user->ip, "02.125.156.14");
    strcpy(first_user->username, "karlis");
    
    User_t *u = first_user->next = malloc(sizeof(struct User));
    u->id = 2;
    strcpy(u->ip, "159.132.154.25");
    strcpy(u->username, "janis");
    u->next = NULL;

    //printf("%s\n", get_user_data());
    

    first_attack = malloc(sizeof(struct Attack));
    first_attack->planet_from = 1;
    first_attack->planet_to = 2;
    first_attack->ships = 10;
    first_attack->attacker_user_id = 1;
    first_attack->defender_user_id = 2;
    first_attack->time_left = 30;
    first_attack->next = NULL;

    printf("%s\n", get_attack_data("159.132.154.25"));
    
    return 0;
}