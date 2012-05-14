/**********************
Kārlis Babris, kb11078
H8. Uzrakstīt programmu, kas ļauj izveidot un papildināt bināru failu, kura ieraksta
struktūra ir sekojoša: gads (int), mēnesis (int), diena (int), nākošā ieraksta numurs (int).
Programmai katrs jauns ieraksts jāieliek faila beigās, modelējot saistītu sarakstu,
kura ieraksti sakārtoti datumu pieaugšanas secībā. Paredzēt iespēju izdrukāt faila
ierakstus pēc fiziskā un pēc loģiskā sakārtojuma.
**********************/

#include <iostream>
#include <fstream>
using namespace std;

struct printSort {
    int size;
    printSort *next;
    printSort() {next=NULL; size=0;}
    printSort(int a) {size=a; next=NULL;}
};

class Vertiba {
    private:
        int year, month, day;
        int next;
    public:
        friend ostream& operator<<(ostream &f, Vertiba &v);
        Vertiba(int y=0, int m=0, int d=0, int n=1) {
            year = y; month = m; day = d; next = n;
        };
        void writeRecord(ostream &f) {
            f.write((char*)this, sizeof(Vertiba));
        };
        bool readRecord(istream &f) {
            f.read((char*)this, sizeof(Vertiba));
            return f.good();
        };
        void createFile(istream &fin=cin, ostream &fout=cout) {
            int c_y, c_n;
            cout << "Insert date (year month day): ";
            cin >> c_y >> month >> day;
            do {
                if (c_y>0 && month>0 && month<=12 && day>0 && day<=31) {
                    next++;
                    year = c_y;
                    writeRecord(fout);
                } else cout << "Incorrect data! Try again!" << endl;
                cout << "Insert date (year month day): ";
                cin >> c_y >> month >> day;
            } while (fin);

            fout.clear();
            fout.seekp(next*sizeof(Vertiba)-2*sizeof(Vertiba));
            c_n=next; next=-1;
            writeRecord(fout);
            next=c_n;


        };
        void reNext(istream &fin=cin, ostream &fout=cout) {
            int date;
            printSort *p = new printSort;
            printSort *a, *r, *g, *pf=p;

            // Sakārto sarakstu pēc loģiskiem datumiem
            ifstream f("vertibas.bin", ios::binary);
            f.open("vertibas.bin", ios::binary);

            f.clear(); f.seekg(0);
            while (readRecord(f)) {
                a=p; r=NULL; date = getDateVal();

                while (a!=NULL) {
                    g=a;
                    if (a->size==0) break;
                    else if (date < a->size) { r=a; break; }
                    a = a->next;
                }

                if (r==NULL) {
                    if (a==p && a->size==0) a->size = date;
                    else g->next = new printSort(date);
                } else {
                    printSort *d=p, *bf;
                    while (d!=NULL) {
                        if (d==r) {
                            if (d==p) {
                                p = new printSort(date); p->next = r;
                                break;
                            } else bf->next = new printSort(date); bf->next->next = d;
                        }
                        bf = d; d = d->next;
                    }
                }
            }

            bool mCheck[next];
            for (int i=0; i<next; i++) mCheck[i]=true;

            // Sakārto norādes bin. failā loģiskā secībā
            f.clear(); f.seekg(0); int i=1; int ncount;
            while (p!=NULL) {
                i++;
                f.clear(); f.seekg(0);
                ncount=0;
                while (readRecord(f)) {
                    if (mCheck[ncount]) {
                        if (p->size == getDateVal()) {
                            mCheck[ncount]=false;
                            fout.clear(); fout.seekp(ncount*sizeof(Vertiba));
                            if (p->next!=NULL) next=i;
                            else next=0;
                            writeRecord(fout);
                            break;
                        }
                    }
                    ncount++;
                }
                p = p->next;
            }
            f.close();

            while(pf!=NULL) {
                a=pf;
                pf=pf->next;
                delete a;
            }
        };
        bool longYear(int y) {
            int s=y;
            if (y>=1000) y%=100;
            else if (y<1000 && y>10) y%=10;
            if (y%4==0) return true;
            else if (s>=100 && s%100==0) return (s>400 && s%400!=0 || s<400 ? true : false);
            return false;
        };
        int getDateVal() {
            int days=0, years=0, ly = (longYear(year) ? 29 : 28);
            int mDays[12] = {31,ly,31,30,31,30,31,31,30,31,30,31};
            for (int i=1; i<=month; i++) days += mDays[month-1];
            for (int i=0; i<year; i++) years += (longYear(i) ? 366 : 365);
            return years+days+day;
        };
        void LogicalRecord(istream &f) {
            // Izvada datumus loģiskā sakārtojumā
            f.clear(); f.seekg(0); int n=2, i=0;
            while (readRecord(f)) {
                if (n == next) {
                    cout << ++i << ") ";
                    cout << *this;
                    f.clear(); f.seekg(0);
                    n++;
                }
            }
            f.clear(); f.seekg(0);
            while (readRecord(f)) {
                if (next==0) {
                    cout << ++i << ") ";
                    cout << *this; break;
                }
            }
        };
        void appendData(istream &fin=cin, ostream &fout=cout) {
            cin.clear();
            cin.ignore();

            fout.seekp(next*sizeof(Vertiba)-2*sizeof(Vertiba));
            writeRecord(fout);

            fout.seekp(next*sizeof(Vertiba)-sizeof(Vertiba));
            int c_y, c_n;
            cout << "Insert date (year month day): ";
            cin >> c_y >> month >> day;
            do {
                if (c_y>0 && month>0 && month<=12 && day>0 && day<=31) {
                    next++;
                    year = c_y;
                    writeRecord(fout);
                } else cout << "Incorrect data! Try again!" << endl;
                cout << "Insert date (year month day): ";
                cin >> c_y >> month >> day;
            } while (fin);

            fout.clear();
            fout.seekp(next*sizeof(Vertiba)-2*sizeof(Vertiba));
            c_n=next; next=-1;
            writeRecord(fout);
            next=c_n;
        };
};
ostream& operator<<(ostream &f, Vertiba &v) {
    if (v.next==0) v.next++;
    f << "Year: " << v.year << " Month: " << v.month << " Day: " << v.day << " | Next: " << v.next << endl;
};

int main() {
    ofstream fout1("vertibas.bin", ios::binary);
    ifstream fin1("vertibas.bin", ios::binary);

    Vertiba date;
    date.createFile(cin, fout1);

    //date.appendData(cin, fout1);
    date.reNext(cin, fout1);
    fout1.close();

    // Fiziska secība
    int i=0;
    cout << endl <<  "Fiziska seciba: " << endl;
    while (date.readRecord(fin1)) {
        cout << ++i << ") ";
        cout << date;
    }
    fin1.close();

    // Loģiska secība
    cout << endl << "Logiska seciba: " << endl;
    fin1.open("vertibas.bin", ios::binary);
    date.LogicalRecord(fin1);

    fin1.close();

    return 0;
}
