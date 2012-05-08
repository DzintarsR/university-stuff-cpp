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
            c_n=next; next=0;
            writeRecord(fout);
            next=c_n;
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
            int date;
            printSort *p = new printSort;
            printSort *a, *r, *g, *pf=p;

            // Sakārto sarakstu pēc loģiskiem datumiem
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

            // Izvada datumus loģiskā sakārtojumā
            f.clear(); f.seekg(0);
            while (p!=NULL) {
                f.clear(); f.seekg(0);
                while (readRecord(f)) {
                    if (p->size == getDateVal()) {
                        cout << *this; break;
                    }
                }
                p = p->next;
            }

            while(pf!=NULL) {
                a=pf;
                pf=pf->next;
                delete a;
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
            c_n=next; next=0;
            writeRecord(fout);
            next=c_n;
        };
};
ostream& operator<<(ostream &f, Vertiba &v) {
    f << "Year: " << v.year << " Month: " << v.month << " Day: " << v.day << " Next: " << v.next << endl;
};

int main() {
    ofstream fout1("vertibas.bin", ios::binary);
    ifstream fin1("vertibas.bin", ios::binary);
    fstream f;

    Vertiba date;
    date.createFile(cin, fout1);

    //date.appendData(cin, fout1);
    fout1.close();

    // Fiziska secība
    cout << endl <<  "Fiziska seciba: " << endl;
    while (date.readRecord(fin1)) {
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
