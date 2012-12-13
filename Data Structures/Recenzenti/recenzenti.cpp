#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct Pse {
    long int id;
    Rec *rec;
};

struct Rec {
    char *name;
    Pse *pse[50];
    int count;
};

class Recenzenti {
    private:
        FILE *file;
    public:
        /** construct */
        Marss(FILE *f) {
            file = f;
        };

        /** destruct */
        ~Marss() {};
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("recenzenti.in", "r");
	outputFile = fopen("recenzenti.out", "w+");

    Recenzenti *recenzenti = new Recenzenti(outputFile);

    char command[10];

    while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;

        if (command[0] == 'I') {

        }
        else if (command[0] == 'D') {

        }
        else if (command[0] == 'L') {

        }
    }

    fclose(inputFile);
    fclose(outputFile);

    marss->~Recenzenti();
	return 0;
}
