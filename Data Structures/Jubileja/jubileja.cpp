#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

#define MONTH 13
#define DAY 32

struct DateTime {
    int day, month, year;
    char name[31], surname[31];
    DateTime *next;

    DateTime() {
        next = NULL;
        year = 0;
    };

    DateTime(int d, int m, int y, char *na, char *su, DateTime *n=NULL) {
        setDateTimeData(d, m, y, na, su, NULL);
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

        void testPrint(int x) {
            fprintf(stdout, "%d\n", x);
        };

        /** converts char array to lower cases */
        char* charToLowerCase(char *str) {
            int i = 0;
            while (str[i]) {
                str[i] = tolower(str[i]);
                i++;
            }
            return str;
        };

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

        /** first surname+name in alphabet */
        int firstInAlphabet(char *gn, char *gs, char *cm, char *cs) {
             gn = charToLowerCase(gn);
             gs = charToLowerCase(gs);
             cm = charToLowerCase(cm);
             cs = charToLowerCase(cs);
             int s = strcmp(gs, cs);
             return s != 0 ? s : strcmp(gn, cm);
        };

        /** insert new member */
        void addNewMember(int day, int month, int year, char *name, char *surname) {
            DateTime *firstElem = getDayForInsert(month, day);
            if (firstElem->year == 0) {
                firstElem->setDateTimeData(day, month, year, name, surname);
                return;
            }

            DateTime *p = firstElem;
            DateTime *pBefore = NULL;
            DateTime *saveP = NULL;
            while (p != NULL) {
                if (p == firstElem && year < p->year) {
                    datetimeList[month][day] = new DateTime(day, month, year, name, surname, firstElem);
                    break;
                }

                else if (year > p->year) {
                    if (p->next == NULL) {
                        p->next = new DateTime(day, month, year, name, surname);
                        break;
                    }
                    else if (p->next != NULL && year < p->next->year) {
                        saveP = p;
                        p->next = new DateTime(day, month, year, name, surname, saveP);
                        saveP = NULL;
                        break;
                    }
                }

                else if (year == p->year) {
                    // d < 0 than name, else p->name
                    int d = firstInAlphabet(name, surname, p->name, p->surname);
                    if (d < 0) {
                        if (pBefore != NULL) {
                            saveP = pBefore->next;
                            pBefore->next = new DateTime(day, month, year, name, surname, saveP);
                            saveP = NULL;
                            break;
                        } else {
                            datetimeList[month][day] = new DateTime(day, month, year, name, surname, firstElem);
                            break;
                        }
                    }

                    else if (d > 0) {
                        if (p->next != NULL && p->next->year != year) {
                            saveP = p->next;
                            p->next = new DateTime(day, month, year, name, surname, saveP);
                            saveP = NULL;
                            break;
                        }
                    }
                }

                pBefore = p;
                p = p->next;
            }
        };

        /** search and print closest birthdays */
        void findClosestBirthday(int day, int month, int year) {
            DateTime *p = NULL;
            int age, hold = 0;
            for (int i=month; i<MONTH; i++) {
                if (datetimeList[i] != NULL) {
                    for (int j=day; j<DAY; j++) {
                        if (datetimeList[i][j] != NULL) {
                            p = datetimeList[i][j];
                            break;
                        }
                    }
                    break;
                }

                // year is over and no birthdays was found - reset year
                if (i == MONTH && p == NULL)  {
                    i = 0;
                    hold++;
                }
            }

            while (p != NULL) {
                age = year - p->year + hold;
                fprintf(file, "%s%d.%s%d.%d\n", addExtraZero(p->day), p->day, addExtraZero(p->day), p->month, year+hold);
                fprintf(file, "%d %s %s\n", age, p->name, p->surname);
                p = p->next;
            }
        };

        /** return 0 char if given value < 10*/
        char* addExtraZero(int x) {
            if (x < 10) return "0";
            return "";
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("jubileja.in", "r");
	outputFile = fopen("jubileja.out", "w+");

    Jubileja *jubileja = new Jubileja(outputFile);

    char command[10], name[31], surname[31];
    int check, day, month, year;

    while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;

        if (command[0] == 'P') {
            fscanf(inputFile, "%s %s %d.%d.%d", name, surname, &day, &month, &year);
            jubileja->addNewMember(day, month, year, name, surname);
        }

        if (command[0] == 'D') {
            fscanf(inputFile, "%d.%d.%d", &day, &month, &year);
            jubileja->findClosestBirthday(day, month, year);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    jubileja->~Jubileja();
	return 0;
}
