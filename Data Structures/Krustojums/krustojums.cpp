#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct Auto {
    int startTime;
    int driveTime;
    int id;
    char rIn[10], rOut[10];
    Auto *next;
    Auto() {next=NULL;}
};

struct Lukso {
    int start, interval;
    Lukso *next;
    Lukso() {next=NULL;}
};

int main() {
    FILE* inputFile;
	FILE* outputFile;

    int time, id, interval;
    int endSimulation, luksoTime=0, goingSimulation=0, roadWay=0;
    char command[10];
    char rin[10];
    char rout[10];

    Auto *a = new Auto, *fAuto = a, *bla;
    Lukso *l = new Lukso, *fLukso = l, *bll;

	 // Load files
    inputFile = fopen("unix-tests/krustojums.i8", "r");
	outputFile = fopen("krustojums.out", "w+");

    // Get data and save
    while (inputFile) {
        fscanf(inputFile, "%d %s %d", &time, command, &id);
        if (command[0] == 'I') {
            if (luksoTime == 0) { luksoTime=id; }
            else {
                l->start = time;
                l->interval = id;
                bll = l;
                l = l->next = new Lukso;
            }
        }

        else if (command[0] == 'A') {
            fscanf(inputFile, "%s %s %d", rin, rout, &interval);
            a->startTime = time;
            a->id = id;
            strcpy(a->rIn, rin);
            strcpy(a->rOut, rout);
            a->driveTime = interval;
            bla = a;
            a = a->next = new Auto;
        }

        if (command[0] == 'F') {
            endSimulation = time;
            break;
        }
    }
    bll->next = NULL;
    bla->next = NULL;

    int tOut;

    if (fAuto->rIn[0] == 'N') fprintf(stdout, "%s \n", fAuto->rIn);


    while (goingSimulation <= endSimulation) {
        if (roadWay == 0) {
            Auto *p = fAuto;
            while (p!=NULL) {
                if (p->rIn[0] == 'N' || p->rIn[0] == 'S') {

                    if (tOut <= luksoTime) fprintf(outputFile, "%d %s %d\n", tOut, p->rOut, p->id);

                }
                p = p->next;
            }
        }

        if (roadWay == 1) {

        }
    }


    /*
    while (fAuto!=NULL) {
        fprintf(stdout, "%d %d %s %s %d \n", fAuto->startTime, fAuto->id, fAuto->rIn, fAuto->rOut, fAuto->driveTime);
        fAuto = fAuto->next;
    }

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
