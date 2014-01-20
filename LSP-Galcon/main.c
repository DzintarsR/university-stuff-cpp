#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define REFRESH_SECONDS 1
#define INITIAL_SHIP_PERCENTAGE 10

struct User {
    int id;
    int unique_ident;
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

Planet_t *first_planet = NULL;
User_t *first_user = NULL;
Attack_t *first_attack = NULL;
int seconds_till_start = 0;
int max_players = 0;
int registered_players = 0;
int generated_planets = 0;
int game_speed = 0;
int max_planet_capacity = 0;
int min_planet_capacity = 0;

Planet_t *get_planet_by_id(int planet_id) {
    Planet_t *planet = first_planet;
    while (planet != NULL) {
        if (planet->id == planet_id) return planet;
        planet = planet->next;
    }

    return NULL;
}

User_t *get_user_by_ident(int user_ident) {
    User_t *user = first_user;
    while (user != NULL) {
        if (user->unique_ident == user_ident) {
            break;
        }
        user = user->next;
    }

    return user;
}

Planet_t *get_free_planet() {
    Planet_t *planet = first_planet;
    Planet_t *hold = NULL;

    while (planet != NULL) {
        if (planet->user == NULL) {
            if (hold == NULL) {
                hold = planet;
            } else {
                if (planet->capacity > hold->capacity) {
                    hold = planet;
                }
            }
        }

        planet = planet->next;
    }

    return hold;
}

int get_attack_count(int user_id) {
    Attack_t *attack = first_attack;
    int i = 0;

    while (attack != NULL) {
        if (attack->defender_user->id == user_id) {
            i++;
        }
        attack = attack->next;
    }

    return i;
}

int get_travel_time(int from_x, int from_y, int to_x, int to_y) {
    int distance = sqrt((from_x - to_x)*(from_x - to_x) + (from_y - to_y)*(from_y - to_y));

    return distance / game_speed;
}

int get_planet_capacity() {
    return (rand() % (max_planet_capacity - min_planet_capacity)) + min_planet_capacity;
}

int get_new_ships(int planet_capacity) {
    return planet_capacity / min_planet_capacity;
}

char *get_map_data() {
    Planet_t *planet = first_planet;
    char buffer[255];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "M %d", generated_planets);

    while (planet != NULL) {
        sprintf(buffer, " %d_%d_%d_%d_%d_%d", 
            planet->id, 
            planet->x, 
            planet->y, 
            planet->user == NULL ? 0 : planet->user->id, 
            planet->capacity, 
            planet->ships
        );
        strcat(command, buffer);

        planet = planet->next;
    }

    return command;
}

char *get_user_data() {
    User_t *user = first_user;
    char buffer[100];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "U %d", registered_players);

    while (user != NULL) {
        sprintf(buffer, " %d_%s", user->id, user->username);
        strcat(command, buffer);

        user = user->next;
    }

    return command;
}

char *get_attack_data(int user_ident) {
    User_t *user = get_user_by_ident(user_ident);

    /* Validate user existence */
    if (user == NULL) {
        return "A 0";
    }

    Attack_t *attack = first_attack;
    char buffer[100];
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "A %d", get_attack_count(user->id));

    while (attack != NULL) {
        if (attack->defender_user->id == user->id) {
            sprintf(buffer, " %d_%d_%d_%d_%d", 
                attack->planet_to->id, 
                attack->ships, 
                attack->planet_from->id, 
                attack->attacker_user->id, 
                attack->time_left
            );
            strcat(command, buffer);
        }

        attack = attack->next;
    }

    return command;
}

char *join_game(char *username, int user_ident) {
    if (seconds_till_start == 0 || registered_players >= max_players) {
        return "J 0 0";
    }

    /* Check existence for free planet */
    Planet_t *planet = get_free_planet();
    if (planet == NULL) {
        return "J 0 0";
    }

    /* Append new user to list */
    User_t *user = first_user;
    for (;;) {
        if (first_user == NULL) {
            user = first_user = malloc(sizeof(struct User));
            break;
        }        
        if (user->next == NULL) {
            user = user->next = malloc(sizeof(struct User));
            break;
        }

        user = user->next;
    }

    registered_players++;

    /* Set planet for user  */
    planet->user = user;

    /* Set user data */
    user->id = registered_players;
    user->unique_ident = user_ident;
    user->next = NULL;
    strcpy(user->username, username);   

    /* Create output string */
    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "J %d %d", user->id, seconds_till_start);
    
    return command;
}

