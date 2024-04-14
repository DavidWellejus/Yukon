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
    Node *cardToDeal = deck->top->next;

    while (cardToDeal != deck->top && cardCount < 52) {
        for (int i = 0; i < 7; ++i) {
            if (cardToDeal == deck->top) break;

            Node *newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate memory for a new card node.\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *cardToDeal;
            newNode->next = newNode->prev = NULL;

            Node *columnDummy = table->columns[i];
            Node *lastCardInColumn = columnDummy->prev;

            newNode->prev = lastCardInColumn;
            newNode->next = columnDummy;
            lastCardInColumn->next = newNode;
            columnDummy->prev = newNode;

            cardToDeal = cardToDeal->next;
            cardCount++;
        }
    }

    if (cardCount < 52) {
        fprintf(stderr, "Error: Not all cards have been distributed from the deck.\n");
    }
}


void printTable(Table *table) {
    if (table == NULL) {
        return;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

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

        if (row % 2 == 0) {
                Node *foundationNode = table->columns[7 + row / 2]->next;
                if (!foundationNode->isDummy) {
                    printf("\t [%c%c] F%d\t", foundationNode->card.value, foundationNode->card.suit, 1 + row / 2);
                } else {
                    printf("\t [ ] F%d\t", 1 + row / 2);
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