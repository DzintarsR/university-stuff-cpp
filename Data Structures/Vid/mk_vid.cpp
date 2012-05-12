#include "string.h"
#include "stdio.h"
#include "stdlib.h"

// Ieliek un izņem pēc steka
struct Packet {
    int docId;
    Packet *next;
    Packet() {next=NULL;}
    Packet(int n) {next=NULL; docId=n;}
    Packet(int n, Packet *np) {next=np; docId=n;}
};

// Paketes uztur pēc rindas
struct PacketRow {
    int docs;
    Packet *firstDoc;
    PacketRow *next;
    PacketRow() {next=NULL;}
    PacketRow(int d, Packet *p) {docs=d; next=NULL; firstDoc=p; }
};

class packetHandler {
    private:
        PacketRow *firstP, *newestP;
        Packet *currentP;
        int docsInCurrentP;
    public:
        packetHandler() {
            newestP = firstP = NULL;
            currentP = NULL;
            docsInCurrentP=0;
        };
        ~packetHandler() {
            Packet *a, *ca;
            PacketRow *p=firstP, *cp;
            while (p != NULL) {
                a = p->firstDoc;
                while (a!=NULL) {
                    ca = a; a=a->next;
                    delete ca;
                }
                cp = p; p=p->next;
                delete cp;
            }
        };
        void createList(int pId) {
            if (currentP == NULL) {
                currentP = new Packet(pId);
            } else {
                Packet *cp;
                cp = new Packet(pId, currentP);
                currentP = cp;
            }
            docsInCurrentP++;
        };
        void addDoc(int pId) {
            if (pId != 0) createList(pId);
            else {
                if (firstP == NULL) {
                    newestP = firstP = new PacketRow(docsInCurrentP, currentP);
                } else {
                    PacketRow *pc;
                    pc = new PacketRow(docsInCurrentP, currentP);
                    newestP->next = pc;
                    newestP = pc;
                }
                docsInCurrentP=0;
                currentP=NULL;
            }
        };
};

int main() {
    FILE* inputFile;
    FILE* outputFile;
    inputFile = fopen("mk_vid.in", "r");
	outputFile = fopen("mk_vid.out", "w+");

	packetHandler *pack = new packetHandler;

    char command[10];
	int check, doc;

	while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;

        if (command[0] == 'P') {
            while (inputFile) {
                fscanf(inputFile, "%d", &doc);
                pack->addDoc(doc);
                if (doc == 0) break;
            }
        }
	}

	return 0;
}
