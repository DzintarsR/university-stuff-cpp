#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct List {
    int money, id;
    List *next;
    List() {next=NULL; id=0;};
};

class moneyData {
    private:
        List *reg, *regf;
        int regId;
    public:
        moneyData() {
            regf = reg = new List;
            regId=0;
        };
        ~moneyData() {
            List *p;
            while (regf!=NULL) {
                p=regf;
                regf=regf->next;
                delete p;
            }
        };
        void addMember(int m, int &i) {
            regId++;
            if (regf->id == 0) {
                reg->money = m;
                reg->id = regId;
            } else {
                List *p=regf, *pbf=NULL;
                while (p!=NULL) {
                    if (m < p->money) {
                        if (p==regf) {
                            reg = new List;
                            reg->money = m;
                            reg->id = regId;
                            reg->next = regf;
                            regf=reg;
                        } else {
                            pbf->next = new List;
                            pbf->next->money = m;
                            pbf->next->id = regId;
                            pbf->next->next = p;
                        }
                        break;
                    }
                    else if (m == p->money && (p->next!=NULL && p->next->money != p->money)) {
                        List *s=p->next;
                        p->next = new List;
                        p->next->money = m;
                        p->next->id = regId;
                        p->next->next = s;
                        break;
                    }
                    else if (p->next == NULL) {
                        p->next = new List;
                        p->next->money = m;
                        p->next->id = regId;
                        break;
                    }
                    pbf=p;
                    p=p->next;
                }
            }

            i=regId;
        };

        List *getList() {
            return regf;
        };

        void payOut(int &i) {
            i = regf->id;
            if (regf->id != 0) {
                List *p=regf;
                regf = regf->next;
                delete p;
                if (regf==NULL) {
                    regf = reg = new List;
                }
            }
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("atbalsts.in", "r");
    //inputFile = fopen("unix-tests/atbalsts.i1", "r");
	outputFile = fopen("atbalsts.out", "w+");

	int paymentSize, rowSize, money, check, regId;
	int regisMembers=0;
	int payedOut=0;
	char command[10];

	moneyData *listClass = new moneyData;

	// Dabun globalo info
	fscanf(inputFile, "%d %d", &paymentSize, &rowSize);

	while (inputFile) {
        check = fscanf(inputFile, "%s %d", command, &money);
        if (check==EOF) break;

        if (payedOut < paymentSize) {
            if (command[0] == 'R') {
                if (regisMembers < rowSize) {
                    if (money < 2000) {
                        listClass->addMember(money, regId);
                        fprintf(outputFile, "%d %d\n", regId, money);
                        regisMembers++;
                        payedOut++;
                    } else {
                        fprintf(outputFile, "0 %d\n", money);
                    }
                } else {
                    fprintf(outputFile, "0 %d\n", money);
                }
            }
            else if (command[0] == 'M') {
                listClass->payOut(regId);
                fprintf(outputFile, "%d %d\n", regId, money);
                regisMembers--;
            }
        } else {
            fprintf(outputFile, "0 %d\n", money);
        }
	}

	return 0;
}
