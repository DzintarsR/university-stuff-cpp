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

        void changeTime(int t, int  &lt) {
            if (lksf==NULL) lt=lt;
            Lukso *p=lksf;
            int newi=0;
            while (p!=NULL) {
                if (p->start <= t) {
                    newi=p->interval;
                    lksf=p->next;
                    delete p;
                    p=lksf;
                } else p=p->next;
            }
            if (newi==0) lt=lt;
            else lt=newi;
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
    inputFile = fopen("unix-tests/krustojums.i4", "r");
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
    Auto_H *faH;
    Auto_V *faV;

    int goTime=0;
    int nMove=0, sMove=0, wMove=0, eMove=0;
    int simulationTimeN;
    int simulationTimeS;
    int simulationTimeW;
    int simulationTimeE;

    int nTimes=0;
    int sTimes=0;
    int wTimes=0;
    int eTimes=0;
    int extraTime;

    // Start simulation
    do {
        fAuto_H = simData->getHAuto();
        fAuto_V = simData->getVAuto();
        nMove=0; sMove=0; wMove=0; eMove=0;


        // ziemelu dienvidu
        if (roadWay == 0) {
            nTimes=0;
            sTimes=0;
            simulationTimeN=goTime;
            simulationTimeS=goTime;
            while (fAuto_H!=NULL) {
                // Ja brauc no ziemeliem
                if (fAuto_H!=NULL && fAuto_H->rIn[0] == 'N' && nTimes==0) {
                    if (simulationTimeN < fAuto_H->startTime) {
                        // Ja simulacija iet no sakuma
                        if (fAuto_H->startTime + fAuto_H->driveTime <= luksoTime && fAuto_H->startTime < endSimulation) {
                            fprintf(outputFile, "%d", fAuto_H->startTime+fAuto_H->driveTime);
                            faH = simData->getHAuto();
                            while (faH!=NULL) {
                                if (faH->rIn[0] == 'S') {
                                    if (faH->startTime+faH->driveTime == fAuto_H->startTime+fAuto_H->driveTime) {
                                        fprintf(outputFile, " %s %d", faH->rOut, faH->id);
                                        faH = simData->dropAutoH(faH);
                                    }
                                    break;
                                }
                                faH = faH->next;
                            }
                            fprintf(outputFile, " %s %d\n", fAuto_H->rOut, fAuto_H->id);
                            simulationTimeN = fAuto_H->startTime+fAuto_H->driveTime;
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else { nTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (nMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeN + fAuto_H->driveTime + extraTime <= goTime+luksoTime && simulationTimeN+extraTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", simulationTimeN + fAuto_H->driveTime + extraTime, fAuto_H->rOut, fAuto_H->id);
                            simulationTimeN += fAuto_H->driveTime+extraTime;
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else { nTimes=1; }
                    }
                    nMove++;
                } else { if (nTimes==1 && fAuto_H!=NULL) fAuto_H=fAuto_H->next; }

                // Ja brauc no dienvidiem
                if (fAuto_H!=NULL && fAuto_H->rIn[0] == 'S' && sTimes==0) {
                    if (simulationTimeS < fAuto_H->startTime) {
                        // Ja simulacija iet no sakuma
                        if (fAuto_H->startTime + fAuto_H->driveTime <= luksoTime && fAuto_H->startTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", fAuto_H->startTime+fAuto_H->driveTime, fAuto_H->rOut, fAuto_H->id);
                            simulationTimeS = fAuto_H->startTime+fAuto_H->driveTime;
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else { sTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (sMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeS + fAuto_H->driveTime + extraTime <= goTime+luksoTime && simulationTimeS+extraTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", simulationTimeS + fAuto_H->driveTime + extraTime, fAuto_H->rOut, fAuto_H->id);
                            simulationTimeS += fAuto_H->driveTime+extraTime;
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else { sTimes=1; }
                    }
                    sMove++;
                } else { if (sTimes==1 && fAuto_H!=NULL) fAuto_H=fAuto_H->next; }
            }
        }


        // Austrumu rietumu
        if (roadWay == 1) {
            wTimes=0;
            eTimes=0;
            simulationTimeW=goTime;
            simulationTimeE=goTime;
            while (fAuto_V!=NULL) {
                // Ja brauc no rietumiem

                if (fAuto_V!=NULL && fAuto_V->rIn[0] == 'W' && wTimes==0) {
                    if (simulationTimeW < fAuto_V->startTime) {
                        // Ja simulacija iet no sakuma
                        if (fAuto_V->startTime + fAuto_V->driveTime <= luksoTime && fAuto_V->startTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", fAuto_V->startTime+fAuto_V->driveTime, fAuto_V->rOut, fAuto_V->id);
                            simulationTimeW = fAuto_V->startTime+fAuto_V->driveTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { wTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (wMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeW + fAuto_V->driveTime + extraTime <= goTime+luksoTime && simulationTimeW+extraTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", simulationTimeW + fAuto_V->driveTime + extraTime, fAuto_V->rOut, fAuto_V->id);
                            simulationTimeW += fAuto_V->driveTime+extraTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { wTimes=1; }
                    }
                    wMove++;
                } else { if (wTimes==1 && fAuto_V!=NULL) fAuto_V=fAuto_V->next; }

                // Ja brauc no austrumiem
                if (fAuto_V!=NULL && fAuto_V->rIn[0] == 'E' && eTimes==0) {
                    if (simulationTimeE < fAuto_V->startTime) {
                        // Ja simulacija iet no sakuma
                        if (fAuto_V->startTime + fAuto_V->driveTime <= luksoTime && fAuto_V->startTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", fAuto_V->startTime+fAuto_V->driveTime, fAuto_V->rOut, fAuto_V->id);
                            simulationTimeE = fAuto_V->startTime+fAuto_V->driveTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { eTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (eMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeE + fAuto_V->driveTime + extraTime <= goTime+luksoTime && simulationTimeE+extraTime < endSimulation) {
                            fprintf(outputFile, "%d %s %d\n", simulationTimeE + fAuto_V->driveTime + extraTime, fAuto_V->rOut, fAuto_V->id);
                            simulationTimeE += fAuto_V->driveTime+extraTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { eTimes=1; }
                    }
                    eMove++;
                } else { if (eTimes==1 && fAuto_V!=NULL) fAuto_V=fAuto_V->next; }
            }
        }


        goTime += luksoTime;
        simData->changeTime(goTime, luksoTime);

        roadWay = (roadWay==0 ? 1 : 0);
    } while (goTime < endSimulation);

    fprintf(outputFile, "%s", "stop");

    fclose(inputFile);
    fclose(outputFile);
	return 0;
}
