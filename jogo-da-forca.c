#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TENTATIVAS 6
#define TAM_PALAVRA 30

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
    printf("Digite a palavra secreta: ");
    scanf("%s", palavra);
    limparBuffer();

    int tamanho = strlen(palavra);

    // Inicializa a exibição com underlines
    for (int i = 0; i < tamanho; i++) {
        exibicao[i] = '_';
    }
    exibicao[tamanho] = '\0';

    printf("\nA palavra tem %d letras.\n", tamanho);

    while (tentativasRestantes > 0 && acertos < tamanho) {
        printf("\nPalavra: ");
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
            printf("Por favor, digite uma letra válida.\n");
            continue;
        }

        if (letraJaUsada(chute, letrasUsadas, letrasUsadasCount)) {
            printf("Você já tentou essa letra. Tente outra.\n");
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
            printf("Boa! Letra correta.\n");
        }
    }

    if (acertos == tamanho) {
        printf("\nParabéns! Você acertou a palavra: %s\n", palavra);
    } else {
        printf("\nVocê perdeu! A palavra era: %s\n", palavra);
    }

    return 0;
}
