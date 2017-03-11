// Programa exemplo para a placa Mestre_Mandou
// com as conexões ordenadas conforme conector
//
// 2017/01/24
// Mau Jabur (maumaker [at] pandoralab.com.br)
//
// Arduino IDE 1.6.13
// Uno R3
//
// Domínio público. Uso livre com atribuição

const int T = 600; // base de tempo (menor -> mais rápido)
int t_base;

// ligue os pinos da placa no Arduino, pinos 2 a 10.
// conforme abaixo
// conecte também VCC e GND
const byte BZ   = 2;

const byte LED1 = 9;
const byte S1   = 10;
const byte S2   = 8;
const byte LED2 = 7;
const byte S3   = 6;
const byte LED3 = 5;
const byte S4   = 4;
const byte LED4 = 3;

// como estamos usando o "pullup" interno:
const int PRESS = LOW;
// usaremos esta constante para facilitar a leitura do código

// juntamos todos os leds e todos os botões em tabelas chamadas 
// vetores ou "arrays". Isto facilita a programação
const byte LED[] = {LED1, LED2, LED3, LED4};
const byte BOTAO[] = {S1, S2, S3, S4};
//também juntamos as frequencias dos sons em uma tabela (vetor)
const unsigned int TOM[] = {523, 587, 659, 698};
// do, re, mi, fa

// constante, tabela e variável para armazenar as notas sorteadas
const int max_seq = 1337; // depois de apenas 1337 notas você vence o sistema!
byte seq[max_seq];
int  qt_notas;

void setup() {

  t_base = 100; // para fazer o auto-teste bem rapidinho
  boolean passou = true; // muda para false se alguma tecla estiver pressionada

  pinMode(BZ, OUTPUT); // o pino onde está ligado o buzzer é uma saída
  
  // aqui geramos na variável 'i' os números de 0 a 3
  // (as tabelas começam em 0)
  for (int i = 0; i < 4; i++) {
    pinMode(LED[i], OUTPUT); // configura o pino ligado ao led como saida
    pinMode(BOTAO[i], INPUT_PULLUP); // configura o pino ligado ao botão
    // como entrada e liga um resistor interno para garantir um nivel
    // positivo (HIGH) quando o botão não estiver apertado
   
    // faz um bip, liga o led e verifica o botão
    passou = passou & teste(i);
    // se o botão estiver pressionado, passou vira "false" e o led
    // fica aceso para indicar (que estava pressionado)
  }

  if (passou == false ) for (;;); // fica parado pra sempre se alguma tecla foi pressionada

  // esta parte é menos importante
  // só faz um "clek clek" depois do teste
  // (eu achei que parecia a música do familia Adams)
  // adams family hack
  delay(17 * t_base / 5);
  digitalWrite(BZ, HIGH);
  delay(20);
  digitalWrite(BZ, LOW);
  delay(18 * t_base / 5);
  digitalWrite(BZ, HIGH);
  delay(20);
  digitalWrite(BZ, LOW);

  // esta função "reseta" as variáveis de base de tempo
  // e tabela de notas sorteadas
  // e fica aguardando uma tecla ser pressionada
  reinicio();
}

void loop() {
  // sorteia um novo passo
  sorteio();
  // reproduz a sequencia incluindo o passo sorteado
  desafio();
  // verifica se o jogador conseguiu reproduzir a sequencia
  if (tentativa() == true) {
    // caso positivo, comemora
    acerto();
  }
  else {
    // se não conseguiu digitar a sequência
    // toca novamente a sequencia sorteada
    desafio();
    // e reinicia o jogo
    reinicio();
  }
}

void reinicio() {
  // a duração das notas volta ao normal
  t_base = T;
  // nenhuma nota sorteada
  qt_notas = 0;
  // aguarda alguma tecla ser pressionada
  espera();
}

void espera() {
  // liga todos os leds, demorando um pouco entre cada um
  for (byte i = 0; i < 4; i++) {
    digitalWrite(LED[i], HIGH);
    delay(50);
  }

  // começa no indice 0
  byte i = 0;
  // enquanto o botão 'i' não for apertado
  while (digitalRead(BOTAO[i]) != PRESS) {
    // pula para o próxim indice
    // não sendo maior ou igual a 4
    // % 4 significa "resto da divisão por 4"
    i = (i+1) % 4;
  }
  
  // depois que algum botão foi apertado
  // desliga todos os leds, demorando um pouco entre cada um
  for (byte i = 0; i < 4; i++) {
    digitalWrite(LED[i], LOW);
    delay(50);
  }

  // para o sorteio não ficar viciado
  // atualiza baseado no tempo que o arduino está ligado
  // e também na leitura de um pino desconectado
  // (leitura de interferência elétrica)
  randomSeed(millis() + analogRead(A0));

  // espera um pouco para retornar ao programa principal
  delay(t_base);
}

