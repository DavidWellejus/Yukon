//
// Created by david on 04-Apr-24.
//

#include <stdbool.h>
#include "deck.h"

#ifndef YUKON_FILE_H
#define YUKON_FILE_H

#endif //YUKON_FILE_H
bool saveDeckToFile(const Deck *deck, const char *filename);
Deck* loadDeckFromFile(const char *filename);