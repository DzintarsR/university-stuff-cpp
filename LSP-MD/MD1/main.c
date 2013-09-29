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
    int been_printed;

    struct Person *fatherPerson;
    struct Person *motherPerson;

    struct Person *next;
    struct Sibling *first_sibling;
};

typedef struct Person Person_t;

struct FamilyMember {
    char name[MAX_BUFFER];
    int name_l;
    int level;

    struct FamilyMember *next;
    struct Person *person;
};

typedef struct FamilyMember Family_t;

int sort_tree(struct Person *first) {
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
            if (position->mother_l > 0) {
                if (strncmp(position->mother, current->name, current->name_l) == 0) {
                    // Detects mother loop
                    if (strncmp(position->name, current->mother, position->name_l) == 0) {
                        write(1, "Mother loop\n", 13);
                        return 1;
                    }
                    position->motherPerson = current;
                }
            }

            // Check if has existing father
            if (position->father_l > 0) {
                if (strncmp(position->father, current->name, current->name_l) == 0) {
                    // Detects father loop
                    if (strncmp(position->name, current->father, position->name_l) == 0) {
                        write(1, "Father loop\n", 13);
                        return 1;
                    }
                    position->fatherPerson = current;
                }
            }

            // Check if persons are siblings
            int same_mother = 1;
            int same_father = 1;

            if (position->mother_l > 0 && current->mother_l > 0) {
                same_mother = strncmp(position->mother, current->mother, current->mother_l);
            }

            if (position->father_l > 0 && current->father_l > 0) {
                same_father = strncmp(position->father, current->father, current->father_l);
            }

            if (same_mother == 0 || same_father == 0) {
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

    return 0;
}

Person_t *add_to_family_tree(struct FamilyMember *family, char *name, int name_l, int level, struct Person *p) {
    while (family != NULL) {
        if (family->next == NULL) {
            strcpy(family->name, name);
            family->name_l = name_l;
            family->level = level;
            family->next = malloc(sizeof(struct FamilyMember));
            family->person = p;
            break;
        }
        family = family->next;
    }
}

void sort_family(struct Person *person, int level, struct FamilyMember *family) {
    if (person->motherPerson != NULL) {
        sort_family(person->motherPerson, level+1, family);
    } else {
        if (person->mother_l != 0) {
            add_to_family_tree(family, person->mother, person->mother_l, level+1, NULL);
        }
    }

    if (person->fatherPerson != NULL) {
        sort_family(person->fatherPerson, level+1, family);
    } else {
        if (person->father_l != 0) {
            add_to_family_tree(family, person->father, person->father_l, level+1, NULL);
        }
    }

    // Add family member to linked list
    add_to_family_tree(family, person->name, person->name_l, level, person);

    Sibling_t *sib = person->first_sibling;
    while (sib != NULL) {
        add_to_family_tree(family, sib->person->name, sib->person->name_l, level, sib->person);
        sib = sib->next;
    }
}

int get_deepest_level(struct FamilyMember *f) {
    int l=0, havent_printed=0;
    while (f != NULL) {
        if (f->person != NULL && f->person->been_printed != 1) {
            havent_printed = 1;
        }

        if (f->level > l) {
            l = f->level;
        }
        f = f->next;
    }

    if (havent_printed == 0) {
        return -1;
    }

    return l;
}

void print_tree(struct Person *first) {
    Person_t *vf = first;

    while (first != NULL) {
        Family_t *family = malloc(sizeof(struct FamilyMember));

        // Sort family with level and store as linked list
        sort_family(first, 0, family);

        // Retrieve deepest family tree level or -1 if
        // if all family members has been already printed
        int level = get_deepest_level(family);

        if (level != -1) {
            if (vf != first) {
                write(1, "\n", 2);
            }

            // Print family members by level [n...0]
            while (level >= 0) {
                Family_t *f = family;
                while (f != NULL) {
                    if (f->level == level) {
                        if (f->person != NULL) {
                            f->person->been_printed = 1;
                        }
                        write(1, f->name, f->name_l);
                    }
                    f = f->next;
                }
                level--;
            }
        }

        first = first->next;
    }
}

int main() {
    int n;
    char buffer[MAX_BUFFER];

    int multiple_mothers = 0;
    int multiple_fathers = 0;

    Person_t *first;
    Person_t *current = NULL;

    first = malloc(sizeof(struct Person));

    // Process and store stdin data
    while((n = read(0, buffer, sizeof(buffer))) != 0) {
        if (n == 1) continue;

        if (strncmp(buffer, "VARDS", 5) == 0) {
            if (current == NULL) {
                current = first;
            } else {
                current = current->next = malloc(sizeof(struct Person));
            }
            strcpy(current->name, buffer+6);
            current->name_l = n-6;
        }

        if (strncmp(buffer, "TEVS", 4) == 0) {
            if (current->father_l > 0) {
                multiple_fathers = 1;
            }

            strcpy(current->father, buffer+5);
            current->father_l = n-5;
        }

        if (strncmp(buffer, "MATE", 4) == 0) {
            if (current->mother_l > 0) {
                multiple_mothers = 1;
            }

            strcpy(current->mother, buffer+5);
            current->mother_l = n-5;
        }
    }

    if (multiple_fathers == 1) {
        write(1, "Multiple fathers were given\n", 29);
        return 1;
    }
    else if (multiple_mothers == 1) {
        write(1, "Multiple mothers were given\n", 29);
        return 1;
    }

    if (sort_tree(first) == 1) {
        return 1;
    }

    print_tree(first);

    return 0;
}
