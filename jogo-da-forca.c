#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define TAM_PALAVRA 100
#define MAX_TENTATIVAS 6

typedef struct {
    const char* pergunta;
    const char* resposta;
} Charada;

Charada listaCharadas[] = {
    {"O que é, o que é: cai em pé e corre deitado?", "chuva"},
    {"Tenho dentes, mas não mordo. O que sou?", "pente"},
    {"Qual é o animal que anda com um pé?", "sapo"},
    {"O que é feito para andar e não anda?", "rua"},
    {"O que tem cabeça, tem dente, tem barba, mas não é gente?", "alho"},
    {"O que passa diante do sol e não faz sombra?", "vento"},
    {"Quanto mais se tira, maior fica. O que é?", "buraco"},
    {"Qual é a coisa que quanto mais cresce, menor fica?", "buraco"},
    {"Anda deitado e dorme em pé?", "sapato"},
    {"Tem asa, mas não voa, bico, mas não bica?", "xicara"}
};

char palavra[TAM_PALAVRA];
char exibicao[TAM_PALAVRA];
char letrasUsadas[26];
int tentativasRestantes = MAX_TENTATIVAS;
int acertos = 0;
int letrasUsadasCount = 0;

GtkWidget *label_palavra;
GtkWidget *label_status;
GtkWidget *label_charada;
GtkWidget *canvas;

void atualizar_palavra() {
    gtk_label_set_text(GTK_LABEL(label_palavra), exibicao);
    char status[100];
    sprintf(status, "Tentativas restantes: %d", tentativasRestantes);
    gtk_label_set_text(GTK_LABEL(label_status), status);
    gtk_widget_queue_draw(canvas);
}

void fim_do_jogo(gboolean venceu) {
    GtkWidget *dialog;
    if (venceu) {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                        "🎉 Você acertou!");
    } else {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                        "💀 Você perdeu!\nResposta: %s", palavra);
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_main_quit();
}

void letra_clicada(GtkWidget *widget, gpointer data) {
    char letra = GPOINTER_TO_INT(data);
    letra = tolower(letra);

    if (tentativasRestantes <= 0 || acertos == strlen(palavra))
        return;

    if (strchr(letrasUsadas, letra))
        return;

    letrasUsadas[letrasUsadasCount++] = letra;

    int encontrou = 0;
    for (int i = 0; i < strlen(palavra); i++) {
        if (tolower(palavra[i]) == letra && exibicao[i] == '_') {
            exibicao[i] = palavra[i];
            acertos++;
            encontrou = 1;
        }
    }

    if (!encontrou) tentativasRestantes--;
    atualizar_palavra();

    if (acertos == strlen(palavra)) fim_do_jogo(TRUE);
    else if (tentativasRestantes == 0) fim_do_jogo(FALSE);
}

void iniciar_jogo() {
    srand(time(NULL));
    int index = rand() % (sizeof(listaCharadas) / sizeof(Charada));
    strcpy(palavra, listaCharadas[index].resposta);

    int tam = strlen(palavra);
    for (int i = 0; i < tam; i++)
        exibicao[i] = (palavra[i] == ' ') ? ' ' : '_';
    exibicao[tam] = '\0';

    gtk_label_set_text(GTK_LABEL(label_charada), listaCharadas[index].pergunta);
    acertos = 0;
    tentativasRestantes = MAX_TENTATIVAS;
    letrasUsadasCount = 0;
}

gboolean desenhar_forca(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_line_width(cr, 3);
    cairo_set_source_rgb(cr, 0, 0, 0);

    cairo_move_to(cr, 20, 200);
    cairo_line_to(cr, 120, 200);
    cairo_move_to(cr, 70, 200);
    cairo_line_to(cr, 70, 50);
    cairo_line_to(cr, 150, 50);
    cairo_line_to(cr, 150, 70);
    cairo_stroke(cr);

    int erros = MAX_TENTATIVAS - tentativasRestantes;

    if (erros >= 1) cairo_arc(cr, 150, 85, 15, 0, 2 * G_PI), cairo_stroke(cr);
    if (erros >= 2) cairo_move_to(cr, 150, 100), cairo_line_to(cr, 150, 140), cairo_stroke(cr);
    if (erros >= 3) cairo_move_to(cr, 150, 110), cairo_line_to(cr, 130, 125), cairo_stroke(cr);
    if (erros >= 4) cairo_move_to(cr, 150, 110), cairo_line_to(cr, 170, 125), cairo_stroke(cr);
    if (erros >= 5) cairo_move_to(cr, 150, 140), cairo_line_to(cr, 130, 165), cairo_stroke(cr);
    if (erros >= 6) cairo_move_to(cr, 150, 140), cairo_line_to(cr, 170, 165), cairo_stroke(cr);

    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Forca com Charadas");
    gtk_window_set_default_size(GTK_WINDOW(window), 550, 500);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label_charada = gtk_label_new("Carregando charada...");
    gtk_label_set_xalign(GTK_LABEL(label_charada), 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label_charada, FALSE, FALSE, 0);

    label_palavra = gtk_label_new("___");
    gtk_widget_set_name(label_palavra, "palavra");
    gtk_box_pack_start(GTK_BOX(vbox), label_palavra, FALSE, FALSE, 0);

    label_status = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), label_status, FALSE, FALSE, 0);

    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 400, 220);
    gtk_box_pack_start(GTK_BOX(vbox), canvas, FALSE, FALSE, 0);
    g_signal_connect(canvas, "draw", G_CALLBACK(desenhar_forca), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    int linha = 0, coluna = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        GtkWidget *botao = gtk_button_new_with_label((gchar[]) {c, '\0'});
        g_signal_connect(botao, "clicked", G_CALLBACK(letra_clicada), GINT_TO_POINTER(c));
        gtk_grid_attach(GTK_GRID(grid), botao, coluna, linha, 1, 1);
        coluna++;
        if (coluna == 8) {
            coluna = 0;
            linha++;
        }
    }

    iniciar_jogo();
    atualizar_palavra();
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
