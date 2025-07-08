# 🕹️ Jogo da Forca em C

Jogo da forca simples feito em linguagem C, jogado via terminal. O jogador 1 insere uma palavra secreta, e o jogador 2 tenta adivinhar letra por letra. O jogo exibe um boneco da forca conforme os erros acontecem.

---

## 🚀 Melhorias implementadas

- Oculta a palavra secreta (limpa a tela antes do jogo começar)
- Suporte a palavras com espaços (ex: "linguagem c")
- Desenho do boneco da forca conforme os erros
- Visual limpo no terminal
- Aceita apenas letras e evita repetições

---

## 💻 Como compilar e executar

### Compilar:

```bash
gcc jogo-da-forca.c -o jogo-da-forca
./jogo-da-forca


==== JOGO DA FORCA ====
Digite a palavra secreta (sem acento): programação

(Palavra oculta com tela limpa)

Palavra: _ _ _ _ _ _ _ _ _ _
Tentativas restantes: 6
Digite uma letra: a

Boa! Letra correta.

Palavra: _ _ _ _ _ _ A _ _ _
Tentativas restantes: 6
...

