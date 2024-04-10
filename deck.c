//
// Created by david on 03-Apr-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "deck.h"


void initializeDeck(Deck *deck){
    Node *dummyNode = malloc(sizeof(Node));
    if(dummyNode == NULL){
        fprintf(stderr, "Unable to allocate memory for dummy Node\n");
        exit(EXIT_FAILURE);
    }

    dummyNode->card.suit = '\0';
    dummyNode->card.value = '\0';
    dummyNode->card.isVisible = false;
    dummyNode->next = dummyNode;
    dummyNode->prev = dummyNode;
    dummyNode->isDummy = true;

    deck->top = dummyNode;
    deck->size = 0;
}



void shuffleDeck(Deck *deck) {
    if (!deck || deck->size < 2) return;

    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    Node **nodeArray = malloc(sizeof(Node*) * deck->size);
    if (!nodeArray) return;

    Node *current = deck->top;
    int count = 0;
    while (current != NULL) {
        nodeArray[count++] = current;
        current = current->next;
    }

    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Node *temp = nodeArray[i];
        nodeArray[i] = nodeArray[j];
        nodeArray[j] = temp;
    }

    deck->top = nodeArray[0];
    Node *node = deck->top;

    for (int i = 1; i < deck->size; i++) {
        node->next = nodeArray[i];
        node = node->next;
    }
    node->next = NULL;  // Sørg for at den sidste node i listen har en NULL pointer som next.

    free(nodeArray);  // Frigiv den midlertidige hukommelse anvendt for at opbevare node pointere.
}

void freeDeck(Deck *deck) {
    Node *current = deck->top;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(deck);
}

void freeTable(Table *table) {
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < 11; i++) {
        if (table->columns[i] != NULL) {
            freeDeck(table->columns[i]);
            table->columns[i] = NULL;
        }
    }
    free(table);
}


bool isValidCard(const char *cardStr) {
    const char *validValues = "A23456789TJQK";
    const char *validSuits = "CDHS";

    if (strlen(cardStr) != 2) {
        return false;
    }

    if (strchr(validValues, cardStr[0]) == NULL) {
        return false;
    }

    if (strchr(validSuits, cardStr[1]) == NULL) {
        return false;
    }

    return true;
}


bool addCard(Deck *deck, const char value, const char suit) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Unable to allocate memory for new Node\n");
        return false;
    }

    newNode->card.value = value;
    newNode->card.suit = suit;
    newNode->card.isVisible = false;
    newNode->isDummy = false;

    newNode->next = deck->top->next;
    newNode->prev = deck->top;
    deck->top->next->prev = newNode;
    deck->top->next = newNode;

    deck->size++;
    return true;
}

Table* initializeTable() {
    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Unable to allocate memory for Table\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 7; i++) {

        table->columns[i] = malloc(sizeof(Column));
        initializeDeck(table->columns[i]);
    }
    return table;
}

void dealToStartTable(Deck *deck, Table *table) {
    int cardCount = 0;
    Node *current = deck->top->next;

    while(cardCount < 52 && current != NULL && !current->isDummy){
        for (int colIndex = 0; colIndex < 7; ++colIndex) {
            if (current == NULL || current->isDummy) break;

            Node *columnDummy = table->columns[colIndex]->top;
            Node *lastCardInColumn = columnDummy;

            while (lastCardInColumn->next != NULL && !lastCardInColumn->next->isDummy) {
                lastCardInColumn = lastCardInColumn->next;
                //break;
            }

            current->prev = lastCardInColumn;
            lastCardInColumn->next = current;

            Node *nextCard = current->next;
            current->next = lastCardInColumn;
            current = nextCard;

            table->columns[colIndex]->size++;
            cardCount++;
        }
    }
    if (current != NULL && !current->isDummy) {
        fprintf(stderr, "Error: There are still cards left undistributed in the deck.\n");
    }
}


void printTable(Table *table) {
    if (table == NULL) {
        return;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\tF1\tF2\tF3\tF4\n\n");

    int maxRows = 0;
    for (int i = 0; i < 7; i++) {
        int columnSize = table->columns[i]->size;
        if (columnSize > maxRows) {
            maxRows = columnSize;
        }
    }

    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            Node *currentNode = table->columns[col]->top->next;
            for (int depth = 0; depth < row && currentNode != NULL; depth++) {
                currentNode = currentNode->next;
            }

            if (currentNode != NULL) {
                printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
            } else {
                printf("\t");
            }
        }

        if (row == 0) {
            for (int col = 7; col < 11; col++) {
                Node *foundationNode = table->columns[col]->top->next;
                if (foundationNode != NULL) {
                    printf("[%c%c]\t", foundationNode->card.value, foundationNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
            }
        }

        printf("\n");
    }
    printf("\nLAST Command: \nMessage: \n");
}

void setShowAllCards(Table *table, bool isVisible) {
    for (int i = 0; i < 7; i++) {
        Node *current = table->columns[i]->top;
        while (current != NULL) {
            current->card.isVisible = isVisible;
            current = current->next;
        }
    }
}

Deck* splitter(Deck* originalDeck, int splitPoint) {
    if (originalDeck == NULL || originalDeck->size < 2) {
        return NULL;
    }

    if (splitPoint <= 0 || splitPoint >= originalDeck->size) {
        srand((unsigned int)time(NULL));
        splitPoint = rand() % (originalDeck->size - 1) + 1;
    }

    Node* firstHalf = originalDeck->top;
    Node* secondHalf = firstHalf;

    for (int i = 0; i < splitPoint - 1; i++) {
        secondHalf = secondHalf->next;
    }

    Node* temp = secondHalf;
    secondHalf = secondHalf->next;
    temp->next = NULL;

    Deck* newDeck = malloc(sizeof(Deck));
    if (newDeck == NULL) {
        fprintf(stderr, "Unable to allocate memory for new Deck\n");
        exit(EXIT_FAILURE);
    }
    newDeck->top = NULL;
    newDeck->size = 0;

    Node** current = &newDeck->top;

    while (firstHalf != NULL && secondHalf != NULL) {
        if (firstHalf) {
            *current = firstHalf;
            firstHalf = firstHalf->next;
            current = &((*current)->next);
            newDeck->size++;
        }
        if (secondHalf) {
            *current = secondHalf;
            secondHalf = secondHalf->next;
            current = &((*current)->next);
            newDeck->size++;
        }
    }

    while (firstHalf != NULL) {
        *current = firstHalf;
        firstHalf = firstHalf->next;
        current = &((*current)->next);
        newDeck->size++;
    }
    while (secondHalf != NULL) {
        *current = secondHalf;
        secondHalf = secondHalf->next;
        current = &((*current)->next);
        newDeck->size++;
    }

    *current = NULL;
    return newDeck;
}


void printDeck(Deck* deck){
    Node* current = deck->top;
    for (int i = 0; i < deck->size; i++){
        printf("%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }
}