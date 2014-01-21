#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"

#include "arpa/inet.h"
#include "netinet/in.h"
#include "pthread.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"

#include "methods.c"

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
                printf("GAME STARTS IN %d\n", seconds_till_start);
                seconds_till_start--;
            } else {
                /* Recalculate ship movements and planet status */
                calculate_attacks();

                /* Recalculate ships on planets */
                calculate_ships();

                printf("GAME TIME IN SECONDS: %d\n\n", game_time);

                game_time++;
            }
            time_counter -= (double)(REFRESH_SECONDS * CLOCKS_PER_SEC);
        }

        if (seconds_till_start == 0 && registered_players == 0) {
            printf("\n--- GAME OVER. NO ONE JOINED. :( ---\n");
            return 0;
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