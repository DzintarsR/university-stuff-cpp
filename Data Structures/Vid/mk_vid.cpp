#include "string.h"
#include "stdio.h"
#include "stdlib.h"

// Ieliek un izņem pēc steka
struct Packet {
    int docId;
    Packet *next;
    Packet() {next=NULL;}
};

// Paketes uztur pēc rindas
struct PacketRow {
    int docs;
    Packet *firstPacket;
    PacketRow *next;
};

int main() {
    FILE* inputFile;
    FILE* outputFile;
    inputFile = fopen("mk_vid.in", "r");
	outputFile = fopen("mk_vid.out", "w+");
}