char *create_attack(int planet_from_id, int planet_to_id, int ships, int attacker_ident) {
    Planet_t *planet_from = get_planet_by_id(planet_from_id);
    Planet_t *planet_to = get_planet_by_id(planet_to_id);
    
    User_t *attacker_user = get_user_by_ident(attacker_ident);
    User_t *defender_user = planet_to->user;

    if (planet_from == NULL || planet_to == NULL) {
        return "S 0 0";
    }

    if (attacker_user == NULL) {
        return "S 0 0";
    }

    if (planet_from->user != attacker_user) {
        return "S 0 0";
    }

    Attack_t *attack = first_attack;
    Attack_t *prev = NULL;
    for (;;) {
        if (first_attack == NULL) {
            attack = first_attack = malloc(sizeof(struct Attack));
            break;
        }        
        if (attack->next == NULL) {
            prev = attack;
            attack = attack->next = malloc(sizeof(struct Attack));
            break;
        }

        attack = attack->next;
    }

    /* Handle ship movement */
    if (planet_from->ships > ships) {
        planet_from->ships -= ships;
    } else {
        ships = planet_from->ships;
        planet_from->ships = 0;
    }

    int time = get_travel_time(planet_from->x, planet_from->y, planet_to->x, planet_to->y);

    attack->planet_from = planet_from;
    attack->planet_to = planet_to;
    attack->ships = ships;
    attack->attacker_user = attacker_user;
    attack->defender_user = defender_user;
    attack->time_left = time;
    attack->prev = prev;

    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "S %d %d", ships, time);

    return command;
}

void set_planet_data(Planet_t *planet, int x, int y, int id) {
    planet->id = id;
    planet->x = x;
    planet->y = y;
    planet->capacity = get_planet_capacity();
    planet->ships = planet->capacity * (INITIAL_SHIP_PERCENTAGE / (double)100);
    planet->next = NULL;
    planet->user = NULL;
}

void generate_planets(FILE *planets) {
    int planet_id = 1;
    int x, y;
    Planet_t *planet;

    while (fscanf(planets, "%d %d", &x, &y) != EOF) {
        if (first_planet == NULL) {
            planet = first_planet = malloc(sizeof(struct Planet));
            set_planet_data(planet, x, y, planet_id);
        } else {
            planet = planet->next = malloc(sizeof(struct Planet));
            set_planet_data(planet, x, y, planet_id);
        }
        planet_id++;
    }

    generated_planets = planet_id - 1;

    printf("--- GENERATED PLANETS ---\n");
    planet = first_planet;
    while (planet != NULL) {
        printf("X: %d\t Y: %d\t CAPACITY: %d\t SHIPS: %d\n", 
            planet->x, planet->y, planet->capacity, planet->ships);

        planet = planet->next;
    }
    printf("--- GENERATED PLANETS ---\n\n");
}

void calculate_attacks() {
    Attack_t *attack = first_attack;

    while (attack != NULL) {
        attack->time_left -= 1;
        
        if (attack->time_left <= 0) {
            /* Handle planet takeover */
            Planet_t *planet = attack->planet_to;

            if ((planet->ships - attack->ships) <= 0) {
                if ((planet->ships - attack->ships) == 0) {
                    planet->user = NULL;
                } else {
                    planet->user = attack->attacker_user;
                }
                planet->ships = attack->ships - planet->ships;
            } else {
                planet->ships -= attack->ships;
            }

            /* Remove attack from list */
            if (attack == first_attack) {
                first_attack = attack->next;
                if (first_attack != NULL) {
                    first_attack->prev = NULL;
                }
            } else {
                attack->prev->next = attack->next;
                if (attack->next != NULL) {
                    attack->next->prev = attack->prev;
                }
            }

            free(attack);
        }
        
        attack = attack->next;
    } 
}

void calculate_ships() {
    int new_ships, user_id;
    Planet_t *planet = first_planet;

    printf("--- GENERATED SHIPS ---\n");
    while (planet != NULL) {
        if (planet->ships != planet->capacity) {
            new_ships = get_new_ships(planet->capacity);
        
            if ((planet->ships + new_ships) >= planet->capacity) {
                planet->ships = planet->capacity;
            } else {
                planet->ships += new_ships;
            }
        }

        user_id = (planet->user == NULL) ? 0 : planet->user->id;
        printf("PLANET: %d\t CAPACITY: %d\t SHIPS: %d\t USER: %d\n", 
            planet->id, planet->capacity, planet->ships, user_id);

        planet = planet->next;
    }
    printf("--- GENERATED SHIPS ---\n\n");
}

