
#include<Servo.h>;
#include<LiquidCrystal.h>;
//#include<SoftwareSerial.h>; //Biblioteca Serial para o BT

// @jucabnu

// pinos do ultrassonic
int trigPin = 6;
int echoPin = 7;

// criando lcd e definindo pinagem
LiquidCrystal lcd(12,11,5,4,3,2);

// criando BT e definiindo piangem (TX, RX)
// SoftwareSerial btCom(6,7); 

// criando servo motores
Servo servoCabeca;
Servo servoDireita;
Servo servoEsquerda;

// distancias
long distDireita;
long distEsquerda;
long distFrente;

// tempo deslocamento
int tempoParaAndar = 1500;
int tempoParaVirar = 1000;

// parametros para configurações
// boolean controleManual;
// boolean imprimeBtCom;
boolean simulador;
boolean imprimeSerial;
boolean imprimeLcd;

void setup(){

// CONFIGURACOES INICIAIS
//controleManual = true;
//imprimeBtCom = false;
  simulador = false; //-----------------------------
  imprimeSerial = true;  
  imprimeLcd = true;

// iniciando o servo da cabeça
  servoCabeca.attach(9);
  servoCabeca.write(90);

// iniciando o lcd
  lcd.begin(16,2);

  // inicia comunicação serial bluetooth RS232
  /*
  btCom.begin(9600); 
   if(btCom.available()){
   imprime("conexao bt", "estabelecida", 0);  
   }
   */
   
  // inicia comunicação serial com PC
  Serial.begin(9600);

  imprime("ROBO AUTONOMO","by Jucabnu ",0);
  delay(2000);

  // inicia os pinos do sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // inicia as distâncias com uma leitura inicial
  distDireita = leDistancia();
  distEsquerda = leDistancia();
  distFrente = leDistancia();
}

// *************************************************
// ************* CONTROLE DE MOVIMENTO *************
// *************************************************

// dois motores rodando para frente
void acelerar(){
  imprime("indo para frente"," ",0);
  if(!simulador){
    servoDireita.attach(10);
    servoEsquerda.attach(8);
    servoDireita.write(0);
    servoEsquerda.write(180);
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
    servoDireita.write(180);
    servoEsquerda.write(0);
  }
  delay(tempoParaAndar);
  parado();
}

// motor da esquerda gira para frente
// enquanto motor da direita gira para tras
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
// enquanto motor da esquerda gira para tras
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
  imprime(".","x PARADO x",0);
  if(!simulador){
    servoDireita.detach();
    servoEsquerda.detach();      
  }
  delay(100);
}

// *************************************************
// ************** CONTROLE AUTOMACAO ***************
// *************************************************

void mapeamento(){

  imprime("iniciando teste"," ",0);

  // obter distância do centro (90 graus)
  // remover se estiver com problema
  // servoCabeca.write(90);  
  delay(500);
  distFrente = leDistancia();
  delay(100);

  // obter distância à direita (0 graus)
  servoCabeca.write(5);
  delay(1000);
  distDireita = leDistancia();
  delay(100);

  servoCabeca.write(90);
  delay(1000);

  // obter distância à esquerda (180 graus)
  servoCabeca.write(175);
  delay(1000);
  distEsquerda = leDistancia();
  delay(100);

  servoCabeca.write(90);
  delay(1000);

  imprime(" Distancias"," obtidas",0);
  delay(500);
  imprime("----------------"," ",0);

}

// le distancia do sensor ultrassonico
int leDistancia(){  
  int dis = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dis = (pulseIn(echoPin, HIGH)/29/2);
  delay(100);
  return dis;

  // pulseInt lê o tempo entre a chamada e o pino entrar em high  
  // calculo é baseado em s = v.t, tempo vem dobrado  
}

// verifica qual distancia é melhor e direciona o carro
void direcionamento(){

  if(distDireita > distEsquerda && distDireita > distFrente){
    imprime("Melhor opcao: ","Direita ",distDireita);
    virarParaDireita();
  }else if(distEsquerda > distDireita && distEsquerda > distFrente){
    imprime("Melhor opcao: ","Esquerda ",distEsquerda);
    virarParaEsquerda();
  }else if(distFrente > distEsquerda && distFrente > distDireita){
    imprime("Melhor opcao: ","Frente ",distFrente);
    acelerar();
  }else{
    imprime("Melhor opcao: ","?????? ",0);
    parado();
  }
}

// metodo para multi impressao (isso devera virar uma classe)
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
/*
if(imprimeBtCom){
    btCom.print(texto1);
    btCom.print(texto2);
    btCom.print(valor);
  }
*/  
}

// dipara alarme no pino 13
/*
void avisoSonoro(){
  digitalWrite(alarme, HIGH);
  tone(8,440);
  delay(50);
  noTone(8);
  delay(50);
}
*/

// metodo principal
void loop(){

  acelerar();
  // cai nesse while e fica aqui ate poweroff
  while(true){
    while(leDistancia() > 20){
      imprime("_confere", " ", 0);
      acelerar();
    }
    //avisoSonoro();
    mapeamento();
    delay(500);
    direcionamento();
  }
}
