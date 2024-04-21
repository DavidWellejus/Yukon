//
// Created by david on 11-Apr-24.
//

#ifndef YUKON_TABLE_H
#define YUKON_TABLE_H

#include "deck.h"

Table* initializeTable();
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table, char lastCommand[256]);
void setShowAllCards(Table *table, bool isVisible);
void dealToGameTable(Table* table, Deck* deck);
void clearTable(Table *table);
void moves(Table* table, char command[256]);
int convertValue(char value);
void movesCol(Table* table, char command[256]);

#endif //YUKON_TABLE_H
