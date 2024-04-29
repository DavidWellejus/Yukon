// main.c
#include "main.h"
#include "gui.h"

// Global variabel til at holde applikationsdata
ApplicationData *appData;


int main(int argc, char **argv) {

    srand(time(NULL));

    // Initialiser applikationsdata og tjek for fejl
    appData = g_new0(ApplicationData, 1);
    appData->deck = malloc(sizeof(Deck));
    appData->table = initializeTable();
    if (appData->deck == NULL || appData->table == NULL) {
        fprintf(stderr, "Failed to allocate memory for deck or table.\n");
        exit(EXIT_FAILURE);
    }

    // Opret og konfigurer GTK applikation
    GtkApplication *app = gtk_application_new("org.example.yukon", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), appData);
    g_signal_connect(app, "shutdown", G_CALLBACK(cleanup), NULL);

    // Kør GTK applikation
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Oprydning efter GTK applikationen er afsluttet
    g_object_unref(app);
    if (appData) {
        if (appData->deck) free(appData->deck);
        if (appData->table) free(appData->table);
        g_free(appData);
    }

    return status;
}

void cleanup(GApplication *app, gpointer user_data) {
    if (appData) {
        if (appData->deck) free(appData->deck); // Antag at du har en funktion til at frigøre dæk
        if (appData->table) free(appData->table); // Antag at du har en funktion til at frigøre tabel
        g_free(appData);
        appData = NULL;
    }
}

