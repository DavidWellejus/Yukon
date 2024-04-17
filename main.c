#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"


void initializeDeck(Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
Deck* shuffleDeck(Deck* deck);
Deck* loadDeckFromFile(const char *filename);
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table, char lastCommand[256]);
void setShowAllCards(Table *table, bool isVisible);
Deck* splitter(Deck* deck, int split);
void printDeck(Deck* deck);
void dealToGameTable(Table* table, Deck* deck);
void clearTable(Table *table);


int main() {
    srand(time(NULL));
    Deck *deck = malloc(sizeof(Deck));
    Table *table = initializeTable();
    if (deck == NULL || table == NULL) {
        fprintf(stderr, "Failed to allocate memory for deck or tableau.\n");
        exit(EXIT_FAILURE);
    }

    printTable(table, "");

    char inputLine[256];
    char command[256];
    char fileName[256];

    int t = 0;
    while (true) {
        if(strcmp(command, "P") == 0) t = 1;
        if(strcmp(command, "Q") == 0 && t == 1) t = 0;
        printf("INPUT > ");
        fflush(stdout);

        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }


            if (sscanf(inputLine, "%s", command) == 1) {

                if(t == 0){
                    if (strcmp(command, "LD") == 0) {
                        if (sscanf(inputLine, "%*s %s", fileName) == 1) {

                            if (!loadDeckFromFile(fileName)) {
                                printf("Error: Invalid file or unable to load deck.\n");
                            } else {
                                deck = loadDeckFromFile(fileName);
                                clearTable(table);
                                dealToStartTable(deck, table);
                                printTable(table, command);
                            }
                        }
                        else {
                            deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
                            clearTable(table);
                            dealToStartTable(deck, table);
                            printTable(table, command);
                        }
                    }
                    else if (strcmp(command, "SW") == 0) {
                        setShowAllCards(table, true);
                        printTable(table, command);
                    }

                    else if (strcmp(command, "SI") == 0) {
                        int split;
                        if (sscanf(inputLine, "%*s %d", &split) == 1) {
                            split = rand() % (deck->size - 1) + 1;
                            printf("Using random split: %d\n", split);

                        }
                        Deck* newDeck = splitter(deck, split);
                        free(deck);
                        deck = newDeck;
                        clearTable(table);
                        dealToStartTable(deck, table);
                        setShowAllCards(table, false);
                        printTable(table, command);
                    }

                    else if(strcmp(command, "SR") == 0){
                        Deck* newDeck = shuffleDeck(deck);
                        free(deck);
                        deck = newDeck;
                        clearTable(table);
                        dealToStartTable(deck, table);
                        setShowAllCards(table, false);
                        printTable(table, command);
                    }

                    else if(strcmp(command, "SD") == 0){
                        if (sscanf(inputLine, "%*s %s", fileName) == 1) {
                            saveDeckToFile(deck, fileName);
                            printTable(table, command);
                        }
                        else {
                            saveDeckToFile(deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/cards.txt");
                            printTable(table, command);
                        }
                    }
                    else if(strcmp(command, "P") == 0){
                        clearTable(table);
                        dealToGameTable(table, deck);
                        printTable(table, command);
                    }
                    else if (strcmp(command, "QQ") == 0) {
                        break;
                    } else {
                        printf("Unknown command.\n");
                    }
                }

                if(t == 1){
                    if (strcmp(command, "Q") == 0) t = 0;
                    clearTable(table);
                    printTable(table, command);
                }
            }
            else {
                printf("Error: No command entered.\n");
            }
    }

    free(deck);
    free(table);

    return 0;
}