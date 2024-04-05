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
    char suits[] = {'S', 'H', 'D', 'C'};
    char values[] = {'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'A'};

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
        tableau->columns[i]->top = NULL;
        tableau->columns[i]->size = 0;
        // Her kan du tilføje kort til hver kolonne baseret på spillets regler
    }

    return tableau;
}
void dealToTableau(Deck *deck, Tableau *tableau) {
    CardNode *current = deck->top;

    // Starter ved kolonne 0 og går til 6, så gentages.
    int colIndex = 0;

    while (current != NULL) {
        // Gemmer det nuværende kort for at tilføje det til kolonnen.
        CardNode *cardToAdd = current;
        // Går til det næste kort i deck.
        current = current->next;
        // Sætter det nuværende korts næste pegepind til NULL, da det vil være den nye 'top'.
        cardToAdd->next = NULL;

        // Tilføjer kortet til toppen af den pågældende kolonne.
        if (tableau->columns[colIndex]->top == NULL) {
            // Hvis der ikke er nogen kort i kolonnen, bliver dette kort det første.
            tableau->columns[colIndex]->top = cardToAdd;
        } else {
            // Finder det sidste kort i kolonnen.
            CardNode *lastCard = tableau->columns[colIndex]->top;
            while (lastCard->next != NULL) {
                lastCard = lastCard->next;
            }
            // Tilføjer det nye kort efter det sidste kort i kolonnen.
            lastCard->next = cardToAdd;
        }

        // Sætter det øverste kort til at være synligt, hvis det er den første uddeling i kolonnen.
        if (tableau->columns[colIndex]->size == 0) {
            cardToAdd->card.isVisible = true;
        }

        // Opdaterer størrelsen på kolonnen.
        tableau->columns[colIndex]->size++;

        // Skifter til næste kolonne, går tilbage til den første efter den syvende.
        colIndex = (colIndex + 1) % 7;
    }

    // Sørger for at deck pegepinden 'top' opdateres til at pege på NULL, da alle kort er blevet delt.
    deck->top = NULL;
}





void printTableau(Tableau *tableau) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    // Find den længste kolonne for at bestemme antallet af rækker, der skal udskrives
    int maxRows = 0;
    for (int col = 0; col < 7; col++) {
        int columnSize = tableau->columns[col]->size;
        maxRows = (columnSize > maxRows) ? columnSize : maxRows;
    }

    // For hver række
    for (int row = 0; row < maxRows; row++) {
        // For hver kolonne
        for (int col = 0; col < 7; col++) {
            // Få den aktuelle node, der skal udskrives
            CardNode *currentNode = tableau->columns[col]->top;
            for (int depth = 0; currentNode != NULL && depth < row; depth++) {
                currentNode = currentNode->next;
            }

            // Hvis det aktuelle kort findes, og vi er på det dybde i listen, der matcher rækken, udskriv det
            if (currentNode != NULL) {
                if (currentNode->card.isVisible) {
                    printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
            } else {
                // Hvis der ikke er flere kort i denne kolonne, udskriv et tomt felt
                printf("\t");
            }
        }
        printf("\n");
    }

    printf("\nLAST Command: \nMessage: \nINPUT > ");
}

