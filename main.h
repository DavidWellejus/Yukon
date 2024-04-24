//
// Created by saleh on 24-04-2024.
//

#ifndef YUKON_MAIN_H
#define YUKON_MAIN_H

void printStartupScreen();
void initializeDeck(Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
Deck* shuffleDeck(Deck* deck);
Deck* loadDeckFromFile(const char *filename);
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table, char lastCommand[256], char message[256]);
void setShowAllCards(Table *table, bool isVisible);
Deck* splitter(Deck* deck, int split);
void printDeck(Deck* deck);
void dealToGameTable(Table* table, Deck* deck);
void clearTable(Table *table);
bool moves(Table* table, char command[256]);
bool movesCol(Table* table, char command[256]);
void stripNewline(char *str);

#endif //YUKON_MAIN_H