char *parse_request(int sock, char *message) {
    struct sockaddr_in client;
    int addr_size = sizeof(client);
    getsockname(sock, (struct sockaddr*)&client, (socklen_t *)&addr_size);

    char username[125];
    int planet_from_id, planet_to_id, ships;

    if (seconds_till_start > 0 && message[0] != 'J') {
        return NULL;
    }

    switch (message[0]) {
        case 'J':
                if (sscanf(message, "J %s", username)) {
                    return join_game(username, sock);
                }
                break;
        case 'M':
                return get_map_data();
                break;
        case 'S':
                if (sscanf(message, "S %d %d %d", &planet_from_id, &planet_to_id, &ships)) {
                    return create_attack(planet_from_id, planet_to_id, ships, sock);
                }
                break;
        case 'U':
                return get_user_data();
                break;
        case 'A':
                return get_attack_data(sock);
                break;
        default:
                return NULL;

    }

    return NULL;
}

void *connection_handler(void *socket_desc) {
    int sock = *(int*) socket_desc;
    int read_size;
    char client_message[2000];
    char *server_message;

    /* read and answer requests */
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
        printf("<--: %s\n", client_message);
        server_message = parse_request(sock, client_message);

        if (server_message == NULL) {
            char err[1000];

            sprintf(err, "ERR %s", client_message);

            write(sock, err, strlen(err));

            client_message[0] = '\0';
            continue;
        }

        write(sock, server_message, strlen(server_message));
        printf("-->: %s\n", server_message);

        client_message[0] = '\0';
    }

    if (read_size == 0) {
            printf("Client disconnected\n");
    } 
    else if (read_size == -1) {
        perror("recv faile\n");
    }

    free(socket_desc);
    return 0;
}

void *listen_connections(void *x) {
    int socket_desc, client_sock, sockaddr_size, *new_sock;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    /* Prepare the sockaddr_in */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind failed. Error");
    }

    listen(socket_desc, 3);
    sockaddr_size = sizeof(struct sockaddr_in);

    printf("--- Start accepting players ---\n\n");

    /* Create new thread for each client */
    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) &sockaddr_size))) {
        if (seconds_till_start == 0) {
           break;
        }

        printf("Accepting new user...\n");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0) {
            perror("could not create thread");
            break;
        }
    }

    if (client_sock < 0) {
        perror("accept failed");
    }
}

int main (int argc, char *argv[]) {
    FILE *file[2];

    char buffer[20];
    int value, i;

    /* For more random numbers */
    srand(time(NULL));

    if (argc < 3) {
        printf("Specify program with input: ./main config_file planet_file\n");
        return 0;
    }

    for (i=0; i<=1; i++) {
        if ((file[i] = fopen(argv[i+1], "r")) == NULL) {
            printf("Can't open file: %s\n", argv[i+1]);
            return 0;
        }
    }

    /* Read configuration */
    while (fscanf(file[0], "%s %d", buffer, &value) != EOF) {
        if (strcmp(buffer, "PLAYERS") == 0) {
            max_players = value;
        }

        if (strcmp(buffer, "SPEED") == 0) {
            game_speed = value;
        }

        if (strcmp(buffer, "START_TIME") == 0) {
            seconds_till_start = value;
        }

        if (strcmp(buffer, "MIN_CAPACITY") == 0) {
            min_planet_capacity = value;
        }

        if (strcmp(buffer, "MAX_CAPACITY") == 0) {
            max_planet_capacity = value;
        }
    }

    /* Generate planets */
    generate_planets(file[1]);

    /* Create connection thread */
    pthread_t connections;
    if (pthread_create(&connections, NULL, listen_connections, (void*) 0) < 0) {
        perror("Can't create connection thread");
        return 0;
    }

    double time_counter = 0;
    int game_time = 0;
    clock_t this_time = clock();
    clock_t last_time = this_time;

    for (;;) {
        this_time = clock();
        time_counter += (double)(this_time - last_time);
        last_time = this_time;

        if(time_counter > (double)(REFRESH_SECONDS * CLOCKS_PER_SEC)) {
            if (seconds_till_start > 0) {
                printf("Starting after %d seconds\n", seconds_till_start);
                seconds_till_start--;
            } else {
                printf("GAME TIME IN SECONDS: %d\n\n", game_time);

                /* Recalculate ship movements and planet status */
                calculate_attacks();

                /* Recalculate ships on planets */
                calculate_ships();

                game_time++;
            }
            time_counter -= (double)(REFRESH_SECONDS * CLOCKS_PER_SEC);
        }
    }

    /*
    Command samples:
    J: join_game("karlis", "0.0.0.0:58265");
    M: get_map_data();
    S: create_attack(1, 2, 30, "0.0.0.0:58265");
    U: get_user_data();
    A: get_attack_data("0.0.0.0:58265");
    */
    
    return 0;
}