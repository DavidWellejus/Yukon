//
// Created by david on 11-Apr-24.
//

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "shuffle.h"

Deck* shuffleDeck(Deck *deck) {
    Deck* shuffledDeck = malloc(sizeof(Deck));
    if (shuffledDeck == NULL) {
        fprintf(stderr, "Unable to allocate memory for new Deck\n");
        exit(EXIT_FAILURE);
    }

    initializeDeck(shuffledDeck);
    Node* currentCard = deck->top->next;

    while(currentCard != deck->top){
        if(shuffledDeck->size == 0){
            Node* newNode = malloc(sizeof(Node));

            if (newNode == NULL) {
                fprintf(stderr, "Unable to allocate memory for Node\n");
                exit(EXIT_FAILURE);
            }

            *newNode = *currentCard;
            newNode->next = shuffledDeck->top;
            newNode->prev = shuffledDeck->top->prev;
            shuffledDeck->top->prev->next = newNode;
            shuffledDeck->top->prev = newNode;
            shuffledDeck->size++;

            currentCard = currentCard->next;
        }
        else {

            Node* positionNode = shuffledDeck->top;
            for(int i = 0; i < (rand() % shuffledDeck->size) + 1; i++){
                positionNode = positionNode->next;
            }
            Node* newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Unable to allocate memory for Node\n");
                exit(EXIT_FAILURE);
            }

            *newNode = *currentCard;
            newNode->next = positionNode;
            newNode->prev = positionNode->prev;
            positionNode->prev->next = newNode;
            positionNode->prev = newNode;
            shuffledDeck->size++;

            currentCard = currentCard->next;

        }
    }
        return shuffledDeck;
}


Deck* splitter(Deck* originalDeck, int splitPoint) {
    if (originalDeck == NULL || originalDeck->size < 2) {
        return NULL;
    }

    if (splitPoint <= 0 || splitPoint >= originalDeck->size) {
        splitPoint = rand() % (originalDeck->size - 1) + 1;
    }

    Node* firstHalf = originalDeck->top->next;
    Node* secondHalf = firstHalf;


    for (int i = 0; i < splitPoint; i++) {
        secondHalf = secondHalf->next;
    }

    Node* temp = secondHalf->prev;
    secondHalf->prev = originalDeck->top;
    temp->next = originalDeck->top;

    Deck* newDeck = malloc(sizeof(Deck));
    if (newDeck == NULL) {
        fprintf(stderr, "Unable to allocate memory for new Deck\n");
        exit(EXIT_FAILURE);
    }

    Node *dummyNode = malloc(sizeof(Node));
    if (dummyNode == NULL) {
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

    Node* current = dummyNode;

    while ((firstHalf != originalDeck->top && secondHalf != originalDeck->top) && newDeck->size < splitPoint*2) {

        if (!firstHalf->isDummy && firstHalf != originalDeck->top && newDeck->size < splitPoint*2) {
            Node* newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate node\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *firstHalf;
            newNode->next = dummyNode->next;
            newNode->prev = dummyNode;
            dummyNode->next->prev = newNode;
            dummyNode->next = newNode;
            newDeck->size++;
            firstHalf = firstHalf->next;
        }

        if (!secondHalf->isDummy && secondHalf != originalDeck->top && newDeck->size < splitPoint*2) {
            Node* newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate node\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *secondHalf;
            newNode->next = dummyNode->next;
            newNode->prev = dummyNode;
            dummyNode->next->prev = newNode;
            dummyNode->next = newNode;
            newDeck->size++;
            secondHalf = secondHalf->next;
        }
    }

    Node* remaining = (firstHalf != originalDeck->top) ? firstHalf : secondHalf;


    while (remaining != originalDeck->top && newDeck->size < 52) {
        if (!remaining->isDummy) {
            Node* newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate node\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *remaining;
            newNode->prev = dummyNode->prev;
            newNode->next = dummyNode;
            dummyNode->prev->next = newNode;
            dummyNode->prev = newNode;
            newDeck->size++;
            remaining = remaining->next;
        }
    }

    dummyNode->next->prev = dummyNode;
    dummyNode->prev->next = dummyNode;

    return newDeck;

}