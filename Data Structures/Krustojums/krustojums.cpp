#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct Auto_H {
    int startTime;
    int driveTime;
    int id;
    char rIn[10], rOut[10];
    Auto_H *next;
    Auto_H() {next=NULL;}
};

struct Auto_V {
    int startTime;
    int driveTime;
    int id;
    char rIn[10], rOut[10];
    Auto_V *next;
    Auto_V() {next=NULL;}
};

struct Lukso {
    int start, interval;
    Lukso *next;
    Lukso() {next=NULL;}
};

class setData {
    private:
        Auto_H *ah, *ahf;
        Auto_V *av, *avf;
        Lukso *lks;
        int ch, cv, lc;
    public:
        setData() {
            ahf = ah = new Auto_H;
            avf = av = new Auto_V;
            lks = new Lukso;
            ch=0; cv=0, lc=0;
        };

        Auto_H *getHAuto() {
            return ahf;
        };

        Auto_V *getVAuto() {
            return avf;
        };

        void setAuto(int way, int st, int dt, int id, char *ri, char *ro) {
            switch (way) {
                case 0:
                    if (ch!=0) ah = ah->next = new Auto_H;
                    ah->startTime = st;
                    ah->id = id;
                    strcpy(ah->rIn, ri);
                    strcpy(ah->rOut, ro);
                    ah->driveTime = dt;
                    ch++;
                    break;
                case 1:
                    if (cv!=0) av = av->next = new Auto_V;
                    av->startTime = st;
                    av->id = id;
                    strcpy(av->rIn, ri);
                    strcpy(av->rOut, ro);
                    av->driveTime = dt;
                    cv++;
                    break;
            }
        };

        void setLukso(int st, int it) {
            if (lc!=0) lks = lks->next = new Lukso;
            lks->start = st;
            lks->interval = it;
            lc++;
        };
};

int main() {
    FILE* inputFile;
	FILE* outputFile;

    int time, id, interval;
    int endSimulation, luksoTime=0, roadWay=0;
    char command[10];
    char rin[10];
    char rout[10];

    setData *simData = new setData;

	 // Load files
    inputFile = fopen("unix-tests/krustojums.i8", "r");
	outputFile = fopen("krustojums.out", "w+");

    // Get data and save
    while (inputFile) {
        fscanf(inputFile, "%d %s %d", &time, command, &id);
        // Set traffic lights data
        if (command[0] == 'I') {
            if (luksoTime == 0) { luksoTime=id; }
            else {
                simData->setLukso(time, id);
            }
        }
        // Set auto data
        else if (command[0] == 'A') {
            fscanf(inputFile, "%s %s %d", rin, rout, &interval);
            if (rin[0] == 'N' || rin[0] == 'S')
                simData->setAuto(0, time, interval, id, rin, rout);
            else
                simData->setAuto(1, time, interval, id, rin, rout);
        }
        // Set simulation end data
        if (command[0] == 'F') {
            endSimulation = time;
            break;
        }
    }



    Auto_H *fAuto_H;
    fAuto_H = simData->getHAuto();

    while (fAuto_H!=NULL) {
        fprintf(stdout, "%d %d %s %s %d \n", fAuto_H->startTime, fAuto_H->id, fAuto_H->rIn, fAuto_H->rOut, fAuto_H->driveTime);
        fAuto_H = fAuto_H->next;
    }

    Auto_V *fAuto_V;
    fAuto_V = simData->getVAuto();

    while (fAuto_V!=NULL) {
        fprintf(stdout, "%d %d %s %s %d \n", fAuto_V->startTime, fAuto_V->id, fAuto_V->rIn, fAuto_V->rOut, fAuto_V->driveTime);
        fAuto_V = fAuto_V->next;
    }


    /*
    while (fLukso!=NULL) {
        fprintf(stdout, "%d %d \n", fLukso->start, fLukso->interval);
        fLukso = fLukso->next;
    }

    fprintf(stdout, "%d \n", endSimulation);
    */


    fclose(inputFile);
	fclose(outputFile);
	return 0;
}
