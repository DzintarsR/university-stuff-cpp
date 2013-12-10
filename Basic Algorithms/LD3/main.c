#include "stdlib.h"
#include "stdio.h"
#include "string.h"

struct Value {
    int number;
    char *value;

    struct Value *left;
    struct Value *right;
};

typedef struct Value Value_t;

/* Returns number from given line */
int get_number(char *line);
/* Returns value from given line */
char *get_value(char *line, int len);
/* Free memory from binary tree */
void free_memory(struct Value *valuePtr);
/* Reset data for new template */
void prepare_environment();
/* Save given value with number on binary tree */
void save_value(int number, char *value, struct Value *valuePtr, int len);
/* Returns value from binary tree for given number or 0 if didn't find one */
char *find_value(struct Value *valuePtr, int number);
/* Generates template with current document number and binary tree values */
void create_document(FILE *file);

/* Holds first node current of binary tree */
Value_t *firstValue = NULL;

/* Holds current document number */
char document_number[10];


int main() {
    FILE *file_template = fopen("template.in", "r");
    FILE *file_values = fopen("values.in", "r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (file_values == NULL) {
        printf("Can't open template.in file!");
        return 0;
    }

    if (file_values == NULL) {
        printf("Can't open values.in file!");
        return 0;
    }

    while ((read = getline(&line, &len, file_values)) != -1) {
       if (line[0] == '-') {
           /* Regenerate enviroment and create last document */
           create_document(file_template);
           prepare_environment();

           /* Set document number */
           strcpy(document_number, get_value(line, len));
       } else {
           /* Save current template value on binary tree */
           save_value(get_number(line), get_value(line, len), firstValue, len);
       }
    }

    /* Create last document */
    create_document(file_template);
    free_memory(firstValue);

    if (line) {
        free(line);
    }

    fclose(file_template);
    fclose(file_values);

    return 0;
}

int get_number(char *line) {
    char num_str[3];
    int i;
    for (i=0; i<3; i++) {
        num_str[i] = line[i];
    }
    return atoi(num_str);
}

char *get_value(char *line, int len) {
    int i = 4;
    char *value = (char *) malloc(sizeof(char) * (len-i));
    while (line[i] != '\n') {
        value[i-4] = line[i];
        i++;
    }
    return value;
}

void free_memory(struct Value *valuePtr) {
    if (valuePtr->left != NULL) {
        free_memory(valuePtr->left);
    }
    if (valuePtr->right != NULL) {
        free_memory(valuePtr->right);
    }
    free(valuePtr);
}

void prepare_environment() {
    if (firstValue != NULL) {
        free_memory(firstValue);
    }

    firstValue = malloc(sizeof(struct Value));
    firstValue->number = 0;
    firstValue->left = NULL;
    firstValue->right = NULL;
}

void save_value(int number, char *value, struct Value *valuePtr, int len) {
    if (valuePtr->number == 0) {
        valuePtr->number = number;
        valuePtr->value = (char *) malloc(sizeof(char) * (len-4));
        strcpy(valuePtr->value, value);

        valuePtr->left = NULL;
        valuePtr->right = NULL;
        return;
    }

    if (number < valuePtr->number) {
        if (valuePtr->left == NULL) {
            valuePtr->left = malloc(sizeof(struct Value));
            valuePtr->left->number = 0;
        }
        save_value(number, value, valuePtr->left, len);
    }
    else if (number > valuePtr->number) {
        if (valuePtr->right == NULL) {
            valuePtr->right = malloc(sizeof(struct Value));
            valuePtr->right->number = 0;
        }
        save_value(number, value, valuePtr->right, len);
    }
}

char *find_value(struct Value *valuePtr, int number) {
    Value_t *p = valuePtr;

    while (p != NULL) {
        if (p->number == number) {
            return p->value;
        }

        if (number < p->number) {
            p = p->left;
        }
        else if (number > p->number) {
            p = p->right;
        }
    }

    return 0;
}

void create_document(FILE *file) {
    if (firstValue == NULL) {
        return;
    }

    fseek(file, 0, SEEK_SET);

    /* Create file for writing */
    char name[80];
    strcpy(name,"document_");
    strcat(name, document_number);
    strcat(name,".txt");
    FILE *document_file = fopen(name, "w+");

    int value, start_code = -1, i;
    char number[3], *found;

    while ((value = fgetc(file))) {
        /* Calculate number and insert appropriate text if found one */
        if (start_code > -1) {
            if (start_code == 3) {
                found = find_value(firstValue, atoi(number));
                if (found != 0) {
                    i = 0;
                    while (found[i] != '\0') {
                        fputc(found[i], document_file);
                        i++;
                    }
                }

                start_code = -1;
            } else {
                number[start_code] = value;
                start_code++;
                continue;
            }
        }

        /* Checks for EOF after last code insert */
        if (value == EOF) {
            break;
        }

        if (value == '#') {
            start_code = 0;
        }

        if (start_code == -1) {
            fputc(value, document_file);
        }
    }

    fclose(document_file);
}
