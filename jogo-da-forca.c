#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TENTATIVAS 6
#define TAM_PALAVRA 100

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// Lista de palavras para modo solo
const char* listaPalavras[] = {
    "computador", "programador", "terminal", "linguagem", "variavel",
    "ponteiro", "compilador", "estruturas", "matriz", "funcoes"
};
const int totalPalavras = sizeof(listaPalavras) / sizeof(listaPalavras[0]);

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void desenharForca(int erros) {
    printf("\n");
    printf("  _______\n");
    printf(" |       |\n");
    printf(" |       %c\n", (erros >= 1) ? 'O' : ' ');
    printf(" |      %c%c%c\n", (erros >= 3) ? '/' : ' ', (erros >= 2) ? '|' : ' ', (erros >= 4) ? '\\' : ' ');
    printf(" |      %c %c\n", (erros >= 5) ? '/' : ' ', (erros >= 6) ? '\\' : ' ');
    printf("_|_\n\n");
}

int letraJaUsada(char letra, char letrasUsadas[], int total) {
    for (int i = 0; i < total; i++) {
        if (letrasUsadas[i] == letra)
            return 1;
    }
    return 0;
}

void limparTela() {
    system(CLEAR);
}

void exibirMensagemFinal(int venceu, const char* palavra) {
    if (venceu) {
        printf("\n🎉 Parabéns! Você acertou a palavra: %s\n", palavra);
    } else {
        printf("\n💀 Você perdeu! A palavra era: %s\n", palavra);
    }
}

void escolherPalavra(char* palavra, int modoSolo) {
    if (modoSolo) {
        srand(time(NULL));
        int index = rand() % totalPalavras;
        strcpy(palavra, listaPalavras[index]);
    } else {
        printf("Digite a palavra secreta (sem acento): ");
        fgets(palavra, TAM_PALAVRA, stdin);
        palavra[strcspn(palavra, "\n")] = '\0';
        limparTela(); // esconde palavra do segundo jogador
    }
}

int main() {
    char palavra[TAM_PALAVRA], exibicao[TAM_PALAVRA];
    char letrasUsadas[26];
    int tentativasRestantes = MAX_TENTATIVAS;
    int acertos = 0, letrasUsadasCount = 0;
    char chute;
    int modoSolo;

    printf("==== JOGO DA FORCA ====\n");
    printf("1 - Jogar com palavra secreta\n");
    printf("2 - Jogar com palavra aleatória (modo solo)\n");
    printf("Escolha o modo (1 ou 2): ");
    scanf("%d", &modoSolo);
    limparBuffer();

    escolherPalavra(palavra, modoSolo == 2);
    int tamanho = strlen(palavra);

    for (int i = 0; i < tamanho; i++) {
        exibicao[i] = (palavra[i] == ' ') ? ' ' : '_';
    }
    exibicao[tamanho] = '\0';

    while (tentativasRestantes > 0 && acertos < tamanho) {
        desenharForca(MAX_TENTATIVAS - tentativasRestantes);

        printf("Palavra: ");
        for (int i = 0; i < tamanho; i++) {
            printf("%c ", exibicao[i]);
        }

        printf("\nTentativas restantes: %d", tentativasRestantes);
        printf("\nLetras já usadas: ");
        for (int i = 0; i < letrasUsadasCount; i++) {
            printf("%c ", letrasUsadas[i]);
        }

        printf("\nDigite uma letra: ");
        scanf(" %c", &chute);
        limparBuffer();
        chute = tolower(chute);

        if (!isalpha(chute)) {
            printf("Digite apenas letras válidas.\n");
            continue;
        }

        if (letraJaUsada(chute, letrasUsadas, letrasUsadasCount)) {
            printf("Letra repetida! Tente outra.\n");
            continue;
        }

        letrasUsadas[letrasUsadasCount++] = chute;

        int encontrou = 0;
        for (int i = 0; i < tamanho; i++) {
            if (tolower(palavra[i]) == chute && exibicao[i] == '_') {
                exibicao[i] = palavra[i];
                acertos++;
                encontrou = 1;
            }
        }

        if (!encontrou) {
            printf("Letra incorreta!\n");
            tentativasRestantes--;
        } else {
            printf("Letra correta!\n");
        }
    }

    desenharForca(MAX_TENTATIVAS - tentativasRestantes);
    exibirMensagemFinal(acertos == tamanho, palavra);

    return 0;
}
