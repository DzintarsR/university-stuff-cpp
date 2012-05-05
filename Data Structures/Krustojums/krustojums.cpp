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

class setData {
    private:
        Auto *ah, *ahf, *av, *avf;
        Lukso *lks, *lksf;
        int ch, cv, lc;
    public:
        setData() {
            ahf = ah = new Auto;
            avf = av = new Auto;
            lksf = lks = new Lukso;
            ch=0; cv=0, lc=0;
        };

        ~setData() {
            Auto *p;
            while (ahf!=NULL) {
                p=ahf; ahf=ahf->next;
                delete p;
            }
            while (avf!=NULL) {
                p=avf; avf=avf->next;
                delete p;
            }
            p=NULL;
        };

        Auto *getHAuto() {
            if (ahf!=NULL && ahf->id) return ahf;
            else return NULL;
        };

        Auto *getVAuto() {
            if (avf!=NULL && avf->id) return avf;
            else return NULL;
        };

        Auto *dropAutoH(Auto *a) {
            Auto *p=ahf;
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

        Auto *dropAutoV(Auto *a) {
            Auto *p=avf;
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
                    if (ch!=0) ah = ah->next = new Auto;
                    ah->startTime = st;
                    ah->id = id;
                    strcpy(ah->rIn, ri);
                    strcpy(ah->rOut, ro);
                    ah->driveTime = dt;
                    ch++;
                    break;
                case 1:
                    if (cv!=0) av = av->next = new Auto;
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

    Auto *fAuto_H;
    Auto *fAuto_V;
    Auto *faH=NULL;
    Auto *faV=NULL;

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
                            fprintf(outputFile, "%d", simulationTimeN + fAuto_H->driveTime + extraTime);
                            faH = simData->getHAuto();
                            while (faH!=NULL) {
                                if (faH->rIn[0] == 'S') {
                                    if (simulationTimeN + faH->driveTime + extraTime == simulationTimeN + fAuto_H->driveTime + extraTime) {
                                        fprintf(outputFile, " %s %d", faH->rOut, faH->id);
                                        faH = simData->dropAutoH(faH);
                                    }
                                    break;
                                }
                                faH = faH->next;
                            }
                            fprintf(outputFile, " %s %d\n", fAuto_H->rOut, fAuto_H->id);
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
                            fprintf(outputFile, "%d", fAuto_H->startTime+fAuto_H->driveTime);
                            fprintf(outputFile, " %s %d", fAuto_H->rOut, fAuto_H->id);
                            faH = simData->getHAuto();
                            int ct1=0;
                            while (faH!=NULL) {
                                if (faH->rIn[0] == 'N') {
                                    if (faH->startTime+faH->driveTime == fAuto_H->startTime+fAuto_H->driveTime) {
                                        fprintf(outputFile, " %s %d\n", faH->rOut, faH->id);
                                        faH = simData->dropAutoH(faH); ct1++;
                                    }
                                    break;
                                }
                                faH = faH->next;
                            }
                            if (ct1==0) fprintf(outputFile, "\n");
                            simulationTimeS = fAuto_H->startTime+fAuto_H->driveTime;
                            fAuto_H = simData->dropAutoH(fAuto_H);
                        } else { sTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (sMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeS + fAuto_H->driveTime + extraTime <= goTime+luksoTime && simulationTimeS+extraTime < endSimulation) {
                            fprintf(outputFile, "%d", simulationTimeS + fAuto_H->driveTime + extraTime);
                            fprintf(outputFile, " %s %d", fAuto_H->rOut, fAuto_H->id);
                            faH = simData->getHAuto();
                            int ct2=0;
                            while (faH!=NULL) {
                                if (faH->rIn[0] == 'N') {
                                    if (simulationTimeS + faH->driveTime + extraTime == simulationTimeS + fAuto_H->driveTime + extraTime) {
                                        fprintf(outputFile, " %s %d\n", faH->rOut, faH->id);
                                        faH = simData->dropAutoH(faH); ct2++;
                                    }
                                    break;
                                }
                                faH = faH->next;
                            }
                            if (ct2==0) fprintf(outputFile, "\n");
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
                        if (fAuto_V->startTime + fAuto_V->driveTime <= simulationTimeW+luksoTime && fAuto_V->startTime < endSimulation) {
                            fprintf(outputFile, "%d", fAuto_V->startTime+fAuto_V->driveTime);
                            faV = simData->getVAuto();
                            int ct3=0;
                            while (faV!=NULL) {
                                if (faV->rIn[0] == 'E') {
                                    if (faV->startTime+faV->driveTime == fAuto_V->startTime+fAuto_V->driveTime) {
                                        ct3++;
                                    }
                                    break;
                                }
                                faV = faV->next;
                            }
                            if (ct3!=0) {
                                if (faV->rOut[0]=='N') {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                                } else {
                                    fprintf(outputFile, " %s %d", fAuto_V->rOut, fAuto_V->id);
                                    fprintf(outputFile, " %s %d\n", faV->rOut, faV->id);
                                }
                                faV = simData->dropAutoV(faV);
                            } else {
                                fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                            }

                            simulationTimeW = fAuto_V->startTime+fAuto_V->driveTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { wTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (wMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeW + fAuto_V->driveTime + extraTime <= goTime+luksoTime && simulationTimeW+extraTime < endSimulation) {
                            fprintf(outputFile, "%d", simulationTimeW + fAuto_V->driveTime + extraTime);
                            faV = simData->getVAuto();
                            int ct4=0;
                            while (faV!=NULL) {
                                if (faV->rIn[0] == 'E') {
                                    if (simulationTimeW + faV->driveTime + extraTime == simulationTimeW + fAuto_V->driveTime + extraTime) {
                                        ct4++;
                                    }
                                    break;
                                }
                                faV = faV->next;
                            }
                            if (ct4!=0) {
                                if (faV->rOut[0]=='N') {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                                } else {
                                    fprintf(outputFile, " %s %d", fAuto_V->rOut, fAuto_V->id);
                                    fprintf(outputFile, " %s %d\n", faV->rOut, faV->id);
                                }
                                faV = simData->dropAutoV(faV);
                            } else {
                                fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                            }

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
                        if (fAuto_V->startTime + fAuto_V->driveTime <= simulationTimeE+luksoTime && fAuto_V->startTime < endSimulation) {
                            fprintf(outputFile, "%d", fAuto_V->startTime+fAuto_V->driveTime);
                            faV = simData->getVAuto();
                            int ct5=0;
                            while (faV!=NULL) {
                                if (faV->rIn[0] == 'W') {
                                    if (faV->startTime+faV->driveTime == fAuto_V->startTime+fAuto_V->driveTime) {
                                        ct5++;
                                    }
                                    break;
                                }
                                faV = faV->next;
                            }
                            if (fAuto_V->rOut[0]=='N') {
                                fprintf(outputFile, " %s %d", fAuto_V->rOut, fAuto_V->id);
                                if (ct5!=0) {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    faV = simData->dropAutoV(faV);
                                }
                                fprintf(outputFile, "\n");
                            } else {
                                if (ct5!=0) {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    faV = simData->dropAutoV(faV);
                                }
                                fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                            }

                            simulationTimeE = fAuto_V->startTime+fAuto_V->driveTime;
                            fAuto_V = simData->dropAutoV(fAuto_V);
                        } else { eTimes=1; }
                    } else {
                        // Ja simulacija jau ir cela
                        if (eMove == 0 && goTime!=0) extraTime=1;
                        else extraTime=0;
                        if (simulationTimeE + fAuto_V->driveTime + extraTime <= goTime+luksoTime && simulationTimeE+extraTime < endSimulation) {
                            fprintf(outputFile, "%d", simulationTimeE + fAuto_V->driveTime + extraTime);
                            faV = simData->getVAuto();
                            int ct6=0;
                            while (faV!=NULL) {
                                if (faV->rIn[0] == 'W') {
                                    if (simulationTimeE + faV->driveTime + extraTime == simulationTimeE + fAuto_V->driveTime + extraTime) {
                                        ct6++;
                                    }
                                    break;
                                }
                                faV = faV->next;
                            }
                            if (fAuto_V->rOut[0]=='N') {
                                fprintf(outputFile, " %s %d", fAuto_V->rOut, fAuto_V->id);
                                if (ct6!=0) {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    faV = simData->dropAutoV(faV);
                                }
                                fprintf(outputFile, "\n");
                            } else {
                                if (ct6!=0) {
                                    fprintf(outputFile, " %s %d", faV->rOut, faV->id);
                                    faV = simData->dropAutoV(faV);
                                }
                                fprintf(outputFile, " %s %d\n", fAuto_V->rOut, fAuto_V->id);
                            }

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

