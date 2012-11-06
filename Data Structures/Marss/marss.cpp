#include "string.h"
#include "stdio.h"
#include "stdlib.h"

struct Node {
    int val;
    Node *left;
    Node *right;

    Node() {
        left = right = NULL;
        val = 0;
    };
    Node(int x, Node *l, Node *r) {
        val = x;
        left = l;
        right = r;
    };
};

class Marss {
    private:
        Node *first, *current;
    public:
        // construct
        Marss() {
            first = current = new Node;
        };

        // destruct
        ~Marss() {};

        // visit tree as inorder
        void inOrder(Node p) {
            if (p == NULL) return NULL;
            else {
                inOrder(p->left);

                // save printable data

                inOrder(p->right);
            }
        };

        // print node value
        void visitNode(Node p) {
            fprintf(outputFile, "%d %d\n", regId, money);
        };

        // set first martian
        void setMainMartian(int x) {
            first->val = x;
        };

        // set right marsion
        void setMarsion(int marsion, int value, int leaf) {
            // leaf (1->left, 0->right)

        };
};


int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("marss.in1000", "r");
	outputFile = fopen("marss.out", "w+");

    Marss *marss = new Marss;

    char command[10];
    int check, main, marsion, value;

    // Get and set main marsion
	fscanf(inputFile, "%d", &main);
	marss->setMainMartian(main);

    while (inputFile) {
	    check = fscanf(inputFile, "%s", command);
        if (check==EOF) break;

        // Get and set marsion
        if (command[0] == 'L') {
            fscanf(inputFile, "%d %d", &marsion, &value);
            marss->setMarsion(marsion, value, 1);
        }
        else if (command[0] == 'R') {
            fscanf(inputFile, "%d %d", &marsion, &value);
            marss->setMarsion(marsion, value, 0);
        }
        else if (command[0] == '?') {
            fscanf(inputFile, "%d", &marsion);
            marss->getMarsionFavourites(marsion);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
	return 0;
}
