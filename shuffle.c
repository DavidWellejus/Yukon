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
        srand((unsigned int)time(NULL));
        splitPoint = rand() % (originalDeck->size - 1) + 1;
    }

    Node* firstHalf = originalDeck->top->next; // Skip dummy node
    Node* secondHalf = firstHalf;

    // Move secondHalf to the start of the second split
    for (int i = 0; i < splitPoint; i++) {
        secondHalf = secondHalf->next;
    }

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

    // Interleave cards from firstHalf and secondHalf
    while (firstHalf != originalDeck->top && secondHalf != originalDeck->top) {
        if (firstHalf->isDummy) {
            firstHalf = originalDeck->top; // Stop if reached dummy
        } else {
            current->next = firstHalf;
            firstHalf->prev = current;
            current = firstHalf;
            firstHalf = firstHalf->next;
            newDeck->size++;
        }

        if (secondHalf->isDummy) {
            secondHalf = originalDeck->top; // Stop if reached dummy
        } else if (newDeck->size < 52) {
            current->next = secondHalf;
            secondHalf->prev = current;
            current = secondHalf;
            secondHalf = secondHalf->next;
            newDeck->size++;
        }
    }

    // If one split still has cards (excluding dummy), add them
    Node* remaining = (firstHalf != originalDeck->top) ? firstHalf : secondHalf;
    while (remaining != originalDeck->top && newDeck->size < 52) {
        if (remaining->isDummy) {
            break; // Do not include dummy node
        }
        current->next = remaining;
        remaining->prev = current;
        current = remaining;
        remaining = remaining->next;
        newDeck->size++;
    }

    // Close the loop of the new deck
    current->next = dummyNode;
    dummyNode->prev = current;

    freeDeck(originalDeck);
    return newDeck;
}


