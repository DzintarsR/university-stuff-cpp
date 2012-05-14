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
        int printFirstDoc() {
            return firstP->firstDoc->docId;
        };
        int printDocsInPacket() {
            return firstP->docs;
        };
        PacketRow *elPlacePointer(int place, PacketRow *p) {
            if (place == 1) return p;
            else if (place == 0) return NULL;
            return elPlacePointer(place-1, p->next);
        }
        void changePackets(int ch1, int ch2) {
            PacketRow *bfn, *n, *bfm, *m;
            int pn1, pn2;
            if (ch2 < ch1) { pn1=ch2; pn2=ch1; }
            else { pn1=ch1; pn2=ch2; }

            bfn = elPlacePointer(pn1-1, firstP);
            if (pn1==1) n=firstP;
            else n=bfn->next;
            bfm = elPlacePointer(pn2-1, firstP);
            m = bfm->next;

            if (firstP->next->next==NULL) {
                n->next=m->next;
                m->next=n;
                firstP=m;
            }
            else if (bfn==NULL) {
                PacketRow *nn=n->next;
                bfm->next = n;
                n->next=m->next;
                m->next = nn;
                if (m->next==m) m->next=n;
                firstP=m;
            } else {
                PacketRow *nn=n->next;
                bfn->next = m;
                bfm->next = n;
                n->next = m->next;
                m->next = nn;
                if (m->next==m) m->next=n;
            }
        };
        void deleteDocs(int del) {
            int hldl=del;
            if (firstP->docs > del) {
                Packet *fp=firstP->firstDoc, *fpc;
                while(fp!=NULL) {
                    if (del==0) break;
                    fpc=fp;
                    fp=fp->next;
                    delete fpc;
                    del--;
                }
                firstP->docs = firstP->docs-hldl;
                firstP->firstDoc = fp;
            }
            else if (firstP->docs <= del) {
                int dip=firstP->docs, delc=0;
                Packet *fp=firstP->firstDoc, *fpc;
                PacketRow *fpr;
                while(fp!=NULL) {
                    fpc=fp;
                    fp=fp->next;
                    delc++;
                    delete fpc;
                }
                fpr=firstP;
                firstP=firstP->next;
                delete fpr;

                if (del > dip) deleteDocs(del-delc);
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
	int check, doc, ch1, ch2, del;

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
        else if (command[0] == 'F') {
            fprintf(outputFile, "%d\n", pack->printFirstDoc());
        }
        else if (command[0] == 'C') {
            fprintf(outputFile, "%d\n", pack->printDocsInPacket());
        }
        else if (command[0] == 'E') {
            fscanf(inputFile, "%d %d", &ch1, &ch2);
            pack->changePackets(ch1, ch2);
        }
        else if (command[0] == 'D') {
            fscanf(inputFile, "%d", &del);
            pack->deleteDocs(del);
        }
	}

    fclose(inputFile);
    fclose(outputFile);
	return 0;
}
