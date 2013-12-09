#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#define READ_BUFFER 100

struct Value {
    int number;
    char *value;

    struct Value *left;
    struct Value *right;
};

typedef struct Value Value_t;

Value_t *firstValue = NULL;
char document_number[10];

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

void create_document(FILE *file) {
    if (firstValue == NULL) {
        return;
    }

    fseek(file, 0, SEEK_SET);
    char value[READ_BUFFER];
    int i;

    // Create file for writing
    char name[80];
    strcpy(name,"document_");
    strcat(name, document_number);
    strcat(name,".txt");
    FILE *document_file = fopen(name, "w+");

    int c, start_code = 0;
    while ((c = fgetc(file)) != EOF) {
        if (start_code) {
            //fwrite (valuevalue, sizeof(char), 1, pFile);
        }

        if (c == '#') {
            start_code = 1;
        } else {
            fputc(c, document_file);
        }
    }

    fclose(document_file);
}

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
           // Regenerate enviroment and create last document
           create_document(file_template);
           prepare_environment();

           // Set document number
           strcpy(document_number, get_value(line, len));
       } else {
           // Save current template value on binary tree
           save_value(get_number(line), get_value(line, len), firstValue, len);
       }
    }

    // Create last document
    create_document(file_template);
    free_memory(firstValue);

    if (line) {
        free(line);
    }

    fclose(file_template);
    fclose(file_values);

    return 0;
}
