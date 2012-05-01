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
        Lukso *lks, *lksf;
        int ch, cv, lc;
    public:
        setData() {
            ahf = ah = new Auto_H;
            avf = av = new Auto_V;
            lksf = lks = new Lukso;
            ch=0; cv=0, lc=0;
        };

        Auto_H *getHAuto() {
            if (ahf!=NULL && ahf->id) return ahf;
            else return NULL;
        };

        Auto_V *getVAuto() {
            if (avf!=NULL && avf->id) return avf;
            else return NULL;
        };

        Lukso *getLukso() {
            return lksf;
        };

        Auto_H *dropAutoH(Auto_H *a) {
            Auto_H *p=ahf;
            while (p!=NULL) {
                if (p==a) {
                    ahf = p->next;
                    delete p;
                    return ahf;
                }
                if (p->next == a) {
                    p->next = p->next->next;
                    delete a;
                    return p->next;
                }
                p = p->next;
            }
            return NULL;
        };

        Auto_V *dropAutoV(Auto_V *a) {
            Auto_V *p=avf;
            while (p!=NULL) {
                if (p==a) {
                    avf = p->next;
                    delete p;
                    return avf;
                }
                if (p->next == a) {
                    p->next = p->next->next;
                    delete a;
                    return p->next;
                }
                p = p->next;
            }
            return NULL;
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
    int endSimulation, luksoTime, roadWay;
    char command[10];
    char rin[10];
    char rout[10];

    endSimulation=0;
    luksoTime=0;
    roadWay=0;

    setData *simData = new setData;

    // Load files
    // krustojums.in
    //inputFile = fopen("krustojums.in", "r");
    inputFile = fopen("unix-tests/krustojums.i5", "r");
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
    Auto_V *fAuto_V;
    // luksoTime
    int goTime=0;
    int nMove=0, sMove=0, wMove=0, eMove=0;
    int spe;
    int drt;

    // Start simulation
    do {
        fAuto_H = simData->getHAuto();
        fAuto_V = simData->getVAuto();

        // ziemelu dienvidu
        if (roadWay == 0) {
            while (fAuto_H!=NULL) {
                // Ja brauc no ziemeliem
                if (fAuto_H!=NULL && fAuto_H->rIn[0] == 'N') {
                    if (true) {
                        spe = fAuto_H->startTime+fAuto_H->driveTime;
                         if (goTime > spe) {
                            drt = goTime+fAuto_H->driveTime;
                            if (nMove == 0) drt++;
                        } else {
                            drt = (nMove>=spe ? goTime+nMove+fAuto_H->driveTime : goTime+spe );
                        }
                        if (spe < luksoTime) {
                            Auto_H *ph=fAuto_H->next;
                            fprintf(outputFile, "%d ", drt);
                            // parbauda vai taja pasa laika neskerso
                            while (ph!=NULL) {
                                if (ph->rIn[0] == 'S') {
                                    if (ph->startTime+ph->driveTime == spe) {
                                         fprintf(outputFile, "%s %d ", ph->rOut, ph->id);
                                         ph = simData->dropAutoH(ph);
                                         break;
                                    }
                                    if (ph->startTime+ph->driveTime > spe) break;
                                }
                                ph = ph->next;
                            }
                            fprintf(outputFile, "%s %d\n", fAuto_H->rOut, fAuto_H->id);
                            nMove += (nMove>=spe ? nMove+fAuto_H->driveTime : spe );
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else fAuto_H = fAuto_H->next;
                    }
                }

                // Ja brauc no dienvidiem
                if (fAuto_H!=NULL && fAuto_H->rIn[0] == 'S') {
                    if (true) {
                        spe = fAuto_H->startTime+fAuto_H->driveTime;
                        if (goTime > spe) {
                            drt = goTime+fAuto_H->driveTime;
                            if (sMove == 0) drt++;
                        } else {
                            drt = (sMove>=spe ? goTime+sMove+fAuto_H->driveTime : goTime+spe );
                        }
                        if (spe < luksoTime) {
                            Auto_H *ph=fAuto_H->next;
                            fprintf(outputFile, "%d ", drt);
                            // parbauda vai taja pasa laika neskerso
                            while (ph!=NULL) {
                                if (ph->rIn[0] == 'N') {
                                    if (ph->startTime+ph->driveTime == spe) {
                                         fprintf(outputFile, "%s %d ", ph->rOut, ph->id);
                                         ph = simData->dropAutoH(ph);
                                         break;
                                    }
                                    if (ph->startTime+ph->driveTime > spe) break;
                                }
                                ph = ph->next;
                            }
                            fprintf(outputFile, "%s %d\n", fAuto_H->rOut, fAuto_H->id);
                            sMove += (sMove>=spe ? sMove+fAuto_H->driveTime : spe );
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else fAuto_H = fAuto_H->next;
                    }
                }
            }
        }

        // Austrumu rietumu
        if (roadWay == 1) {
            while (fAuto_V!=NULL) {
                // Ja brauc no rietumiem
                if (fAuto_V!=NULL && fAuto_V->rIn[0] == 'W') {
                    if (true) {
                        spe = fAuto_V->startTime+fAuto_V->driveTime;
                        if (goTime > spe) {
                            drt = goTime+fAuto_V->driveTime;
                            if (wMove == 0) drt++;
                        } else {
                            drt = (wMove>=spe ? goTime+wMove+fAuto_V->driveTime : goTime+spe );
                        }
                        if (spe < luksoTime) {
                            Auto_V *ph=fAuto_V->next;
                            fprintf(outputFile, "%d ", drt);
                            // parbauda vai taja pasa laika neskerso
                            while (ph!=NULL) {
                                if (ph->rIn[0] == 'E') {
                                    if (ph->startTime+ph->driveTime == spe) {
                                         fprintf(outputFile, "%s %d ", ph->rOut, ph->id);
                                         ph = simData->dropAutoV(ph);
                                         break;
                                    }
                                    if (ph->startTime+ph->driveTime > spe) break;
                                }
                                ph = ph->next;
                            }
                            fprintf(outputFile, "%s %d\n", fAuto_V->rOut, fAuto_V->id);
                            wMove += (wMove>=spe ? wMove+fAuto_V->driveTime : spe );
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else fAuto_V = fAuto_V->next;
                    }
                }

                // Ja brauc no austrumiem
                if (fAuto_V!=NULL && fAuto_V->rIn[0] == 'E') {
                    if (true) {
                        spe = fAuto_V->startTime+fAuto_V->driveTime;
                        if (goTime > spe) {
                            drt = goTime+fAuto_V->driveTime;
                            if (eMove == 0) drt++;
                        } else {
                            drt = (eMove>=spe ? goTime+eMove+fAuto_V->driveTime : goTime+spe );
                        }
                        if (spe < luksoTime) {
                            Auto_V *ph=fAuto_V->next;
                            fprintf(outputFile, "%d ", drt);
                            // parbauda vai taja pasa laika neskerso
                            while (ph!=NULL) {
                                if (ph->rIn[0] == 'W') {
                                    if (ph->startTime+ph->driveTime == spe) {
                                         fprintf(outputFile, "%s %d ", ph->rOut, ph->id);
                                         ph = simData->dropAutoV(ph);
                                         break;
                                    }
                                    if (ph->startTime+ph->driveTime > spe) break;
                                }
                                ph = ph->next;
                            }
                            fprintf(outputFile, "%s %d\n", fAuto_V->rOut, fAuto_V->id);
                            eMove += (eMove>=spe ? eMove+fAuto_V->driveTime : spe );
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else fAuto_V = fAuto_V->next;
                    }
                }
            }
        }

        goTime += luksoTime;
        roadWay = (roadWay==0 ? 1 : 0);

    } while (goTime < endSimulation);

    fprintf(outputFile, "%s", "stop");

    fclose(inputFile);
    fclose(outputFile);
	return 0;
}
