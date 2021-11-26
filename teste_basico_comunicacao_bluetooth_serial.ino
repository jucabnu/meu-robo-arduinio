#include<SoftwareSerial.h>;
#include<Servo.h>;


// @jucabnu

Servo servo;
SoftwareSerial btCom(10,11);

void setup(){
  
  Serial.begin(9600);
  btCom.begin(9600);
  btCom.println("teste...");
  servo.attach(9);
  servo.write(90);

}

void loop(){



  String comando;
  String comandoPronto;
//  btCom.print("Aguardando comando:");
  
  if(btCom.available()){
    comando = btCom.readString();
    comandoPronto = comando.substring(0, comando.length()-2);
    Serial.println(comandoPronto);
    Serial.println(comandoPronto.length());
//    servo.write(comando);
    

    
    if(comandoPronto == "left"){
      servo.write(5);
    }
    if(comandoPronto == "center"){
      servo.write(90);
    }
    if(comandoPronto == "right"){
      servo.write(175);
    }

    delay(500);
  
  
  }
  delay(100);

  if(comando == "left"){
    Serial.println("confere");
    delay(2000);
  }

}
