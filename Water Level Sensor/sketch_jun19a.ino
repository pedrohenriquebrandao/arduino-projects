const int analogInPin = A0;
const int analogInPin2 = A1;

bool tanque1Booleano;
bool tanque2Booleano;

int tanque1 = 0;
int tanque2 = 0;

void setup() {
  pinMode(2,OUTPUT); // Cheio 
  pinMode(3,OUTPUT); // Vazio
  Serial.begin(9600);
}

void loop() {
  tanque1 = analogRead(analogInPin);
  tanque2 = analogRead(analogInPin2);
  
  Serial.print("Tanque 1 = ");
  Serial.print(tanque1);
  Serial.print(" --- ");
  Serial.print("Tanque 2 = ");
  Serial.print(tanque2);
  Serial.print(" --> ");
  Serial.print(tanque1Booleano);
  Serial.print(" - ");
  Serial.print(tanque2Booleano);
  Serial.print("\n");
  delay(200);

  // Definição dos valores booleanos a partir dos dados dos sensores 

  if(tanque1<=600 && tanque2<=600){  // Vazio
    tanque1Booleano = false;
    tanque2Booleano = false;
  }

  else if(tanque1>=600 && tanque2>=600) { // Cheio
    tanque1Booleano = true;
    tanque2Booleano = true;
  }

  else if(tanque1>=600 && tanque2<=600) { 
    tanque1Booleano = true;
    tanque2Booleano = false;
  }

  else if(tanque1<=600 && tanque2>=600) {
    tanque1Booleano = false;
    tanque2Booleano = true;
  }

  // Portas lógicas

  if(!tanque1Booleano & !tanque2Booleano){ // Vazio
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    delay(100);
  }

  else if(tanque1Booleano & tanque2Booleano){ // Cheio
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
    delay(100);
  }

  else {
    digitalWrite(3,LOW);
    digitalWrite(2,LOW);
    delay(100);
  }
}