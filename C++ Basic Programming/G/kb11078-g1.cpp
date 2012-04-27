#include <iostream>
using namespace std;
#include "class-struct.cpp"

int main() {
    List *elem = new List;
    int num, rEl, wEl;

    cout << "Add number to list: ";
    cin >> num;

    while (num!=0) {
        elem->addElement(num);
        cout << "Add number to list: ";
        cin >> num;
    }

    cout << "Kam ar ko:(1 un 2): ";
    cin >> rEl >> wEl;
    elem->removeAndSwitchElement(rEl, wEl);

    elem->printList();
    elem->deleteList();

    return 0;
}
