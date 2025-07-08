# 🧠 Jogo da Forca com Charadas (GTK)

Um jogo da forca interativo com charadas inteligentes, desenvolvido em C com interface gráfica usando GTK+ 3.

---

## 🎮 Funcionalidades

- 🧩 **Charadas aleatórias**: Você deve adivinhar a resposta da charada como em um jogo da forca.
- 🧍 **Boneco desenhado**: A forca aparece gradualmente a cada erro.
- 🔠 **Botões de letras**: Interface amigável com cliques para jogar, sem usar teclado.
- 🧼 **Tela limpa e responsiva**: Organizada com charada, palavra oculta e status.
- 🚫 **Evita letras repetidas**: Ignora cliques repetidos (e pode ser melhorado com botão desabilitado).
- 🧑‍💻 **Código limpo e comentado**: Ideal para aprendizado de C com GTK.

---

## 📷 Captura de Tela (opcional)

> (adicione aqui um print do jogo rodando com charada e boneco da forca)

---

## ⚙️ Como compilar

Certifique-se de ter o GTK 3 instalado. Em sistemas baseados em Ubuntu:

```bash
sudo apt install libgtk-3-dev

gcc jogo-da-forca.c -o forca_gtk `pkg-config --cflags --libs gtk+-3.0`

./forca_gtk

teste
