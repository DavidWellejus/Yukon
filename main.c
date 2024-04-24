#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"
#include <gtk/gtk.h>
#include "gui.h"
#include "main.h"



int main(int argc, char **argv) {
    srand(time(NULL));
    Deck *deck = malloc(sizeof(Deck));
    Table *table = initializeTable();
    if (deck == NULL || table == NULL) {
        fprintf(stderr, "Failed to allocate memory for deck or tableau.\n");
        exit(EXIT_FAILURE);
    }

    //her kommer gui-logikken på spil:
    GtkApplication *app;
    int status;

    // Opret en ny GtkApplication
    app = gtk_application_new("org.example.yukon", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Kør applikationen og afregistrer
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;






    printTable(table, "", "");

    char inputLine[256];
    char command[256];
    char fileName[256];


    int t = 0;
    while (true) {
        if (strcmp(command, "P") == 0) t = 1;
        if (strcmp(command, "Q") == 0 && t == 1) t = 0;
        char message[256] = "OK";
        printf("INPUT > ");
        fflush(stdout);

        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        stripNewline(inputLine);


        if (sscanf(inputLine, "%s", command) == 1) {

            if(t == 1 && strcmp(command, "LD") == 0 || strcmp(command, "SI") == 0 || strcmp(command, "SR") == 0 || strcmp(command, "SD") == 0 || strcmp(command, "QQ") == 0){
                strcpy(message, "Command not available in the PLAY phase.");
                printTable(table, command, message);
            }
            if (t == 0) {
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
                        deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
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
                        Deck *newDeck = splitter(deck, split);
                        free(deck);
                        deck = newDeck;
                        clearTable(table);
                        dealToStartTable(deck, table);
                        setShowAllCards(table, false);
                        printTable(table, command, message);
                    }
                    else {
                        split = rand() % (deck->size - 1) + 1;
                        strcpy(message,"Using random split.");
                        Deck *newDeck = splitter(deck, split);
                        free(deck);
                        deck = newDeck;
                        clearTable(table);
                        dealToStartTable(deck, table);
                        setShowAllCards(table, false);
                        printTable(table, command, message);
                    }
                }

                else if (strcmp(command, "SR") == 0) {
                    Deck *newDeck = shuffleDeck(deck);
                    free(deck);
                    deck = newDeck;
                    clearTable(table);
                    dealToStartTable(deck, table);
                    setShowAllCards(table, false);
                    printTable(table, command, message);
                }

                else if (strcmp(command, "SD") == 0) {
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
                        saveDeckToFile(deck,"C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/cards.txt");
                        printTable(table, command, message);
                    }
                }

                else if (strcmp(command, "P") == 0) {
                    clearTable(table);
                    dealToGameTable(table, deck);
                    printTable(table, command, message);
                }

                else if (strcmp(command, "QQ") == 0) {
                    break;
                } else {
                    printf("Unknown command.\n");
                }
            }

            if (t == 1) {
                if (strcmp(command, "Q") == 0){
                    t = 0;
                    clearTable(table);
                    printTable(table, command, message);
                }

                char sourceCol, destCol;
                int sourceColNum, destColNum;
                char cardValue[3];
                char cardSuit;


                int count = sscanf(inputLine, "%c%d:%2s->%c%d", &sourceCol, &sourceColNum, cardValue, &destCol, &destColNum);

                if (count == 5) {
                    if (!moves(table, inputLine)) {
                        strcpy(message, "The move is not valid.");
                    }
                    printTable(table, command, message);
                }
                else {
                    count = sscanf(inputLine, "%c%d->%c%d", &sourceCol, &sourceColNum, &destCol, &destColNum);
                    if (count == 4) {
                        if (!movesCol(table, inputLine)) {
                            strcpy(message, "The move is not valid.");
                        }
                        printTable(table, inputLine, message);
                    }
                }
            }
        } else {
            printf("Error: No command entered.\n");
        }

    }
    free(deck);
    free(table);

    return 0;
}

void stripNewline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';  // Erstat '\n' med null-terminator
    }
}