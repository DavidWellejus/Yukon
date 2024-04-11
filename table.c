//
// Created by david on 11-Apr-24.
//

#include <stdio.h>
#include <stdlib.h>
#include "table.h"
Table* initializeTable() {
    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Unable to allocate memory for Table\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 11; i++) {

        table->columns[i] = malloc(sizeof(Node));
        if (table->columns[i] == NULL) {
            fprintf(stderr, "Unable to allocate memory for new Node\n");
            return false;
        }

        table->columns[i]->card.value = '\0';
        table->columns[i]->card.suit = '\0';
        table->columns[i]->card.isVisible = false;
        table->columns[i]->prev = table->columns[i];
        table->columns[i]->next = table->columns[i];
        table->columns[i]->isDummy = true;
    }
    return table;
}

void dealToStartTable(Deck *deck, Table *table) {
    int cardCount = 0;
    Node* cardToDeal = deck->top->next;

    while (!cardToDeal->isDummy) {
        for (int colIndex = 0; colIndex < 7; ++colIndex) {
            if (cardToDeal->isDummy) break;

            Node *columnDummy = table->columns[colIndex];
            Node *lastCardInColumn = columnDummy;

            while (!lastCardInColumn->next->isDummy) {
                lastCardInColumn = lastCardInColumn->next;
            }

            cardToDeal->prev = lastCardInColumn;
            lastCardInColumn->next = cardToDeal;

            Node *nextCard = cardToDeal->next;

            cardToDeal->next = columnDummy;
            columnDummy->prev = cardToDeal;
            cardToDeal = nextCard;

            cardCount++;
            if (cardToDeal->isDummy) break;
        }
    }

    if (!cardToDeal->isDummy) {
        fprintf(stderr, "Error: There are still cards left undistributed in the deck.\n");
    }
}

void printTable(Table *table) {
    if (table == NULL) {
        return;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\tF1\tF2\tF3\tF4\n\n");

    int maxRows = 0;
    for (int col = 0; col < 7; col++) {
        int count = 0;
        Node *current = table->columns[col]->next;
        while (!current->isDummy) {
            count++;
            current = current->next;
        }
        if (count > maxRows) {
            maxRows = count;
        }
    }

    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            Node *currentNode = table->columns[col]->next;
            for (int depth = 0; depth < row && !currentNode->isDummy; depth++) {
                currentNode = currentNode->next;
            }

            if (!currentNode->isDummy) {
                if(currentNode->card.isVisible){
                    printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
            } else {
                printf("\t");
            }
        }

        // Only print the foundation nodes on the first row
        if (row == 0) {
            for (int col = 7; col < 11; col++) {
                Node *foundationNode = table->columns[col]->next;
                if (!foundationNode->isDummy) {
                    printf("[%c%c]\t", foundationNode->card.value, foundationNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
            }
        } else {
            printf("\t\t\t\t"); // Space for foundation columns
        }

        printf("\n");
    }

    printf("\nLAST Command: \nMessage: \n");
}


void setShowAllCards(Table *table, bool isVisible) {
    for (int i = 0; i < 7; i++) {
        Node *current = table->columns[i]->next;
        while (!current->isDummy) {
            current->card.isVisible = isVisible;
            current = current->next;
        }
    }
}