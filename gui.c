//
// Created by saleh on 17-04-2024.
//

#include "gui.h"
#include <gtk/gtk.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"



//Den opringdelige funktion som virker, men er udkommenteret i forsøg på at implementere funktion til knap:
//funktion når knappen bliver klikket:
//static void on_button_clicked(GtkWidget *widget, gpointer data) {
   // g_print("Knap blev trykket!\n");

//}

//Her er "LD-funktionen" som vi vil aktivere når knappen biver trykket:
// deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
// clearTable(table);
//dealToStartTable(deck, table);
//  printTable(table, command);

//Callback funktionen til LD knappen:
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("LD-knappen blev trykket!\n");

    // Sørger for at data pointeren er gyldig og castes til den korrekte type
    ApplicationData *appData = (ApplicationData *)data;
    if (appData == NULL) {
        g_print("Applikationsdata er ikke tilgængelig\n");
        return;
    }

    // Indlæser kortdækket fra filen
    appData->deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
    if (appData->deck == NULL) {
        g_print("Kunne ikke indlæse kortdækket.\n");
        return;
    }

    // Rydder bordet og fordeler kortene
    clearTable(appData->table);
    dealToStartTable(appData->deck, appData->table);

    // Opdaterer bordvisningen
    printTable(appData->table, "LD");
}

//Callback funktionen til SW knappen:
// Tilføj denne nye funktion i gui.c
static void on_button_sw_clicked(GtkWidget *widget, gpointer data) {
    ApplicationData *appData = (ApplicationData *)data;
    if (appData == NULL) {
        g_print("Applikationsdata er ikke tilgængelig\n");
        return;
    }
    if (appData->table == NULL) {
        g_print("Table data er ikke tilgængelig\n");
        return;
    }

    // Sæt alle kort til at være synlige
    setShowAllCards(appData->table, true);

    // Opdater bordvisningen (antag at printTable er tilpasset til at håndtere 'SW' som command)
    printTable(appData->table, "SW");
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *buttonSW;

    //Her starter den nye kode:
    ApplicationData *appData = g_new0(ApplicationData, 1);

    // Initialiserer appData strukturen
    appData->deck = NULL;
    appData->table = initializeTable();

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
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), appData);

    //Knappen til SW:

    //Først oprettes en knap med tilhørende tekst PÅ knappen:
    buttonSW = gtk_button_new_with_label("SW");

    //Derefter hvilken funktion der skal køre når man trykker på knappen:
    g_signal_connect(buttonSW, "clicked", G_CALLBACK(on_button_sw_clicked), appData);

    //Deretfer knappens placering, højde og bredde i "gitteret"
    gtk_grid_attach(GTK_GRID(grid), buttonSW, 1, 0, 1, 1);


    //Her indsættes knappen i gridedet:
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    gtk_widget_show_all(window);
}