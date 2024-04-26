

#include "gui.h"
#include <gtk/gtk.h>
#include "deck.h"
#include "file.h"
#include "shuffle.h"
#include "table.h"

char message[256]="OK";





//Her kommer callback funktionerne til de forskellige knapper:
//Callback funktionen til LD knappen:
 void on_button_clicked(GtkWidget *widget, gpointer data) {

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
    printTable(appData->table, "LD", message);
}

//Callback funktionen til SW knappen:
 void on_button_sw_clicked(GtkWidget *widget, gpointer data) {
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
    printTable(appData->table, "SW", message);
}

//Callbackfunktion til SI-knappen:
 void on_button_si_clicked(GtkWidget *widget, gpointer data) {
    ApplicationData *appData = (ApplicationData *)data;
    if (appData == NULL || appData->deck == NULL) {
        g_print("Applikationsdata eller deck er ikke tilgængelig\n");
        return;
    }

    // Implementer logikken for at splitte deck
    int splitPosition = rand() % (appData->deck->size - 1) + 1;  // Vælg en tilfældig split position
    Deck *newDeck = splitter(appData->deck, splitPosition);
    if (newDeck == NULL) {
        g_print("Fejl ved splitting af deck\n");
        return;
    }

    free(appData->deck);  // Frigiv det gamle deck
    appData->deck = newDeck;  // Opdaterer med det nye splittede deck

    clearTable(appData->table);  // Ryd bordet
    dealToStartTable(appData->deck, appData->table);  // Fordel kortene igen
    printTable(appData->table, "SI", message);  // Vis bordet
}

// Callback funktionen til SR-knappen:
 void on_button_sr_clicked(GtkWidget *widget, gpointer data) {
    ApplicationData *appData = (ApplicationData *)data;
    if (appData == NULL || appData->deck == NULL) {
        g_print("Applikationsdata eller deck er ikke tilgængelig\n");
        return;
    }

    // Shuffle logik
    Deck *shuffledDeck = shuffleDeck(appData->deck);
    if (shuffledDeck == NULL) {
        g_print("Fejl ved at blande deck\n");
        return;
    }

    free(appData->deck);  // Frigiv det gamle deck hvis nødvendigt
    appData->deck = shuffledDeck;  // Opdater med det nye shuffled deck

    clearTable(appData->table);  // Ryd bordet
    dealToStartTable(appData->deck, appData->table);  // Fordel kortene igen
    printTable(appData->table, "SR", message);  // Vis bordet
}

//Callback funktionen til P-knappen:
void on_button_p_clicked(GtkWidget *widget, gpointer data) {
    ApplicationData *appData = (ApplicationData *)data;
    g_print("Button P clicked\n");
    if (appData == NULL) {
        g_print("Application data is not available\n");
        return;
    }

    if (appData->deck == NULL || appData->table == NULL) {
        g_print("Deck or table data is not available\n");
        return;
    }
    g_print("Deck and table data are available\n");

    // Sørg for, at clearTable og dealToGameTable håndterer NULL værdier og andre fejl korrekt.
    clearTable(appData->table);
    g_print("Table cleared\n");
    dealToGameTable(appData->table, appData->deck);
    g_print("Cards dealt to game table\n");
    printTable(appData->table, "P", "Play mode activated.");
    g_print("Table printed\n");
}




 void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *buttonSW;
    GtkWidget *buttonSI;
    GtkWidget *buttonSR;
    GtkWidget *buttonP;
    GtkWidget *image1;
    GtkWidget *image2;




    //Her kommer koden til den grønne baggrund:
    // Opretter en CSS provider
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(cssProvider,
                                    "window { background-color: green; }", -1, NULL);

    // Tilføjer CSS provideren til alle widgets
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    //Her starter den nye kode:
    ApplicationData *appData = g_new0(ApplicationData, 1);

    // Initialiserer appData strukturen
    appData->deck = NULL;
    appData->table = initializeTable();

    // Startvinduet (Denne her skal der IKKE PILLES VED !!!!!)
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Yukon Card Game-ORIGINALERNE!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Opret et grid til at arrangere widgets
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);



    //Her tilføjes en box og billede:
   // box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    image1 = gtk_image_new_from_file("C:\\DTU\\2.Semester\\02322MaskinaerProgrammering\\lab\\project2_machine\\Yukon\\spillekort\\AS.png");
    image2 = gtk_image_new_from_file("C:\\DTU\\2.Semester\\02322MaskinaerProgrammering\\lab\\project2_machine\\Yukon\\spillekort\\AS.png");

     // Tilføjer billeder til boxen
     //gtk_box_pack_start(GTK_BOX(box), image1, TRUE, TRUE, 0);


     gtk_grid_attach(GTK_GRID(grid), image1, 1, 2, 1, 1);
     gtk_grid_attach(GTK_GRID(grid), image2, 1, 3, 1, 1);




     // Her tilføjes de forskellige knapper:
    //Knappen til LD:
    //Først oprettes en knap med tilhørende tekst PÅ knappen:
    button = gtk_button_new_with_label("LD");

    //Derefter hvilken funktion der skal kaldes når man trykker på knappen:
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), appData);

    //Deretfer knappens placering, højde og bredde i "gitteret"
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);


    //Knappen til SW:
    buttonSW = gtk_button_new_with_label("SW");
    g_signal_connect(buttonSW, "clicked", G_CALLBACK(on_button_sw_clicked), appData);
    gtk_grid_attach(GTK_GRID(grid), buttonSW, 1, 0, 1, 10);

    //Knappen til SI:
    buttonSI = gtk_button_new_with_label("SI");
    g_signal_connect(buttonSI, "clicked", G_CALLBACK(on_button_si_clicked), appData);  // Sætter callback funktionen for SI knappen
    gtk_grid_attach(GTK_GRID(grid), buttonSI, 2, 0, 1, 1);

    //Knappen til SR:
    buttonSR= gtk_button_new_with_label("SR");
    g_signal_connect(buttonSR, "clicked", G_CALLBACK(on_button_sr_clicked), appData);
    gtk_grid_attach(GTK_GRID(grid), buttonSR, 3, 0, 1, 1);

    //Knappen til P:
    buttonP = gtk_button_new_with_label("P");
    g_signal_connect(buttonP, "clicked", G_CALLBACK(on_button_p_clicked), user_data);
    gtk_grid_attach(GTK_GRID(grid), buttonP, 4, 0, 1, 1);




     gtk_widget_show_all(window);
}