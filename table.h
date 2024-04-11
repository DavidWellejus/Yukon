//
// Created by david on 11-Apr-24.
//

#ifndef YUKON_TABLE_H
#define YUKON_TABLE_H

#include "deck.h"

Table* initializeTable();
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table);
void setShowAllCards(Table *table, bool isVisible);

#endif //YUKON_TABLE_H
