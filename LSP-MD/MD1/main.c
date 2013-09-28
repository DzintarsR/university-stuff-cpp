#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER 64

struct Sibling {
    struct Person *person;
    struct Sibling *next;
};

typedef struct Sibling Sibling_t;

struct Person {
    char name[MAX_BUFFER];
    char mother[MAX_BUFFER];
    char father[MAX_BUFFER];

    int name_l;
    int mother_l;
    int father_l;

    struct Person *fatherPerson;
    struct Person *motherPerson;

    int been_printed;

    struct Person *next;
    struct Sibling *first_sibling;
};

typedef struct Person Person_t;

void sort_tree(struct Person *first) {
    Person_t *current, *position;
    position = first;

    while (position != NULL) {
        current = first;

        while (current != NULL) {
            // Don't math same person
            if (position == current) {
                current = current->next;
                continue;
            }

            // Check if has existing mother
            if (strncmp(position->mother, current->name, current->name_l) == 0) {
                position->motherPerson = current;
            }

            // Check if has existing father
            else if (strncmp(position->father, current->name, current->name_l) == 0) {
                position->fatherPerson = current;
            }

            // Check if persons are siblings
            int same_mother = strncmp(position->mother, current->mother, current->mother_l);
            int same_father = strncmp(position->father, current->father, current->father_l);

            if (same_mother == 0 && same_father == 0) {
                if (position->first_sibling == NULL) {
                    position->first_sibling = malloc(sizeof(struct Sibling));
                    position->first_sibling->person = current;
                } else {
                    Sibling_t *s = position->first_sibling;
                    while (s != NULL) {
                        if (s->next == NULL) {
                            s->next = malloc(sizeof(struct Sibling));
                            s->next->person = current;
                            break;
                        }
                        s = s->next;
                    }
                }
            }

            current = current->next;
        }

        position = position->next;
    }
}

void print_tree(struct Person *first) {
    if (first->motherPerson != NULL) {
        print_tree(first->motherPerson);
    } else {
        write(1, first->mother, first->mother_l);
    }

    if (first->fatherPerson != NULL) {
        print_tree(first->fatherPerson);
    } else {
        write(1, first->father, first->father_l);
    }

    write(1, first->name, first->name_l);
}

int main() {
    int n;
    char buffer[MAX_BUFFER];

    Person_t *first;
    Person_t *current = NULL;

    first = malloc(sizeof(struct Person));
    first->been_printed = 0;

    while((n = read(0, buffer, sizeof(buffer))) != 0) {
        if (n == 1) continue;

        if (strncmp(buffer, "VARDS", 5) == 0) {
            if (current == NULL) {
                current = first;
            } else {
                current = current->next = malloc(sizeof(struct Person));
                current->been_printed = 0;
            }
            strcpy(current->name, buffer+6);
            current->name_l = n-6;
        }

        if (strncmp(buffer, "TEVS", 4) == 0) {
            strcpy(current->father, buffer+5);
            current->father_l = n-5;
        }

        if (strncmp(buffer, "MATE", 4) == 0) {
            strcpy(current->mother, buffer+5);
            current->mother_l = n-5;
        }
    }

    sort_tree(first);

    print_tree(first);

    while (first != NULL) {
        if (first->fatherPerson != NULL) {
            //write(1, first->fatherPerson->name, first->fatherPerson->name_l);
        }

        //write(1, first->name, first->name_l);
        //write(1, first->father, first->father_l);
        //write(1, first->mother, first->mother_l);
        //fwrite(first->name, 1, sizeof(first->name), stdout);
        first = first->next;
    }

    return 0;
}
