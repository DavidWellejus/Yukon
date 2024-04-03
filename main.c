#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


typedef struct Card {
    char value; // A, 2-9, T, J, Q, K
    char suit;  // C, D, H, S
    bool isHidden;
} Card;

typedef struct CardNode {
    Card card;
    struct CardNode* next;
} CardNode;

typedef struct Deck {
    CardNode *head;
    int size;
} Deck;

void printStartupScreen();
void initializeDeck(Deck *deck);
void printDeck(const Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
void shuffleDeck(Deck *deck);
bool isValidCard(const char *cardStr);
bool loadDeckFromFile(Deck *deck, const char *filename);
void printHiddenDeck(Deck *deck);
bool addCard(Deck *deck, const char value, const char suit);

int main() {
    Deck deck;
    initializeDeck(&deck);

    srand(time(NULL));
    shuffleDeck(&deck);
    printDeck(&deck);

    saveDeckToFile(&deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/Lab/project2_machine/Yukon/deckYukon.txt");
    printStartupScreen();
    char command[3];
    char filename[256];

    if(scanf("%2s %255s", command, filename) == 2){

        if(strcmp(command, "LD") == 0){
            loadDeckFromFile(&deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/Lab/project2_machine/Yukon/deckYukon.txt");

            printHiddenDeck(&deck);
        }
    }



    // Husk at frigive hukommelsen for dækket her
    return 0;
}


//Bare for at tjekke at dækket bliver lave korrekt.
void printDeck(const Deck *deck) {
    CardNode *current = deck->head;
    while (current != NULL) {
        printf("%c%c ", current->card.value, current->card.suit);
        current = current->next;
    }
    printf("\n");
}

void initializeDeck(Deck * deck){
    deck->head = NULL;
    deck->size = 0;
    char suits[] = {'C', 'D', 'H', 'S'};
    char value[] = {'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'A'};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            CardNode *newNode = (CardNode*)malloc(sizeof(CardNode));
            if(newNode == NULL){
                fprintf(stderr, "Unable to allocate memory for new CardNode\n");
                exit(EXIT_FAILURE);
            }
            newNode->card.suit = suits[i];
            newNode->card.value = value[j];
            newNode->card.isHidden = true;
            newNode->next = deck->head;
            deck->head = newNode;
            deck->size++;

        }
    }
}
void printStartupScreen() {

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    printf("\t\t\t\t\t\t\t[] F1\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F2\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F3\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F4\n\n");

    printf("LAST Command:\n");
    printf("Message:\n");
    printf("INPUT > ");
}

bool saveDeckToFile(const Deck *deck, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return false;
    }

    CardNode *current = deck->head;
    while (current != NULL) {
        fprintf(file, "%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }

    fclose(file);
    return true;
}

void shuffleDeck(Deck *deck) {
    if (deck->size < 2) return;  // Ingen grund til at blande, hvis der er 0 eller 1 kort

    // Opret et array til pointere af CardNode for nem adgang
    CardNode *nodesArray[deck->size];
    CardNode *current = deck->head;
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
    deck->head = nodesArray[0];
    current = deck->head;
    for (int i = 1; i < deck->size; i++) {
        current->next = nodesArray[i];
        current = current->next;
    }
    current->next = NULL;  // Sæt det sidste elements 'next' til NULL
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

bool loadDeckFromFile(Deck *deck, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return false;
    }

    char cardStr[4]; // 2 characters for the card and 1 for the null-terminator
    int line = 0;
    bool success = true;

    while (fgets(cardStr, sizeof(cardStr), file) && line < 52) {
        if (cardStr[strlen(cardStr) - 1] == '\n') {
            cardStr[strlen(cardStr) - 1] = '\0'; // Remove newline character
        }

        if (!isValidCard(cardStr)) {
            fprintf(stderr, "Error: Invalid card at line %d.\n", line + 1);
            success = false;
            break;
        }

        if (!addCard(deck, cardStr[0], cardStr[1])) {
            success = false;
            break; // Afslut hvis der opstår en fejl ved tilføjelse af kort
        }

        line++;
    }

    if (line != 52) {
        fprintf(stderr, "Error: Incorrect number of cards. Expected 52, got %d.\n", line);
        success = false;
    }

    fclose(file);
    return success;
}
void printHiddenDeck(Deck *deck) {
    // Antallet af kort der skal vises i hver kolonne.
    int cardsInColumns[] = {8, 8, 8, 7, 7, 7, 7};

    // Udskriver kolonneoverskrifterne
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");

    // Find det maksimale antal kort i en kolonne
    int maxRows = cardsInColumns[0]; // Start med antallet af kort i første kolonne
    int sideRow = 1;

    // Print de skjulte kort i hver kolonne og tilføj F1-F4 til højre
    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            if (row < cardsInColumns[col]) {
                printf("[ ]\t");
            } else {
                printf("\t");
            }
        }

        // Tilføj fundamenterne på samme linje, til højre
        if (row % 2 == 0) {
            printf("[ ] F%d", sideRow);
            sideRow += 1;
        }
        printf("\n");
    }
    printf("LAST Command: LD\n");
    printf("Message: OK\n");
    printf("INPUT > ");
}
bool addCard(Deck *deck, const char value, const char suit) {
    // Opret et nyt kortnode
    CardNode *newCard = (CardNode*)malloc(sizeof(CardNode));
    if (newCard == NULL) {
        fprintf(stderr, "Unable to allocate memory for new CardNode\n");
        return false;
    }

    // Sæt værdierne for det nye kort
    newCard->card.value = value;
    newCard->card.suit = suit;
    newCard->next = NULL;

    // Tilføj det nye kort til toppen af dækket
    if (deck->head == NULL) { // Dækket er tomt
        deck->head = newCard;
    } else { // Dækket indeholder allerede kort
        newCard->next = deck->head;
        deck->head = newCard;
    }

    deck->size++; // Opdater størrelsen af dækket
    return true;
}
