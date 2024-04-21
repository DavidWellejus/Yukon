#ifndef YUKON_GUI_H
#define YUKON_GUI_H

#include "deck.h"  // Antager at denne fil definerer Deck strukturen
#include "table.h" // Antager at denne fil definerer Table strukturen

typedef struct {
    Deck *deck;
    Table *table;
} ApplicationData;

#endif // YUKON_GUI_H
