//
// Created by david on 11-Apr-24.
//

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "shuffle.h"
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