// main.h

#ifndef YUKON_MAIN_H
#define YUKON_MAIN_H

#include <gtk/gtk.h>
#include "deck.h"
#include "table.h"

// Deklaration af funktioner, der bruges i main.c og andre dele af programmet
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
void cleanup(GApplication *app, gpointer user_data);

#endif // YUKON_MAIN_H
