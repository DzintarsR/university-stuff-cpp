#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct List {
    char command[10];
    int money;
    List *next;
};

class listData {
    private:
        List *fli, *li;
        int ct;
    public:
        listData() {
            fli = li = new List;
            ct=0;
        };

        void addCommand(char *c, int m) {
            if (ct!=0) li = li->next = new List;
            strcpy(li->command, c);
            li->money = m;
            ct++;
        };

        List *getList() {
            return fli;
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("atbalsts.in", "r");
    //inputFile = fopen("unix-tests/atbalsts.i1", "r");
	outputFile = fopen("atbalsts.out", "w+");

	int paymentSize, rowSize, money, a;
	char command[10];

	listData *listClass = new listData;

	// Dabun globalo info
	fscanf(inputFile, "%d %d", &paymentSize, &rowSize);

	while (inputFile) {
        a = fscanf(inputFile, "%s %d", command, &money);
        if (a==EOF) break;
        listClass->addCommand(command, money);
	}

	List *p=listClass->getList();
	while (p!=NULL) {
        fprintf(outputFile, "%s %d\n", p->command, p->money);
        p = p->next;
	}

	return 0;
}
