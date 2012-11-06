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
    Node(int x) {
        left = right = NULL;
        val = x;
    };
};

class Marss {
    private:
        //Node *first, *current;
        Node *findLeafNode, *first, *current;
        FILE *file;
        int favourites[3];
        bool continueRecursion;
    public:
        // construct
        Marss(FILE *f) {
            file = f;
            first = current = new Node;
            findLeafNode = NULL;
            continueRecursion = true;

            for (int i=0; i<3; i++) favourites[i] = 0;
        };

        // destruct
        ~Marss() {};

        void findInOrder(int x, Node *p) {
            if (continueRecursion) {
                if (p->left != NULL) {
                    findInOrder(x, p->left);
                }

                if (p->val == x) favourites[1] = x;
                else if (p->val != x && favourites[1] == 0) favourites[0] = p->val;
                else if (favourites[1] != 0 && favourites[2] == 0) {
                    favourites[2] = p->val;
                    continueRecursion = false;
                }

                if (p->right != NULL) {
                    findInOrder(x, p->right);
                }
            }
        };

        // print error message
        void printError(int x) {
            char e0[] = "error0";
            char e1[] = "error1";
            char e2[] = "error2";
            char e3[] = "error3";
            char e4[] = "error4";
            char e5[] = "error5";
            switch (x) {
                case 0: fprintf(file, "%s\n", e0); break;
                case 1: fprintf(file, "%s\n", e1); break;
                case 2: fprintf(file, "%s\n", e2); break;
                case 3: fprintf(file, "%s\n", e3); break;
                case 4: fprintf(file, "%s\n", e4); break;
                case 5: fprintf(file, "%s\n", e5); break;
            }
        };

        // set first martian
        void setMainMartian(int x) {
            first->val = x;
        };

        // find leaf and return its adress
        void findLeaf(int x, Node *p) {
            if (findLeafNode == NULL) {
                if (p->val == x) findLeafNode = p;
                else {
                    if (p->left != NULL) findLeaf(x, p->left);
                    if (p->right != NULL) findLeaf(x, p->right);
                }
            }
        };

        // set right marsion
        void setMarsion(int marsion, int childValue, int leaf) {
            // leaf (1->left, 0->right)

            // check if child already exists
            findLeaf(childValue, first);
            if (findLeafNode != NULL) {
                // If child exists error3
                printError(3);
            } else {
                // look for marsion, if find set, if not, print error
                findLeaf(marsion, first);
                if (findLeafNode != NULL) {
                    switch (leaf) {
                        case 1:
                            if (findLeafNode->left == NULL) findLeafNode->left = new Node(childValue);
                            // If left child exists error4
                            else printError(4);
                            break;
                        case 0:
                            if (findLeafNode->right == NULL) findLeafNode->right = new Node(childValue);
                            // If right child exists error5
                            else printError(5);
                            break;
                    }
                // If marsion doesnt exists error2
                } else printError(2);
            }

            findLeafNode = NULL;
        };

        void printMarsionFavourites() {
            if (favourites[1] != 0) {
                fprintf(file, "%d %d\n", favourites[0], favourites[2]);
            } else printError(0);
        };

        void getMarsionFavourites(int marsion) {
            findInOrder(marsion, first);
            continueRecursion = true;

            printMarsionFavourites();
            // reset marsions
            for (int i=0; i<3; i++) favourites[i] = 0;
        };
};


int main() {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("marss.in", "r");
	outputFile = fopen("marss.out", "w+");

    Marss *marss = new Marss(outputFile);

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
            if (marsion == value) {
                marss->printError(1);
            } else marss->setMarsion(marsion, value, 1);
        }
        else if (command[0] == 'R') {
            fscanf(inputFile, "%d %d", &marsion, &value);
            if (marsion == value) {
                marss->printError(1);
            } else marss->setMarsion(marsion, value, 0);
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
