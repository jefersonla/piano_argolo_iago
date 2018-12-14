// Inclui as notas
#include "notes.h"

// Define tempo entre clique milissegundo
#define TEMPO_ENTRE_CLIQUE 500

// Define o tempo do blink
#define TEMPO_BLINK 500

// Define o pino para o emissor de som do tipo buzzer
#define PIN_BUZZER 5

// Define os pinos utilizados pelos botões azul e vermelho
#define PIN_BOTAO_VERMELHO 2
#define PIN_BOTAO_AZUL 3

// Define os pinos dos leds utilizados pelos botões
#define PIN_LED_BOTAO_AZUL 6
#define PIN_LED_BOTAO_VERMELHO 7

// Define os pinos utilizados pelas teclas do piano A, B, C e D
const unsigned int pinosPiano[] = {
  A2,
  A3,
  A4,
  A5
};

// Define um valor entre 0 e 1023 que representa o treshold para que um sinal seja válido
#define THRESHOLD_LEITURA_PIANO 850

// Notas utilizadas pelo piano
const unsigned int notasPiano[] = {
  NOTE_C7,
  NOTE_B6,
  NOTE_A6,
  NOTE_G6,
  NOTE_F6,
  NOTE_E6,
  NOTE_D6,
  NOTE_C6
};

// Pino sendo lido no momento
unsigned int pinoLeitura = 0;

// Define o estado das notas escolhidas
boolean notasEscolhidas = false;

// Tempo entre as tarefas
unsigned long long tempoAnteriorNotas = 0;
unsigned long long tempoBlink = 0;

// Define o estado entre o blink
boolean estadoBlink = false;

/**
 * Executa uma vez a rotina de configuração
 */
void setup() {
  
  // Inicializa os pinos
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BOTAO_VERMELHO, INPUT);
  pinMode(PIN_BOTAO_AZUL, INPUT);
  pinMode(PIN_LED_BOTAO_AZUL, OUTPUT);
  pinMode(PIN_LED_BOTAO_VERMELHO, OUTPUT);

  // Inicializa a comunicação serial
  Serial.begin(115200);

}

/**
 * Executa diversas vezes
 */
void loop() {

  // Pega o tempo atual
  const unsigned long long tempoAtual = millis();

  // Reseta o pino selecionado
  if (pinoLeitura > 3) {
    pinoLeitura = 0;
    //Serial.write('\n');
  }

  // Insere o separador caso não seja o primeiro valor
  if(pinoLeitura != 0) {
    //Serial.write(',');
  }

  // Lê um pino da entrada analógica
  const unsigned int leituraAnalogica = constrain(analogRead(pinosPiano[pinoLeitura]), 0, 1023);
  //Serial.print(leituraAnalogica);

  // Conjunto de notas
  const boolean pinoLidoNotasEscolhidas = !digitalRead(PIN_BOTAO_AZUL);

  // Troca as notas escolhidas
  if (pinoLidoNotasEscolhidas && (tempoAtual - tempoAnteriorNotas) > TEMPO_ENTRE_CLIQUE) {
    tempoAnteriorNotas = tempoAtual;
    notasEscolhidas = !notasEscolhidas;
    Serial.print("Notas Escolhidas");
    Serial.println(notasEscolhidas);
  }

  // Define o estado led
  digitalWrite(PIN_LED_BOTAO_AZUL, notasEscolhidas);

  // Executa o blink no intervalo proposto
  if ((tempoAtual - tempoBlink) > TEMPO_BLINK) {

    // Atualiza o tempo do blink
    tempoBlink = tempoAtual;

    // Atualiza o estado do blink
    estadoBlink = !estadoBlink;

    // Ativa o pino do blink
    digitalWrite(PIN_LED_BOTAO_VERMELHO, estadoBlink);

  }

  // Verifica se a leitura analogica é superior ao threshold definido
  if (leituraAnalogica <= THRESHOLD_LEITURA_PIANO) {

    // Emite a leitura
    tone(PIN_BUZZER, (notasEscolhidas ? notasPiano[pinoLeitura] : notasPiano[pinoLeitura + 4]), 20);
    
  }

  // Incrementa o contador do pino analogico
  pinoLeitura++;
  
}
