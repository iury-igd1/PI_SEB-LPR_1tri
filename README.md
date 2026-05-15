# Sistema de Controle de Aulas com STM32

Projeto Integrado desenvolvido para as disciplinas de Sistemas Embarcados e Linguagem de Programação.

O sistema simula o controle de aulas em laboratório utilizando um microcontrolador STM32F103C8, um display ST7735 e botões físicos para interação do professor.

---

## ⚙️ Funcionalidades

- Autenticação por senha numérica;
- Bloqueio de segurança após múltiplos erros;
- Configuração da capacidade da turma;
- Controle de entrada e saída de alunos;
- Limite de alunos fora da sala;
- Barra visual de ocupação da sala;
- Relatório final da aula;
- Cálculo do tempo total da aula.

---

## 🔧 Hardware Utilizado

- STM32F103C8 (Blue Pill);
- Display ST7735 SPI;
- Push Buttons.

---

## 📋 Funcionamento

O professor realiza a autenticação utilizando uma senha gerada aleatoriamente pelo sistema. Após o login, é possível configurar a quantidade máxima de alunos da turma e controlar toda a dinâmica da aula utilizando os botões.

Durante a execução, o sistema:

- Registra entradas de alunos;
- Controla alunos fora da sala;
- Impede ações inválidas;
- Exibe informações em tempo real no display;
- Gera um relatório ao final da aula.

A leitura de matrícula foi simulada utilizando uma variável em modo Debug do STM32CubeIDE.

---

## 🧠 Decisões de Implementação

- Bloqueio progressivo após múltiplas tentativas inválidas de autenticação;
- Simulação de leitura de matrícula via modo Debug;
- Atualização do display baseada em mudança de estados;
- Debounce de software para evitar múltiplos acionamentos dos botões;
- Organização do sistema utilizando máquina de estados.

---

## 🎥 Demonstração em Vídeo

▶ [Assistir apresentação no YouTube](https://youtu.be/M9W3hgQNasE)

---

## 💻 Tecnologias Utilizadas

- Linguagem C;
- STM32CubeIDE;
- STM32 HAL;
- GitHub.

---

## ▶️ Como Executar

1. Clone o repositório;
2. Abra o projeto no STM32CubeIDE;
3. Compile o código;
4. Grave na placa STM32;
5. Execute em modo Debug do STM32CubeIDE.

---

## 👥 Integrantes

- Iury Gonçalves de Souza
- Caroline Maciel de Ávila Rosa

---

Turma 34 DS — 2026  
Projeto Integrado — 1º trimestre
