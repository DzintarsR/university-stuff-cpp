#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define MARSIONS 10000

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
        Node *first, *marsionArray[MARSIONS+1];
        FILE *file;
        int favourites[3];
        bool continueRecursion;
    public:
        // construct
        Marss(FILE *f) {
            file = f;
            first = new Node;
            continueRecursion = true;

            for (int i=0; i<3; i++) favourites[i] = 0;
            for (int i=1; i<=MARSIONS; i++) marsionArray[i] = NULL;
        };

        // delete all nodes from tree and free memory
        void deleteTree(struct Node *p) {
            if(p) {
                if(p->left) deleteTree(p->left);
                if(p->right) deleteTree(p->right);
                delete(p);
            }
        };

        // destruct
        ~Marss() {
            deleteTree(first);
        };

        void findInOrder(int x, struct Node *p) {
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
            fprintf(file, "error%d\n", x);
        };

        // set first martian
        void setMainMartian(int x) {
            first->val = x;
            marsionArray[x] = first;
        };

        // set marsion: leaf (1->left, 0->right)
        void setMarsion(int marsion, int childValue, int leaf) {
            // check if child already exists
            if (marsionArray[childValue] != NULL) {
                // If child exists error3
                printError(3);
            } else {
                // look for marsion, if find set, if not, print error
                Node *marsionNode;
                marsionNode = marsionArray[marsion];
                if (marsionNode != NULL) {
                    switch (leaf) {
                        case 1:
                            if (marsionNode->left == NULL)
                                 marsionArray[childValue] = marsionNode->left = new Node(childValue);
                            // If left child exists error4
                            else printError(4);
                            break;
                        case 0:
                            if (marsionNode->right == NULL)
                                marsionArray[childValue] = marsionNode->right = new Node(childValue);
                            // If right child exists error5
                            else printError(5);
                            break;
                    }
                // If marsion doesnt exists error2
                } else printError(2);
            }
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

    marss->~Marss();
	return 0;
}

