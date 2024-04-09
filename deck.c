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
    deck->top = NULL;
    deck->size = 0;
    char suits[] = {'C', 'D', 'H', 'S'};
    char values[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            Node *newNode = malloc(sizeof(Node));
            if(newNode == NULL){
                fprintf(stderr, "Unable to allocate memory for new CardNode\n");
                exit(EXIT_FAILURE);
            }
            newNode->card.suit = suits[i];
            newNode->card.value = values[j];
            newNode->card.isVisible = false;
            newNode->next = deck->top;
            deck->top = newNode;
            deck->size++;
        }
    }
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
    Node *newCard = malloc(sizeof(Node));
    if (newCard == NULL) {
        fprintf(stderr, "Unable to allocate memory for new CardNode\n");
        return false;
    }

    newCard->card.value = value;
    newCard->card.suit = suit;
    newCard->card.isVisible = false;
    newCard->next = NULL;

    if (deck->top == NULL) {
        deck->top = newCard;
    }
    else {
        newCard->next = deck->top;
        deck->top = newCard;
    }
    deck->size++;
    return true;
}
Table* initializeTable() {
    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Unable to allocate memory for Table\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 11; i++) {
        table->columns[i] = malloc(sizeof(Column));
        table->columns[i]->top = NULL;
        table->columns[i]->size = 0;
    }
    return table;
}

void dealToStartTable(Deck *deck, Table *table) {
    Node *current = deck->top;
    int colIndex = 0;

    while (current != NULL) {
        Node *cardToAdd = current;
        current = current->next;
        cardToAdd->next = NULL;

        if (table->columns[colIndex]->top == NULL) {
            table->columns[colIndex]->top = cardToAdd;
        }
        else {
            Node *lastCard = table->columns[colIndex]->top;
            while (lastCard->next != NULL) {
                lastCard = lastCard->next;
            }
            lastCard->next = cardToAdd;
        }
        table->columns[colIndex]->size++;
        colIndex = (colIndex + 1) % 7;
    }
    deck->top = NULL;
}

void printTable(Table *table) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    int maxRows = 0;
    for (int col = 0; col < 7; col++) {
        int columnSize = table->columns[col]->size;
        maxRows = (columnSize > maxRows) ? columnSize : maxRows;
    }

    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            Node *currentNode = table->columns[col]->top;
            for (int depth = 0; currentNode != NULL && depth < row; depth++) {
                currentNode = currentNode->next;
            }

            if (currentNode != NULL) {
                if (currentNode->card.isVisible) {
                    printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
                }
                else {
                    printf("[ ]\t");
                }
            }
            else {
                printf("\t");
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("\nLAST Command: \nMessage: \nINPUT > ");
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