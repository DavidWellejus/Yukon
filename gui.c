//
// Created by saleh on 17-04-2024.
//

#include "gui.h"
#include <gtk/gtk.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;

    // Startvinduet (Denne her skal der IKKE PILLES VED !!!!!)
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Yukon Card Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Opret et grid til at arrangere widgets
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Du kan tilføje flere GUI-komponenter her, f.eks. knapper, labels osv.

    gtk_widget_show_all(window);
}