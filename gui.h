#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"

// Definer en struktur til at holde applikationsdata
typedef struct {
    Deck *deck;
    Table *table;
    GtkWidget *grid;
} ApplicationData;

// Deklarationer af callback funktioner
void on_button_clicked(GtkWidget *widget, gpointer data);
void on_button_sw_clicked(GtkWidget *widget, gpointer data);
void on_button_si_clicked(GtkWidget *widget, gpointer data);
void on_button_sr_clicked(GtkWidget *widget, gpointer data);
void display_cards(GtkWidget *grid, Table *table);
GtkWidget* create_scaled_image(const char* file_path, int width, int height);

// Deklaration af aktiveringsfunktion
void activate(GtkApplication *app, gpointer user_data);

#endif // GUI_H
