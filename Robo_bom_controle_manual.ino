
#include<Servo.h>;
#include<SoftwareSerial.h>;
#include<LiquidCrystal.h>;


// @jucabnu

LiquidCrystal lcd(12,11,5,4,3,2);

// TX, RX interface modulo bluetooth
SoftwareSerial btCom(6,7); 

// servo motores
Servo servoDireita;
Servo servoEsquerda;

// tempo deslocamento
int tempoParaAndar = 5000; // 10 segundos
int tempoParaVirar = 2000; // 2 segundos

// parametros para configurações
boolean simulador;
boolean imprimeSerial;
boolean imprimeBtCom;
boolean imprimeLcd;

void setup(){

  // CONFIGURACOES INICIAIS
  simulador = true;
  imprimeSerial = true;
  imprimeBtCom = false;
  imprimeLcd = true;

  lcd.begin(16,2);

  // inicia comunicação serial bluetooth RS232
  btCom.begin(9600); 
  if(btCom.available()){
    imprime("conexao bt", "estabelecida", 0);  
  }

  // inicia comunicação serial com PC
  Serial.begin(9600);

  imprime("ROBO AUTONOMO","by Jucabnu ",0);
  delay(2000);
}

// *********** controle de direção ***********

// dois motores rodando para frente
void acelerar(){
  imprime("indo para frente"," ",0);
  if(!simulador){
    servoDireita.attach(10);
    servoEsquerda.attach(8);
    servoDireita.write(180);
    servoEsquerda.write(0);
  }
  delay(tempoParaAndar);
  parado();
}

// dois motores rodando para tras
void andarParaTras(){
  imprime("indo para tras"," ",0);
  if(!simulador){
    servoDireita.attach(10);
    servoEsquerda.attach(8);
    servoDireita.write(0);
    servoEsquerda.write(180);
  }
  delay(tempoParaAndar);
  parado();
}

// motor da esquerda gira para frente
// enquanto motor da direita girar para tras
void virarParaDireita(){
  imprime("virando a direita"," ",0);
  if(!simulador){
    servoDireita.attach(10);
    servoEsquerda.attach(8);
    servoDireita.write(180);
    servoEsquerda.write(180);
  }
  delay(tempoParaVirar);
  parado();
}

// motor da direita gira para frente
// enquanto motor da esquerda girar para tras
void virarParaEsquerda(){
  imprime("virando a esquerda"," ",0);
  if(!simulador){
    servoDireita.attach(10);
    servoEsquerda.attach(8);
    servoDireita.write(0);
    servoEsquerda.write(0); 
  }
  delay(tempoParaVirar);
  parado();
}

// destaca os dois motores para que fiquem parados
void parado(){
  imprime(".","XX parado XX",0);
  if(!simulador){
    servoDireita.detach();
    servoEsquerda.detach();  
  }
}

void imprime(String texto1, String texto2, int valor){

  if(imprimeSerial){
    Serial.println(texto1);
    if(texto2 != "" && texto2 != " "){
      Serial.print(texto2);
      Serial.print(" ");
    }
    if(valor != 0){
      Serial.println(valor);
    }
  }
  if(imprimeBtCom){
    btCom.print(texto1);
    btCom.print(texto2);
    btCom.print(valor);
  }
  if(imprimeLcd){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(texto1);
    if(texto2 != "" && texto2 != " "){
      lcd.setCursor(0,1);
      lcd.print(texto2);
    }
    if(valor != 0){
      lcd.print("                ");
      lcd.setCursor(0,1);
      delay(1000);
      lcd.print("                ");
      lcd.print(valor); 
      delay(100);
    }
  }  
}

void loop(){

  String comando;
  comando = btCom.readString().substring(0, btCom.readString().length()-2);

  if(comando == "stop"){
    parado();
  }
  if(comando == "go"){
    acelerar();
  }
  if(comando == "right"){
    virarParaDireita();
  }
  if(comando == "left"){
    virarParaEsquerda();
  }
  delay(100);
}

