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

    Node* dummyNode = deck->top->next;
    newNode->card.value = value;
    newNode->card.suit = suit;
    newNode->card.isVisible = false;
    newNode->isDummy = false;

    Node* lastRealNode = deck->top;
    while (lastRealNode->next != deck->top) {
        lastRealNode = lastRealNode->next;
    }

    newNode->next = deck->top;
    newNode->prev = lastRealNode;
    lastRealNode->next = newNode;
    deck->top->prev = newNode;

    deck->size++;
    return true;
}


void printDeck(Deck* deck){
    Node* current = deck->top;
    for (int i = 0; i < deck->size; i++){
        printf("%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }
}