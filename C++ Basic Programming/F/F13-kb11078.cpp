/*******************************************
Kārlis Babris, kb11078
F13. Doti divi teksta faili. Uzrakstīt programmu, kura izveido trešo failu sekojoši:
katras pirmā faila rindiņas galā ir pierakstīta atbilstoša otrā faila rindiņa.
(Ja ieejas failos rindiņu skaits ir atšķirīgs, tad pēdējās rindiņas trešajā failā
ir tādas pašas kā failā ar lielāko rindiņu skaitu).
Programma izveidota: 2012/02/07
*******************************************/

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    char sim, sim1, c=0;
    bool proceed=false;

    // Ielādē failus
    fstream file1("file1.txt", ios::in);
    fstream file2("file2.txt", ios::in);
    fstream file3("file3.txt", ios::out);

    // Ciklē cauri katram simbolam failā
    file1.get(sim);
    while (file1) {
        if (sim == '\n') {
            file2.get(sim1);
            while (sim1 != '\n' && sim1) {
                file3.put(sim1);
                file2.get(sim1);
                if (sim1) proceed=true;
            };
            file3.put('\n');
        }
        else file3.put(sim);
        file1.get(sim);

        // Ja pirmajā failā vairs nav atlicis rindu, bet otrā ir, tad tās saliek faila beigās
        if (!file1 && proceed) {
            file2.get(sim1);
            while (file2) {
                file3.put(sim1);
                file2.get(sim1);
            };
        }
        c++;
    };

    // Ja 1. fails ir bijis tukšs, tad 3. aizpilda pēc 2.
    if (c == 0) {
        file2.get(sim1);
        while (file2) {
            file3.put(sim1);
            file2.get(sim1);
        };
    }

    // Aizver atvērtos failus
    file1.close();
    file2.close();
    file3.close();

    cout << "Faili tika apstradai.";

    return 0;
}

