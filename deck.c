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
            CardNode *newNode = malloc(sizeof(CardNode));
            if(newNode == NULL){
                fprintf(stderr, "Unable to allocate memory for new CardNode\n");
                exit(EXIT_FAILURE);
            }
            newNode->card.suit = suits[i];
            newNode->card.value = values[j];
            newNode->card.isVisible = true; // Opdater til det nye feltnavn
            newNode->next = deck->top;
            deck->top = newNode;
            deck->size++;
        }
    }
}


void shuffleDeck(Deck *deck) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }

    if (deck->size < 2) return;


    // Opret et array til pointere af CardNode for nem adgang
    CardNode *nodesArray[deck->size];
    CardNode *current = deck->top;
    for (int i = 0; i < deck->size; i++) {
        nodesArray[i] = current;
        current = current->next;
    }

    // Brug Fisher-Yates shuffle algoritmen til at blande arrayet
    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // Bytter elementer
        CardNode *temp = nodesArray[i];
        nodesArray[i] = nodesArray[j];
        nodesArray[j] = temp;
    }

    // Genopbyg listerne med de blandet pointere
    deck->top = nodesArray[0];
    current = deck->top;
    for (int i = 1; i < deck->size; i++) {
        current->next = nodesArray[i];
        current = current->next;
    }
    current->next = NULL;  // Sæt det sidste elements 'next' til NULL
}
void freeDeck(Deck *deck) {
    CardNode *current = deck->top;
    while (current != NULL) {
        CardNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(deck);
}

bool isValidCard(const char *cardStr) {
    // Validerer et kort baseret på dit spils regler
    // Et eksempel på, hvad denne funktion kunne indeholde
    const char *validValues = "A23456789TJQK";
    const char *validSuits = "CDHS";

    // Tjek længden af kortet
    if (strlen(cardStr) != 2) {
        return false;
    }

    // Tjek om værdien er gyldig
    if (strchr(validValues, cardStr[0]) == NULL) {
        return false;
    }

    // Tjek om kuløren er gyldig
    if (strchr(validSuits, cardStr[1]) == NULL) {
        return false;
    }

    return true;
}


bool addCard(Deck *deck, const char value, const char suit) {
    CardNode *newCard = malloc(sizeof(CardNode));
    if (newCard == NULL) {
        fprintf(stderr, "Unable to allocate memory for new CardNode\n");
        return false;
    }

    newCard->card.value = value;
    newCard->card.suit = suit;
    newCard->card.isVisible = true; // Antag at alle nye kort er synlige når de tilføjes
    newCard->next = NULL;

    if (deck->top == NULL) {
        deck->top = newCard;
    } else {
        newCard->next = deck->top;
        deck->top = newCard;
    }

    deck->size++;
    return true;
}
Tableau* initializeTableau() {
    Tableau *tableau = malloc(sizeof(Tableau));
    if (tableau == NULL) {
        fprintf(stderr, "Unable to allocate memory for Tableau\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 7; i++) {
        tableau->columns[i] = malloc(sizeof(Deck));
        initializeDeck(tableau->columns[i]);
        // Her kan du tilføje kort til hver kolonne baseret på spillets regler
    }

    return tableau;
}
void dealToTableau(Deck *deck, Tableau *tableau) {
    int cardCount = 0;  // Holder styr på antallet af tildelte kort
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row <= col; row++) {
            if (deck->top == NULL) {
                fprintf(stderr, "Not enough cards in deck to deal to tableau\n");
                exit(EXIT_FAILURE);
            }

            // Fjern kortet fra dækket
            CardNode *cardToAdd = deck->top;
            deck->top = cardToAdd->next;
            deck->size--;

            // Sæt kortets synlighed baseret på om det er det øverste kort i kolonnen
            cardToAdd->card.isVisible = (row == col);

            // Tilføj kortet til den nuværende kolonne
            cardToAdd->next = tableau->columns[col]->top;
            tableau->columns[col]->top = cardToAdd;
            tableau->columns[col]->size++;

            cardCount++;
        }
    }

    // Tjek om alle kort er blevet tildelt korrekt
    if (cardCount != 52) {
        fprintf(stderr, "Error: Not all cards have been dealt to the tableau\n");
        exit(EXIT_FAILURE);
    }
}


void printTableau(Tableau *tableau) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    // Antag at vi har 7 kolonner
    int maxRows = 0;
    for (int i = 0; i < 7; i++) {
        int columnSize = tableau->columns[i]->size;
        maxRows = (columnSize > maxRows) ? columnSize : maxRows;
    }

    // Gå igennem hver række
    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            Deck *columnDeck = tableau->columns[col];
            CardNode *currentNode = columnDeck->top;

            // Find kortet på den nuværende position (row)
            for (int cardPos = 0; cardPos < row && currentNode != NULL; cardPos++) {
                currentNode = currentNode->next;
            }

            // Print kortet eller tom plads
            if (currentNode != NULL) {
                if (currentNode->card.isVisible) {
                    printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
            } else {
                printf("\t");
            }
        }
        printf("\n");
    }

    printf("\nLAST Command: \nMessage: \nINPUT > ");
}
