#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct Pse;
struct Rec;

struct Pse {
    long int id;
    Pse *left, *right;
    Rec *rec;

    Pse(int x, Rec *r) {
        id = x;
        rec = r;
        left = NULL;
        right = NULL;
    };
};

struct Rec {
    char name[101];
    Pse *pse[50];
    int count;

    Rec *next;
    Rec *prev;

    Rec (char *n) {
        strcpy(name, n);
        count = 0;
        next = NULL;
        prev = NULL;
        for (int i=0; i<50; i++) pse[i] = NULL;
    }
};

class Recenzenti {
    private:
        FILE *file;
        Rec *firstRecNode;
    public:
        Pse *firstPseNode;

        /** construct */
        Recenzenti(FILE *f) {
            file = f;
            firstRecNode = NULL;
            firstPseNode = NULL;
        };

        /** destruct */
        ~Recenzenti() {};

        /** Accept action message */
        void printOk() {
            fprintf(file, "ok\n");
        };

        /** False action message */
        void printNo() {
            fprintf(file, "no\n");
        };

        /** Pseido rec name */
        void printRecName(char *name) {
            fprintf(file, "%s\n", name);
        };

        /** Find pseido and return rec pointer */
        Rec* lookUp(int pseId, Pse *pseNode) {
            if (pseNode == NULL) {
                return NULL;
            }
            if (pseId == pseNode->id) {
                return pseNode->rec;
            } else {
                if (pseId < pseNode->id) {
                    return lookUp(pseId, pseNode->left);
                } else {
                    return lookUp(pseId, pseNode->right);
                }
            }
        };

        /** Find rec by name and returns its pointer */
        Rec* findRecByName(char *n) {
            if (firstRecNode == NULL) {
                firstRecNode = new Rec(n);
                return firstRecNode;
            }

            Rec *r = firstRecNode;
            Rec *save;
            while (r != NULL) {
                if (strcmp(r->name, n) == 0) {
                    return r;
                }

                if (r->next == NULL) save = r;

                r = r->next;
            }

            save->next = new Rec(n);
            save->next->prev = save;
            return save->next;
        };

        /** Insert pseido node in pseido binary tree */
        Pse* insertPseNode(Rec *rec, int id) {
            if (firstPseNode == NULL) {
                firstPseNode = new Pse(id, rec);
                return firstPseNode;
            }

            Pse *pse = firstPseNode;

            while (pse != NULL) {
                if (id < pse->id) {
                    if (pse->left == NULL) {
                        pse->left = new Pse(id, rec);
                        return pse->left;
                    }
                    pse = pse->left;
                } else {
                    if (pse->right == NULL) {
                        pse->right = new Pse(id, rec);
                        return pse->right;
                    }
                    pse = pse->right;
                }
            }
        };

        /** Find and deletes all pse nodes by using rec pse node array and then deletes rec list element */
        void invokeDelete(Rec *rec) {
            Pse **pIds = rec->pse;
            for (int i=0; i<50; i++) {
                if (pIds[i] != NULL) {
                    deletePseNode(pIds[i]);
                }
            }

            if (rec->prev != NULL) {
                rec->prev = rec->next;
                if (rec->next != NULL) {
                    rec->next->prev = rec->prev;
                }
            } else {
                firstRecNode = rec->next;
                firstRecNode->prev = NULL;
            }
            delete rec;
        };

        void deletePseNode(Pse *p) {
            if (p->right == NULL ) {
                p = p->left;
            } else {
                Pse *q = p->right;
                while (q->left != NULL) {
                    q = q->left;
                }
                p = q;
                q = q->right;
                q->left = p->left;
                q->right = p->right;
            }
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("recenzenti.in", "r");
	outputFile = fopen("recenzenti.out", "w+");

    Recenzenti *recenzenti = new Recenzenti(outputFile);

    char command[10];
    char name[101];

    int check, pseidoCount, pseidoId;

    bool existingKey = false, alreadyKeyInarray = false;
    int givenKeys[50];

    while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;

        if (command[0] == 'I') {
            // Reset holder data
            for (int i=0; i<50; i++) givenKeys[i] = 0;
            existingKey = false;

            fscanf(inputFile, "%s %d", name, &pseidoCount);

            for (int i=0; i<pseidoCount; i++) {
                fscanf(inputFile, "%d", &pseidoId);

                for (int i=0; i<50; i++) {
                    if (givenKeys[i] == pseidoId) {
                        alreadyKeyInarray = true;
                        break;
                    }
                }

                if (!alreadyKeyInarray) {
                    givenKeys[i] = pseidoId;
                }

                if (!existingKey && !alreadyKeyInarray) {
                    Rec *r = recenzenti->lookUp(pseidoId, recenzenti->firstPseNode);

                    if (r != NULL && strcmp(r->name, name) != 0) {
                        recenzenti->printNo();
                        existingKey = true;
                    }
                }

                alreadyKeyInarray = false;
            }

            if (!existingKey) {
                Rec *r2 = recenzenti->findRecByName(name);

                for (int i=0; i<50; i++) {
                    if (givenKeys[i] != 0)  {
                        r2->pse[r2->count] =  recenzenti->insertPseNode(r2, givenKeys[i]);
                        r2->count++;
                    }
                }

                recenzenti->printOk();
            }
        }
        else if (command[0] == 'D') {
            Rec *p = recenzenti->lookUp(pseidoId, recenzenti->firstPseNode);
            if (p == NULL) {
                recenzenti->printNo();
            } else {
                recenzenti->invokeDelete(p);
                recenzenti->printOk();
            }
        }
        else if (command[0] == 'L') {
            fscanf(inputFile, "%d", &pseidoId);
            recenzenti->printRecName(recenzenti->lookUp(pseidoId, recenzenti->firstPseNode)->name);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    recenzenti->~Recenzenti();
	return 0;
}
