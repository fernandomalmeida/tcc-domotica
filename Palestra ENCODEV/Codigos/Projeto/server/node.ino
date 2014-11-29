#include <Package.h>

#define MAX_BUFFER SIZE_TOTAL_PACKAGE

unsigned char buffer[MAX_BUFFER] = "";
int tamBuffer = 0;

Package p;

#define LED1 A1
#define LED2 A2
#define BOTAO A0

#define BUZZER 5

int led1;
int led2;
int botao;
int buzzer;


void setup(){
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BOTAO, INPUT);
  pinMode(BUZZER, OUTPUT);

  led1 = LOW;
  led2 = LOW;
  buzzer = LOW;
  
}

void callbackSerial(unsigned char *buffer, int tamBuffer){
  ByteArrayToPackage(buffer, &p);

  if(p.isRequest){
    switch(p.info){
      case GET:
        if(p.body[0] == 'l' && p.body[1] == '1'){
          p.body[2] = ((led1/1000)%10)+'0';
          p.body[3] = ((led1/100)%10)+'0';
          p.body[4] = ((led1/10)%10)+'0';
          p.body[5] = ((led1)%10)+'0';
        } else if(p.body[0] == 'l' && p.body[1] == '2'){
          p.body[2] = ((led2/1000)%10)+'0';
          p.body[3] = ((led2/100)%10)+'0';
          p.body[4] = ((led2/10)%10)+'0';
          p.body[5] = ((led2)%10)+'0';
        } else if(p.body[0] == 'b' && p.body[1] == 't'){
          p.body[2] = ((botao/1000)%10)+'0';
          p.body[3] = ((botao/100)%10)+'0';
          p.body[4] = ((botao/10)%10)+'0';
          p.body[5] = ((botao)%10)+'0';
        } else if(p.body[0] == 'b' && p.body[1] == 'z'){
          p.body[2] = ((buzzer/1000)%10)+'0';
          p.body[3] = ((buzzer/100)%10)+'0';
          p.body[4] = ((buzzer/10)%10)+'0';
          p.body[5] = ((buzzer)%10)+'0';
        }
        p.body[6] = '\r';
        p.body[7] = '\n';
        break;
      case POST:
        if(p.body[0] == 'l' && p.body[1] == '1'){
          led1 = (p.body[5]=='1')?(1):(0);
          p.body[2] = ((led1/1000)%10)+'0';
          p.body[3] = ((led1/100)%10)+'0';
          p.body[4] = ((led1/10)%10)+'0';
          p.body[5] = ((led1)%10)+'0';
        } else if(p.body[0] == 'l' && p.body[1] == '2'){
          led2 = (p.body[5]=='1')?(1):(0);
          p.body[2] = ((led2/1000)%10)+'0';
          p.body[3] = ((led2/100)%10)+'0';
          p.body[4] = ((led2/10)%10)+'0';
          p.body[5] = ((led2)%10)+'0';
        } else if(p.body[0] == 'b' && p.body[1] == 't'){
          p.body[2] = ((botao/1000)%10)+'0';
          p.body[3] = ((botao/100)%10)+'0';
          p.body[4] = ((botao/10)%10)+'0';
          p.body[5] = ((botao)%10)+'0';
        } else if(p.body[0] == 'b' && p.body[1] == 'z'){
          buzzer = (p.body[5]=='1')?(1):(0);
          p.body[2] = ((buzzer/1000)%10)+'0';
          p.body[3] = ((buzzer/100)%10)+'0';
          p.body[4] = ((buzzer/10)%10)+'0';
          p.body[5] = ((buzzer)%10)+'0';
        }
        p.body[6] = '\r';
        p.body[7] = '\n';
        break;
      case PUT:
        /* Nao implementado */
        break;
      case DELETE:
        /* Nao implementado */
        break;
    }
  }

  PackageToByteArray(&p, buffer);
  
  Serial.write(buffer, MAX_BUFFER);
  Serial.flush();
  
}

void loop(){
  digitalWrite(LED1, led1);
  digitalWrite(LED2, led2);

  if(buzzer == 1){
    analogWrite(BUZZER, 127);
  } else {
    analogWrite(BUZZER, 0);
  }
  

  botao = analogRead(BOTAO);
  
  if(Serial.available()){
    char tmp = Serial.read();
    
    buffer[tamBuffer] = tmp;
    tamBuffer++;
    
    if( (buffer[tamBuffer-2] == '\r' && buffer[tamBuffer-1] == '\n') ||
        tamBuffer >= MAX_BUFFER){
      callbackSerial(buffer, tamBuffer);
      for(;tamBuffer>0; tamBuffer--){
        buffer[tamBuffer] = '\0';
      }
      buffer[0] = '\n';
      tamBuffer = 0;
    }
  }
}
