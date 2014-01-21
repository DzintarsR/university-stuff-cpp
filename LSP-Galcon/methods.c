#include "server.h"

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
    int x = (planet_capacity / min_planet_capacity) * (SHIP_RENEWAL_PERCENTAGE / (double)100);
    
    if (x == 0) {
        return 1;
    }
    
    return x;
}

void set_all_users() {
	if (all_users == NULL) {
            all_users = malloc(sizeof(struct User));
    }
    /* Set user data */
	all_users->id = 0;
    all_users->unique_ident = 0;
	all_users->last_read = 0;
	all_users->next = NULL;
	strcpy(all_users->username, "all");
}

User_t *get_user_by_id(int user_id) {
	if (all_users->id == user_id) return all_users;
	
    User_t *user = first_user;
    while (user != NULL) {
        if (user->id == user_id) return user;
        user = user->next;
    }

    return NULL;
}

int get_chat_count(User_t *user) {
    Chat_t *chat = first_chat;
    int i = 0;

    while (chat != NULL) {
		if (chat->receiver_user->id == user->id || chat->receiver_user->id == all_users->id && chat->id > user->last_read) {
            i++;
        }
        chat = chat->next;
    }

    return i;
}

char *get_current_time () {
    time_t now;
    struct tm *tm;

    now = time(0);
    if ((tm = localtime (&now)) == NULL) {
        printf ("Error extracting time stuff\n");
        return NULL;
    }

    char *current_time = (char *) malloc(sizeof(char));
    sprintf(current_time, "%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);

    return current_time;
}

char *create_chat(int user_to_id, char *message, int user_ident) { 
	User_t *sender_user = get_user_by_ident(user_ident);
	User_t *receiver_user = get_user_by_id(user_to_id);
	
	char *time = get_current_time();

    if (receiver_user == NULL || sender_user == NULL) {
        return "C 0 0";
    }

    Chat_t *chat = first_chat;
    for (;;) {
        if (first_chat == NULL) {
            chat = first_chat = malloc(sizeof(struct Chat));
            break;
        }        
        if (chat->next == NULL) {
            prev = chat;
            chat = chat->next = malloc(sizeof(struct Chat));
            break;
        }

        chat = chat->next;
    }
	
	chat_count++;
	/* Set chat data */
	chat->id = chat_count;
	strcpy(chat->time, time);
	strcpy(chat->message, message);
	chat->sender_user = sender_user;
    chat->receiver_user = receiver_user;

    char *command = (char *) malloc(sizeof(char));
    sprintf(command, "C %d", receiver_user->id);	// šeit jāizdomā, ko atgriezt
printf("** create_chat returns: %s\n", command);	// debug line
    return command;
}

char *get_chat_data(int user_ident) {
    User_t *user = get_user_by_ident(user_ident);

    /* Validate user existence */
    if (user == NULL) {
        return "R 0 0 0 0";
    }

    Chat_t *chat = first_chat;
    char buffer[120];
    char *command = (char *) malloc(sizeof(char));
	int chat_count = get_chat_count(user);
    sprintf(command, "R %d", chat_count);
	if (chat_count > 0){
		while (chat != NULL) {
			if (chat->receiver_user->id == user->id || chat->receiver_user->id == all_users->id && chat->id > user->last_read) {
				sprintf(buffer, " %d_%s_%s_%d", 
					chat->receiver_user->id, 
					chat->time, 
					chat->message, 
					chat->sender_user->id
				);
				strcat(command, buffer);
			}
			/* mark last read to avoid sending again */
			user->last_read = chat->id;
			chat = chat->next;
		}
	}
printf("** get_chat_data returns: %s\n", command);  // debug line
    return command;
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
    attack->next = NULL;

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

    printf("--- ATTACKS ---\n");
    while (attack != NULL) {
        attack->time_left -= 1;
        
        if (attack->time_left <= 0) {
            /* Handle planet takeover */
            Planet_t *planet = attack->planet_to;

            /* Handle sending reinforcements to own planet */
            if (attack->attacker_user == attack->defender_user) {
                if ((planet->ships + attack->ships) >= planet->capacity) {
                    planet->ships = planet->capacity;
                } else {
                    planet->ships += attack->ships;
                }
            } else {
                /* Handle attack to opponent planet */
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

        printf("FROM: %d\t TO: %d\t SHIPS: %d\t ARRIVAL: %d\t ATTACKER: %s\n", 
            attack->planet_from->id, 
            attack->planet_to->id, 
            attack->ships, 
            attack->time_left,
            (attack->attacker_user->username == NULL) ? "None" : attack->attacker_user->username
        );
        
        attack = attack->next;
    } 
    printf("--- ATTACKS ---\n\n");
}

void calculate_ships() {
    int new_ships;
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

        printf("PLANET: %d\t CAPACITY: %d\t SHIPS: %d\t USER: %s\n", 
            planet->id, 
            planet->capacity, 
            planet->ships, 
            (planet->user == NULL) ? "None" : planet->user->username
        );

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
        //printf("\n<--: %s\n\n", client_message);
        server_message = parse_request(sock, client_message);

        if (server_message == NULL) {
            char err[1000];
            sprintf(err, "ERR %s", client_message);
            write(sock, err, strlen(err));

            memset(client_message, 0, 2000);
            continue;
        }

        write(sock, server_message, strlen(server_message));
        //printf("-->: %s\n\n", server_message);

        memset(client_message, 0, 2000);
    }

    if (read_size == 0) {
            printf("Client disconnected\n");
    } 
    else if (read_size == -1) {
        perror("Socket receive failed\n");
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
        perror("Can't bind socket");
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
            perror("Can't create client thread");
            break;
        }
    }

    if (client_sock < 0) {
        perror("Accepting client connection failed");
    }
}