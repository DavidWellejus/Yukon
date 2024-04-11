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
    Node *cardToDeal = deck->top->next; // Start with the first real card, skipping the dummy.

    while (cardToDeal != deck->top && cardCount < 52) { // We will deal 52 cards only.
        for (int colIndex = 0; colIndex < 7; ++colIndex) {
            if (cardToDeal == deck->top) break; // If we have looped back to the dummy node, break.

            // Create a new node that will be added to the column.
            Node *newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate memory for a new card node.\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *cardToDeal; // Copy the card details.
            newNode->next = newNode->prev = NULL; // This node will be standalone until linked.

            // Now link the new node into the column.
            Node *columnDummy = table->columns[colIndex];
            Node *lastCardInColumn = columnDummy->prev; // The last card is the one before the dummy node.

            newNode->prev = lastCardInColumn;
            newNode->next = columnDummy;
            lastCardInColumn->next = newNode;
            columnDummy->prev = newNode;

            cardToDeal = cardToDeal->next; // Move to the next card to deal.
            cardCount++; // Increase the dealt card count.
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