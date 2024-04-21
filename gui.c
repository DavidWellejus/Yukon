//
// Created by saleh on 17-04-2024.
//

#include "gui.h"
#include <gtk/gtk.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"


//funktion når knappen bliver klikket:
//static void on_button_clicked(GtkWidget *widget, gpointer data) {
   // g_print("Knap blev trykket!\n");

//}

static void on_button_clicked(GtkWidget *widget, gpointer data) {

    g_print("Knap blev trykket!\n");
   // deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
   // clearTable(table);
    //dealToStartTable(deck, table);
  //  printTable(table, command);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    // Startvinduet (Denne her skal der IKKE PILLES VED !!!!!)
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Yukon Card Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Opret et grid til at arrangere widgets
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Du kan tilføje flere GUI-komponenter her, f.eks. knapper, labels osv.
    //Her oprettes knappen:
    button = gtk_button_new_with_label("LD");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    //Her indsættes knappen i gridedet:
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    gtk_widget_show_all(window);
}