#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"

void printStartupScreen();
void initializeDeck(Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
Deck* shuffleDeck(Deck* deck);
Deck* loadDeckFromFile(const char *filename);
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table, char lastCommand[256], char message[256]);
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

    printStartupScreen();

    char inputLine[256];
    char command[256];
    char fileName[256];


    while (true) {
        char message[256] = "OK";
        printf("INPUT > ");
        fflush(stdout);

        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        if (sscanf(inputLine, "%s", command) == 1) {

            if (strcmp(command, "LD") == 0) {
                if (sscanf(inputLine, "%*s %s", fileName) == 1) {

                    if (loadDeckFromFile(fileName) == NULL) {
                        strcpy(message, "Error: Invalid file or unable to load deck.");
                        printTable(table, command, message);
                    } else {
                        deck = loadDeckFromFile(fileName);
                        clearTable(table);
                        dealToStartTable(deck, table);
                        printTable(table, command, message);
                    }
                }
                else {
                    deck = loadDeckFromFile("C:/Users/klavs/OneDrive/Skrivebord/DTU/2.Semester/Maskiner programering/Yukon/Deck.txt");
                    clearTable(table);
                    dealToStartTable(deck, table);
                    printTable(table, command, message);
                }
            }
            else if (strcmp(command, "SW") == 0) {
                setShowAllCards(table, true);
                printTable(table, command, message);
            }

            else if (strcmp(command, "SI") == 0) {
                int split;
                if (sscanf(inputLine, "%*s %d", &split) == 1) {
                    split = rand() % (deck->size - 1) + 1;
                    strcpy(message,"Using random split.");

                }
                Deck* newDeck = splitter(deck, split);
                free(deck);
                deck = newDeck;
                clearTable(table);
                dealToStartTable(deck, table);
                setShowAllCards(table, true);
                printTable(table, command, message);
            }

            else if(strcmp(command, "SR") == 0){
                Deck* newDeck = shuffleDeck(deck);
                free(deck);
                deck = newDeck;
                clearTable(table);
                dealToStartTable(deck, table);
                setShowAllCards(table, true);
                printTable(table, command, message);
            }

            else if(strcmp(command, "SD") == 0){
                if (sscanf(inputLine, "%*s %s", fileName) == 1) {
                    if(!saveDeckToFile(deck, fileName)){
                        strcpy(message, "Error: Unable to open file for writing.");
                        printTable(table, command, message);
                    }
                    else{
                        saveDeckToFile(deck, fileName);
                        printTable(table, command, message);
                    }

                }
                else {
                    saveDeckToFile(deck, "C:/Users/klavs/OneDrive/Skrivebord/DTU/2.Semester/Maskiner programering/Yukon/cards.txt");
                    printTable(table, command, message);
                }
            }
            else if(strcmp(command, "P") == 0){
                clearTable(table);
                dealToGameTable(table, deck);
                printTable(table, command, message);
            }
            else if (strcmp(command, "QQ") == 0) {
                break;
            } else {
                strcpy(message,"Unknown command.\n");
            }
        } else {
            strcpy(message,"Error: No command entered.\n");
        }
    }

    free(deck);
    free(table);

    return 0;
}

void printStartupScreen() {

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    printf("\t\t\t\t\t\t\t\t[ ] F1\n");
    printf("\t\t\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t\t[ ] F2\n");
    printf("\t\t\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t\t[ ] F3\n");
    printf("\t\t\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t\t[ ] F4\n\n");

    printf("LAST Command:\n");
    printf("Message:\n");
}