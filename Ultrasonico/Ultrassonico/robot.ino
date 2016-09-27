#include <AFMotor.h>
#include <Ultrasonic.h>
 

#define HC_SR04_TRIGGER A2
#define HC_SR04_ECHO A1

int distancia_cm = 0; 

Ultrasonic ultrasonic(HC_SR04_TRIGGER, HC_SR04_TRIGGER);

AF_DCMotor motor1(1); //Seleciona o motor 1
AF_DCMotor motor2(4); //Seleciona o motor 2
AF_DCMotor motor3(2); //Seleciona o motor 3
AF_DCMotor motor4(3); //Seleciona o motor 4

void setup()
{
    Serial.begin(9600); // inicializa a comunicação serial para mostrar dados   
     
    pinMode(HC_SR04_TRIGGER, OUTPUT); // Define o trigger do sensor para enviar o sinal  
    pinMode(HC_SR04_ECHO, INPUT);   // Define o Echo do sensor para receber o sinal 
     
    motor1.setSpeed(500); //Gira o motor sentido horario
    motor2.setSpeed(500); //Gira o motor sentido horario
    motor3.setSpeed(500); //Gira o motor sentido horario
    motor4.setSpeed(500); //Gira o motor sentido horario
}
 
void loop()
{
  andar();
}

int lerSonar()
{    
   digitalWrite(HC_SR04_TRIGGER, LOW);     // Desliga a emisão do som  
   delayMicroseconds(4);            // Aguarda 4 segundos  
   digitalWrite(HC_SR04_TRIGGER, HIGH);     // Liga a trasmisão de som  
   delayMicroseconds(20);            // Continua emitindo o som durante 20 segundos  
   digitalWrite(HC_SR04_TRIGGER, LOW);     // Desliga a emisão do som   
   delayMicroseconds(10);            // Aguarda 10 segundos para poder receber o som  
   long pulse_us = pulseIn(HC_SR04_ECHO, HIGH); // Liga o recebedor e calcula quandos pulsos ele recebeu  
   distancia_cm = pulse_us / 59;        // Calcula a distaâcia em CM  
   delay(300);  
   return distancia_cm;             // Retorna a distância  
}  

void andar()
{       
   int distancia = lerSonar(); // Ler o sensor de distância  
   Serial.print("Distancia: "); // Exibe no serial  
   Serial.println(distancia);   

   if (distancia > 25) {    
     rotacao_Frente();    
   }else{
     rotacao_Direita();     
     andar();    
  }
}
 void rotacao_Direita()    
  {    
   motor1.run(RELEASE); // Roda vai para frente  
   motor2.run(RELEASE); // Roda vai para frente
   motor3.run(RELEASE); // Roda vai para frente  
   motor4.run(RELEASE); // Roda vai para frente 
  }   
  void rotacao_Frente()    
  {    
   Serial.println(" Frente ");   
   motor1.run(FORWARD); // Roda vai para frente  
   motor2.run(FORWARD); // Roda vai para frente
   motor3.run(FORWARD); // Roda vai para frente  
   motor4.run(FORWARD); // Roda vai para frente     
  }   
  

