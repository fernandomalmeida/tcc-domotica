#define BOTAO A0

void setup(){
  pinMode(BOTAO, INPUT);

  Serial.begin(9600);
}

void loop(){
  int botao = analogRead(BOTAO);

  if(botao < 69){
    Serial.println("Right!");
  } else if(botao < 230){
    Serial.println("Up!");
  } else if(botao < 407){
    Serial.println("Down!");
  } else if(botao < 630){
    Serial.println("Left!");
  } else if(botao < 896){
    Serial.println("SEL!");
  } else {
    Serial.println("Todos soltos...");
  }
  
  delay(1000);
}
