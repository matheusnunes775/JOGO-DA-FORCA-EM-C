// Jogo da Forca com GTK — versão completa com melhorias
// Desenvolvido por Matheus e ChatGPT

#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#define MAX_TENTATIVAS 6
#define TAM_PALAVRA 100
#define QTD_LETRAS 26
#define QTD_CHARADAS 10

typedef struct {
    const char* pergunta;
    const char* resposta;
} Charada;

// Lista de charadas + palavras comuns (misturadas)
Charada charadas[] = {
    {"O que é, o que é: cai em pé e corre deitado?", "chuva"},
    {"Tenho dentes, mas não mordo. O que sou?", "pente"},
    {"Qual é o animal que anda com um pé?", "sapo"},
    {"O que é feito para andar e não anda?", "rua"},
    {"O que tem cabeça, tem dente, tem barba, mas não é gente?", "alho"},
    {"O que passa diante do sol e não faz sombra?", "vento"},
    {"Quanto mais se tira, maior fica. O que é?", "buraco"},
    {"Anda deitado e dorme em pé?", "sapato"},
    {"Tem asa, mas não voa, bico, mas não bica?", "xicara"},
    {"Comida" , "macarrao"} // palavra comum sem charada
};

char palavra[TAM_PALAVRA];
char exibicao[TAM_PALAVRA];
char letrasUsadas[QTD_LETRAS];
int tentativasRestantes = MAX_TENTATIVAS;
int acertos = 0;
int letrasUsadasCount = 0;
int vitorias = 0;

GtkWidget *label_palavra, *label_status, *label_charada, *canvas, *grid_letras, *label_vitorias;
GtkWidget *botoes_letras[QTD_LETRAS];
int jogo_ativo = 1;

void atualizar_palavra() {
    gtk_label_set_text(GTK_LABEL(label_palavra), exibicao);
    char status[100];
    sprintf(status, "Tentativas restantes: %d", tentativasRestantes);
    gtk_label_set_text(GTK_LABEL(label_status), status);
    gtk_widget_queue_draw(canvas);
}

void fim_do_jogo(gboolean venceu) {
    jogo_ativo = 0;
    GtkWidget *dialog;
    if (venceu) {
        vitorias++;
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "\uD83C\uDF89 Você acertou!");
    } else {
        vitorias = 0;
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "\uD83D\uDC80 Você perdeu!\nResposta: %s", palavra);
    }
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    char placar[50];
    sprintf(placar, "Vitórias seguidas: %d", vitorias);
    gtk_label_set_text(GTK_LABEL(label_vitorias), placar);
}

void reiniciar_jogo();

void letra_clicada(GtkWidget *widget, gpointer data) {
    if (!jogo_ativo) return;

    char letra = GPOINTER_TO_INT(data);
    letra = tolower(letra);

    if (strchr(letrasUsadas, letra)) return;
    letrasUsadas[letrasUsadasCount++] = letra;
    gtk_widget_set_sensitive(widget, FALSE);

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
    int index = rand() % QTD_CHARADAS;
    strcpy(palavra, charadas[index].resposta);

    int tam = strlen(palavra);
    for (int i = 0; i < tam; i++)
        exibicao[i] = (palavra[i] == ' ') ? ' ' : '_';
    exibicao[tam] = '\0';

    acertos = 0;
    tentativasRestantes = MAX_TENTATIVAS;
    letrasUsadasCount = 0;
    jogo_ativo = 1;

    gtk_label_set_text(GTK_LABEL(label_charada), charadas[index].pergunta);

    for (int i = 0; i < QTD_LETRAS; i++)
        gtk_widget_set_sensitive(botoes_letras[i], TRUE);

    atualizar_palavra();
}

void reiniciar_jogo() {
    iniciar_jogo();
}

gboolean desenhar_forca(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_line_width(cr, 3);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 20, 200); cairo_line_to(cr, 120, 200);
    cairo_move_to(cr, 70, 200); cairo_line_to(cr, 70, 50);
    cairo_line_to(cr, 150, 50); cairo_line_to(cr, 150, 70);
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
    setlocale(LC_ALL, "");
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Jogo da Forca com Charadas");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 550);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label_charada = gtk_label_new("Carregando charada...");
    label_palavra = gtk_label_new("");
    label_status = gtk_label_new("");
    label_vitorias = gtk_label_new("Vitórias seguidas: 0");

    gtk_box_pack_start(GTK_BOX(vbox), label_charada, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_palavra, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_status, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_vitorias, FALSE, FALSE, 0);

    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 400, 200);
    gtk_box_pack_start(GTK_BOX(vbox), canvas, FALSE, FALSE, 0);
    g_signal_connect(canvas, "draw", G_CALLBACK(desenhar_forca), NULL);

    grid_letras = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid_letras), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid_letras), 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid_letras, FALSE, FALSE, 0);

    for (int i = 0; i < QTD_LETRAS; i++) {
        char letra = 'A' + i;
        botoes_letras[i] = gtk_button_new_with_label((gchar[]) {letra, '\0'});
        g_signal_connect(botoes_letras[i], "clicked", G_CALLBACK(letra_clicada), GINT_TO_POINTER(letra));
        gtk_grid_attach(GTK_GRID(grid_letras), botoes_letras[i], i % 8, i / 8, 1, 1);
    }

    GtkWidget *botao_reiniciar = gtk_button_new_with_label("Jogar Novamente");
    g_signal_connect(botao_reiniciar, "clicked", G_CALLBACK(reiniciar_jogo), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), botao_reiniciar, FALSE, FALSE, 0);

    iniciar_jogo();
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
