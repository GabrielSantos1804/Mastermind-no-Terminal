  # 🎯 Mastermind — Jogo de Adivinhação de Cores

![Linguagem](https://img.shields.io/badge/linguagem-C-blue.svg)
![Status](https://img.shields.io/badge/status-concluído-brightgreen.svg)
![Licença](https://img.shields.io/badge/licença-MIT-lightgrey.svg)

Implementação em **C** do clássico jogo de tabuleiro **Mastermind**, em que o jogador tenta descobrir uma combinação secreta de cores dentro de um número limitado de tentativas.

O projeto foi desenvolvido como exercício prático de lógica de programação, manipulação de arrays e estruturas de controle, com foco em um algoritmo de comparação eficiente e testes iterativos até a validação de todos os casos de uso.

---

## 🕹️ Como funciona

1. O programa gera (ou define) uma combinação secreta de cores.
2. O jogador insere tentativas, tentando acertar a combinação.
3. Para cada tentativa, o programa retorna um feedback indicando:
   - **Cores certas na posição certa**
   - **Cores certas na posição errada**
4. O jogo continua até o jogador acertar a combinação ou esgotar o número de tentativas.

---

## ⚙️ Destaques técnicos

- **Algoritmo de comparação em duas passagens**: a primeira passagem identifica acertos exatos (cor e posição corretas); a segunda passagem verifica cores corretas em posições erradas, evitando contagens duplicadas.
- **Controle de posições já utilizadas** por meio de um array booleano, garantindo que cada posição da combinação seja avaliada uma única vez por tentativa.
- **Depuração e testes iterativos**, cobrindo casos extremos como cores repetidas e tentativas parcialmente corretas.

---

## 🛠️ Tecnologias

- **Linguagem:** C
- **Compilador:** GCC
- **Ambiente de desenvolvimento:** Terminal Linux / WSL, VS Code

---

## 🚀 Como executar

```bash
# Clone o repositório
git clone https://github.com/GabrielSantos1804/mastermind.git
cd mastermind

# Compile o projeto
gcc mastermind.c -o mastermind

# Execute
./mastermind
```

---

## 📚 O que eu aprendi

Este projeto foi uma oportunidade de aplicar, na prática, conceitos fundamentais de lógica de programação e estruturas de dados, além de reforçar a importância de testes cuidadosos para validar diferentes cenários de jogo — especialmente casos com cores repetidas na combinação secreta.

---

## 👤 Autor

**Gabriel da Silva Santos**
Estudante de Ciência da Computação — UFOP

- GitHub: [@GabrielSantos1804](https://github.com/GabrielSantos1804)
- LinkedIn: [gabriel-santos6059803b5](https://linkedin.com/in/gabriel-santos6059803b5)

---
