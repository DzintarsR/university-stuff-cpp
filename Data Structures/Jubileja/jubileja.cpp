#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define MONTH 12
#define DAY 31

struct DateTime {
    int day, month, year;
    char name[31], surname[31];
    DateTime *next;

    DateTime() {
        next = NULL;
    };

    void setDateTimeData(int d, int m, int y, char *na, char *su, DateTime *n=NULL) {
        day = d;
        month = m;
        year = y;
        next = n;
        strcpy(name, na);
        strcpy(surname, su);
    };
};

class Jubileja {
    private:
        FILE *file;
        DateTime*** datetimeList;
    public:
        /** construct */
        Jubileja(FILE *f) {
            file = f;
            datetimeList = new DateTime**[MONTH];

            for (int i=0; i<MONTH; i++) datetimeList[i] = NULL;
        };

        ~Jubileja() {};

        /** get day for insert */
        DateTime* getDayForInsert(int month, int day) {
            if (datetimeList[month] == NULL) {
                datetimeList[month] = new DateTime*[DAY];
                for (int i=0; i<DAY; i++) datetimeList[month][i] = NULL;
            }

            if (datetimeList[month][day] == NULL) {
                datetimeList[month][day] = new DateTime;
            }

            return datetimeList[month][day];
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("jubileja.in", "r");
	outputFile = fopen("jubileja.out", "w+");

    Jubileja *jubileja = new Jubileja(outputFile);

    char command[10];
    char name[31], surname[31];

    int check;

    while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;
    }

    fclose(inputFile);
    fclose(outputFile);

    jubileja->~Jubileja();
	return 0;
}
