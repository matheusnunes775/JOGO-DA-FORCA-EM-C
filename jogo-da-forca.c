#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TENTATIVAS 6
#define TAM_PALAVRA 100

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

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

int main() {
    char palavra[TAM_PALAVRA], exibicao[TAM_PALAVRA];
    char letrasUsadas[26];
    int tentativasRestantes = MAX_TENTATIVAS;
    int acertos = 0, letrasUsadasCount = 0;
    char chute;

    printf("==== JOGO DA FORCA ====\n");
    printf("Digite a palavra secreta (sem acento): ");

    // Captura a palavra com espaços
    fgets(palavra, TAM_PALAVRA, stdin);
    palavra[strcspn(palavra, "\n")] = '\0'; // Remove o \n

    system(CLEAR); // Limpa a tela

    int tamanho = strlen(palavra);

    // Inicializa o array de exibição
    for (int i = 0; i < tamanho; i++) {
        if (palavra[i] == ' ')
            exibicao[i] = ' ';
        else
            exibicao[i] = '_';
    }
    exibicao[tamanho] = '\0';

    printf("A palavra tem %d letras.\n", tamanho);

    while (tentativasRestantes > 0 && acertos < tamanho) {
        printf("\n");
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
            printf("Digite apenas letras.\n");
            continue;
        }

        if (letraJaUsada(chute, letrasUsadas, letrasUsadasCount)) {
            printf("Você já tentou essa letra.\n");
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

    if (acertos == tamanho) {
        printf("\nParabéns! Você acertou a palavra: %s\n", palavra);
    } else {
        printf("\nVocê perdeu! A palavra era: %s\n", palavra);
    }

    return 0;
}
