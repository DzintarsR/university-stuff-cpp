#define NULL_VALL 0

struct El {
    int val;
    El *next;
};

class List {
	El *first, *last;
	El * elPlacePointer(int, El *);
	public:
		List() { 
			last = first = new El; 
			first->val=NULL_VALL; 
			first->next=NULL; 
		};
		~List(){ if(first!=NULL) deleteList(); };
		void addElement(int);
		void deleteList();
		void printList();
		void removeAndSwitchElement(int n, int m);
};

void List::deleteList() {
	El *current=first, *next;
	while (first->val!=NULL_VALL) {
		if (current->next == NULL) { 
			delete current;
			first = NULL;
			break;
		}
		next = current->next;
		delete current;
		current = next;
	}
}

void List::addElement(int a) {
	El *p=first;
	bool proceed=true;
	if (p->val==NULL_VALL) {
		p->val = a;
		proceed=false;
	}
	while (proceed) {
		if (p->next != NULL) {
			p = p->next;
		} else {
			p->next = new El;
			p->next->val = a;
			p->next->next = NULL;
			last = p->next;
			break;
		}
	}
}

void List::printList() {
	El *p=first;
	bool proceed=true;
	if (p->val==NULL_VALL) {
		cout << "List is empty!" << endl;
		proceed=false;
	}
	while (proceed) {
		if (p->next != NULL) {
			cout << p->val << endl;
			p = p->next;
		} else {
			cout << p->val << endl;
			break;
		}
	}
}

El * List::elPlacePointer(int place, El *p) {
	if (place == 1) {
		if (p->val != 0) {
			return p;
		}
	}
	if ((p->next == NULL && place-1>=1) || place < 1) {
		return NULL;
	}
	return elPlacePointer(place-1, p->next);
}

void List::removeAndSwitchElement(int n, int m) {
	El *removeEl, *changeEl;
	El *beforeDel, *afterDel, *beforeChange, *afterChange;
	removeEl = elPlacePointer(n, first);
	changeEl = elPlacePointer(m, first);
	if (removeEl != NULL && changeEl != NULL) {
		afterDel = elPlacePointer(n+1, first);
		beforeDel = elPlacePointer(n-1, first);
		afterChange = elPlacePointer(m+1, first);
		beforeChange = elPlacePointer(m-1, first);

		// Ja maināmais ir sarakstā pirmais
		if (removeEl == first) {
			if (afterDel == changeEl) {
				first = changeEl;
			} else {
				first = changeEl;
				changeEl->next = afterDel;
				if (afterChange != NULL) {
					beforeChange->next = afterChange;
				} else {
					beforeChange->next = NULL;
					last = beforeChange;
				}
			}
		}

		// Ja maināmais ir saraksta beigās
		else if (removeEl == last) {
			if (changeEl == beforeDel) {
				last = beforeDel;
				beforeDel->next = NULL;
			} else {
				last = beforeDel;
				last->next = changeEl;
				changeEl->next = NULL;
				if (changeEl == first) {
					first = afterChange;
				} else {
					beforeChange->next = afterChange;
				}
			}
		}

		// Ja maināmie ir kaut kur pa vidu
		else if (removeEl != first && removeEl != last) {
			if (changeEl == afterDel) {
				beforeDel->next = afterDel;
			}
			else if (changeEl == beforeDel) {
				beforeDel->next = changeEl;
				changeEl->next = afterDel;
			} 
			else {
				beforeDel->next = changeEl;
				changeEl->next = afterDel;
				if (changeEl == first) {
					first = afterChange;
				} else {
					beforeChange->next = afterChange;
				}
			}
		}
		delete removeEl;
	} else 
	cout << "Dzesamais vai aizvietojamais el. neeksiste!" << endl;
}