void sorteio() {
  // sorteia um número maior ou igual a zero
  // e menor que 4
  // ou seja, entre zero e tres
  seq[qt_notas] = random(4);
  
  // agora que uma nota foi sorteada,
  // aumenta a contagem de notas
  qt_notas++;
  
  // deixa mais rápido com mais notas
  t_base = T / 2 + T / (2 * qt_notas);
  
  // se a sequência chegou ao limite
  if (qt_notas > max_seq) {
    // toca um som diferente para comemorar
    epic();
    // faz replay na sequencia
    desafio();
    // e reinicia o jogo!
    reinicio();
  }
}

void desafio() {
  // começando no indice zero
  // e indo até a quantidade de notas sorteadas
  for (int i = 0; i < qt_notas; i++) {
    // toca a nota da vez
    nota(seq[i]);
  }
}

boolean tentativa() {
  // começando no indice zero
  // e indo até a quantidade de notas sorteadas
  for (int i = 0; i < qt_notas; i++) {
    if (le_botao() != seq[i]) {
      // se o botão lido não coincidir com a nota da vez
      // indica o erro
      erro(seq[i]);
      // e para de reproduzir, retornando "false"
      return false;
    }
    // se não houve erro, continua tocando a proxima nota
    // executando o proximo i do "for"
  }
  // se a sequencia chegou äo fim sem erro
  // retorna "true"
  return true;
}

void erro(byte indice) {
  // Espera um pouco
  delay(T / 5);
  // acende o led correto na sequencia
  digitalWrite(LED[indice], HIGH);
  // toca um som "feio"
  tone(BZ, 80);
  // espera 
  delay(T);
  // apaga o led
  digitalWrite(LED[indice], LOW);
  // desliga o som
  noTone(BZ);
  // espera um pouco antes de retornar
  delay(T);
}

void acerto() {
    // espera um pouco
  delay(T/4);
  // repete duas vezes o bloco abaixo
  for (int i = 0; i < 2; i++) {
    // toca um som
    tone(BZ, 440);
    // acende os leds 1 e 3 e apaga os outros
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    // espera um pouco
    delay(T / 4);

    // toca outro som
    tone(BZ, 880);
    // acende os leds 2 e 4 e apaga os outros
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    // espera um pouco
    delay(T / 4);
  }
  
  // desliga o som e os leds
  noTone(BZ);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  // espera um pouco antes de retornar
  delay(T);
}

void nota(byte indice) {
  // toca a nota (0 a 3) da tabela de notas
  tone (BZ, TOM[indice]);
  // acende o led (0 a 3) da tabela de leds
  digitalWrite(LED[indice], HIGH);
  // espera a duração da nota
  delay(t_base);
  // desliga o som e apaga o led
  noTone(BZ);
  digitalWrite(LED[indice], LOW);
  // espera um pouco
  delay(t_base / 5);
}

void epic() {
  // similar ao acerto
  // vamos colocar uma musica mais bacana?
  // precisa alterar aqui
  // dá pra animar as luzes também!
  for (int i = 0; i < 4; i++) {
    tone(BZ, 440);
    delay(T / 4);
    tone(BZ, 880);
    delay(T / 4);
  }
  noTone(BZ);
  delay(T);
}

byte le_botao() {
  // inicializamos com um numero de botão que não existe
  // para saber que nenhum foi detectado ainda
  byte lido = 255;
  
  // enquanto um botão não for detectado
  while (lido == 255) {
    // varre os botoes nos indices 0 a 3
    for (byte i = 0; i < 4; i++) {
      // se o botão foi pressionado
      if (digitalRead(BOTAO[i]) == PRESS) {
        // toca a nota associada ao botão e acende o led
        tone(BZ, TOM[i]);
        digitalWrite(LED[i], HIGH);
        
        do {
          // aguarda um pouco para ler de novo
          delay(50);
        } // enquanto o botão estiver pressionado
        while (digitalRead(BOTAO[i]) == PRESS);

        // desliga o som e o led
        noTone(BZ);
        digitalWrite(LED[i], LOW);
        // marca o botão lido
        lido = i;
      }
    }
  }
  
  // e retorna o resultado
  return lido;
}

boolean teste(byte indice) {
  // liga o led no indice
  digitalWrite (LED[indice], HIGH);
  // se o botão estiver apertado
  if (digitalRead(BOTAO[indice]) == PRESS) {
    // toca um som "feio"
    tone (BZ, TOM[indice] / 4, 180);
    // espera um tempo
    delay(t_base * 2);
    // desliga o som
    noTone(BZ);
    // e retorna indicando botão apertado
    return false;
  }
  else {
    // senão toca a nota associada ao botão por um tempo
    nota(indice);
  }
  // e retorna indicando que correu tudo normalmente
  return true;
}
