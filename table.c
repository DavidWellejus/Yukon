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
    Node *cardToDeal = deck->top->next;

    while (cardToDeal != deck->top && cardCount < 52) {
        for (int i = 0; i < 7; ++i) {
            if (cardToDeal == deck->top) break;

            Node *newNode = malloc(sizeof(Node));
            if (newNode == NULL) {
                fprintf(stderr, "Failed to allocate memory for a new card node.\n");
                exit(EXIT_FAILURE);
            }
            *newNode = *cardToDeal;
            newNode->next = newNode->prev = NULL;

            Node *columnDummy = table->columns[i];
            Node *lastCardInColumn = columnDummy->prev;

            newNode->prev = lastCardInColumn;
            newNode->next = columnDummy;
            lastCardInColumn->next = newNode;
            columnDummy->prev = newNode;

            cardToDeal = cardToDeal->next;
            cardCount++;
        }
    }

    if (cardCount < 52) {
        fprintf(stderr, "Error: Not all cards have been distributed from the deck.\n");
    }
}


void printTable(Table *table, char lastCommand[256], char message[256]) {
    if (table == NULL) {
        return;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    bool allColumnsDone;
    int row = 0;
    int i = 0;
    do {
        allColumnsDone = true;
        for (int col = 0; col < 7; col++) {
            Node *currentNode = table->columns[col]->next;

            for (int depth = 0; depth < row && currentNode != table->columns[col]; depth++) {
                currentNode = currentNode->next;
            }

            if (currentNode != table->columns[col] && !currentNode->isDummy) {
                if (currentNode->card.isVisible) {
                    printf("%c%c\t", currentNode->card.value, currentNode->card.suit);
                } else {
                    printf("[ ]\t");
                }
                allColumnsDone = false;
            } else {
                printf("\t");
            }
        }

        if (row % 2 == 0 && i < 4) {
            Node *foundationNode = table->columns[7 + row / 2]->next;
            if (!foundationNode->isDummy) {
                printf("\t %c%c F%d\t", foundationNode->card.value, foundationNode->card.suit, 1 + row / 2);
            } else {
                printf("\t [ ] F%d\t", 1 + row / 2);
            }
            i++;
        } else {
            printf("\t\t\t\t\t");
        }

        printf("\n");
        row++;
    } while (!allColumnsDone);

    if(row < 8){
        while(i < 4){
            if (row % 2 == 0) {
                Node *foundationNode = table->columns[7 + row / 2]->next;
                if (!foundationNode->isDummy) {
                    printf("\t\t\t\t\t\t\t\t %c%c F%d\n", foundationNode->card.value, foundationNode->card.suit, 1 + row / 2);
                } else {
                    printf("\t\t\t\t\t\t\t\t [ ] F%d\n", 1 + row / 2);
                }
                i++;
                row++;
            } else {
                printf("\n");
                row++;
            }
        }
    }
    printf("\nLAST Command: %s\nMessage: %s\n", lastCommand, message);
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
void dealToGameTable(Table* table, Deck* deck) {
    Node *cardToDeal = deck->top->next;
    int cards_in_column[7] = {1, 6, 7, 8, 9, 10, 11};
    int cardsDealtToColumn[7] = {0};

    while (cardToDeal != deck->top) {
        for (int i = 0; i < 7; i++) {
            if (cardsDealtToColumn[i] < cards_in_column[i]) {
                Node *newNode = malloc(sizeof(Node));
                if (newNode == NULL) {
                    fprintf(stderr, "Failed to allocate memory for a new card node.\n");
                    exit(EXIT_FAILURE);
                }

                *newNode = *cardToDeal;
                newNode->card.isVisible = (cardsDealtToColumn[i] >= i);

                Node *lastCardInColumn = table->columns[i];
                while (lastCardInColumn->next != table->columns[i]) {
                    lastCardInColumn = lastCardInColumn->next;
                }

                newNode->next = table->columns[i];
                newNode->prev = lastCardInColumn;
                lastCardInColumn->next = newNode;
                table->columns[i]->prev = newNode;

                cardToDeal = cardToDeal->next;
                cardsDealtToColumn[i]++;
                if (cardToDeal == deck->top) break;
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        if (cardsDealtToColumn[i] != cards_in_column[i]) {
            fprintf(stderr, "Error: Not all cards were distributed correctly in column %d.\n", i+1);
        }
    }
}



void clearTable(Table *table) {
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < 11; i++) {
        Node *current = table->columns[i]->next;
        while (current != table->columns[i]) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
        table->columns[i]->next = table->columns[i];
        table->columns[i]->prev = table->columns[i];
    }
}
bool moves(Table* table, char command[256]){
    Node* colFrom;
    if(command[0] == 'F'){
        colFrom = table->columns[convertValue(command[1]) + 6];
    } else {
        colFrom = table->columns[convertValue(command[1]) - 1];
    }

    Node* colTo;
    if(command[7] == 'F'){
        colTo = table->columns[convertValue(command[8]) + 6];
    } else{
        colTo = table->columns[convertValue(command[8]) - 1];
    }

    char cardValueFrom = command[3];
    char cardSuitFrom = command[4];
    char cardSuitTo = colTo->prev->card.suit;
    char cardValueTo = colTo->prev->card.value;
    int currentValueInt;
    int cardValueToInt;
    bool moveMade = false;

    Node* current = colFrom->next;

    while(current != colFrom && (cardValueFrom != current->card.value || cardSuitFrom != current->card.suit)){
        current = current->next;
    }
    Node* lastCardInCol = current->next;
    int t = 0;
    while (!lastCardInCol->next->isDummy){
        lastCardInCol = lastCardInCol->next;
        t++;
    }

    if(!current->isDummy){
        currentValueInt = convertValue(current->card.value);
        cardValueToInt = convertValue(cardValueTo);

        if(command[7] == 'F' && (cardSuitFrom == cardSuitTo || (colTo->isDummy && cardValueToInt + 48 == currentValueInt - 1 && current == lastCardInCol))){
            current->prev->next = colFrom;
            colFrom->prev = current->prev;

            Node* topCard = colTo->next;
            colTo->next = current;
            current->prev = colTo;
            current->next = topCard;
            topCard->prev = current;
            moveMade = true;
        }

        if(cardSuitFrom != cardSuitTo && cardValueToInt > currentValueInt || (cardValueToInt + 48 == 0 && currentValueInt == 13)){
            current->prev->next = colFrom;
            colFrom->prev = current->prev;

            lastCardInCol->next = colTo;
            current->prev = colTo->prev;
            colTo->prev->next = current;
            current->next->prev = current;
            colTo->prev = lastCardInCol;
            moveMade = true;
        }
    }
    if(!colFrom->prev->card.isVisible){
        colFrom->prev->card.isVisible = true;
    }
    return moveMade;
}


bool movesCol(Table* table, char command[256]){
    char cardSuitTo;
    char cardSuitFrom;
    int cardValueTo;
    int cardValueFrom;
    Node* current;

    Node* colFrom;
    if(command[0] == 'F'){
        colFrom = table->columns[convertValue(command[1]) + 6];
    } else {
        colFrom = table->columns[convertValue(command[1]) - 1];
    }

    Node* colTo;
    if(command[4] == 'F'){
        colTo = table->columns[convertValue(command[5]) + 6];
    } else{
        colTo = table->columns[convertValue(command[5]) - 1];
    }

    cardSuitTo = colTo->prev->card.suit;
    cardSuitFrom = colFrom->prev->card.suit;
    if(command[4] == 'F'){cardValueTo = convertValue(colTo->next->card.value); }
    else{cardValueTo = convertValue(colTo->prev->card.value);}
    if(command[0] == 'F'){cardValueFrom = convertValue(colFrom->next->card.value);}
    else{cardValueFrom = convertValue(colFrom->prev->card.value);}
    if(command[0] == 'F'){current = colFrom->next;}
    else{current = colFrom->prev;}
    bool moveMade = false;

    if(command[4] == 'F'){
        if((cardSuitFrom == cardSuitTo && cardValueTo == cardValueFrom - 1) || (colTo->next->isDummy && cardValueTo + 48 == cardValueFrom - 1)){
            current->prev->next = colFrom;
            colFrom->prev = current->prev;

            Node* topCard = colTo->next;
            colTo->next = current;
            current->prev = colTo;
            current->next = topCard;
            topCard->prev = current;
            moveMade = true;
        }
    }

    else if(command[0] == 'F'){
        if(cardSuitFrom != cardSuitTo && cardValueTo - 1 == cardValueFrom){
            Node* cardToMove = colFrom->next;

            colFrom->next = cardToMove->next;
            cardToMove->next->prev = colFrom;

            // Tilføj kortet til mål-kolonnen
            cardToMove->next = colTo->prev;    // Kortet der flyttes bliver det nye top kort i kolonnen
            cardToMove->next = colTo;
            colTo->prev->next = cardToMove;
            colTo->prev = cardToMove;

            moveMade = true;
        }
    }

    else if(cardSuitFrom != cardSuitTo && cardValueTo - 1 == cardValueFrom || (cardValueTo + 48 == 0 && cardValueFrom == 13)){
        current->prev->next = colFrom;
        colFrom->prev = current->prev;

        current->next = colTo;
        current->prev = colTo->prev;
        colTo->prev->next = current;
        current->next->prev = current;
        colTo->prev = current;
        moveMade = true;
    }

    if(!colFrom->prev->card.isVisible){
        colFrom->prev->card.isVisible = true;
    }

    return moveMade;
}



int convertValue(char value) {
    switch (value) {
        case 'A':
            return 1;
        case 'K':
            return 13;
        case 'Q':
            return 12;
        case 'J':
            return 11;
        case 'T':
            return 10;
        default:
            return value - '0';
    }
}