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

    // Bestem splitpunktet
    if (splitPoint <= 0 || splitPoint >= originalDeck->size) {
        srand((unsigned int)time(NULL));
        splitPoint = rand() % (originalDeck->size - 1) + 1;
    }

    Node* firstHalf = originalDeck->top->next; // Skip dummy node
    Node* secondHalf = firstHalf;

    // Find splitpunktet
    for (int i = 0; i < splitPoint; i++) {
        secondHalf = secondHalf->next;
    }

    // Opretter et nyt dæk med en dummy node
    Deck* newDeck = malloc(sizeof(Deck));
    if (newDeck == NULL) {
        fprintf(stderr, "Unable to allocate memory for new Deck\n");
        exit(EXIT_FAILURE);
    }

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
    newDeck->top = dummyNode;
    newDeck->size = 0;

    Node** current = &dummyNode->next;

    // Kombinerer de to halvdele ved at flette dem sammen
    while (firstHalf != originalDeck->top && secondHalf != originalDeck->top) {
        if (firstHalf != originalDeck->top) {
            *current = firstHalf;
            firstHalf->prev = dummyNode;
            firstHalf = firstHalf->next;
            current = &((*current)->next);
            newDeck->size++;
        }
        if (secondHalf != originalDeck->top) {
            *current = secondHalf;
            secondHalf->prev = *current;
            secondHalf = secondHalf->next;
            current = &((*current)->next);
            newDeck->size++;
        }
    }

    // Tilføjer resten af kortene fra den halvdel, der ikke er tømt
    while (firstHalf != originalDeck->top) {
        *current = firstHalf;
        firstHalf->prev = *current;
        firstHalf = firstHalf->next;
        current = &((*current)->next);
        newDeck->size++;
    }
    while (secondHalf != originalDeck->top) {
        *current = secondHalf;
        secondHalf->prev = *current;
        secondHalf = secondHalf->next;
        current = &((*current)->next);
        newDeck->size++;
    }

    // Luk cirklen ved at forbinde sidste kort til dummyNode
    dummyNode->prev = *current;
    (*current)->next = dummyNode;

    return newDeck;
}
