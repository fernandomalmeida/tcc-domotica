#define BOTAO A0

void setup(){
  pinMode(BOTAO, INPUT);

  Serial.begin(9600);
}

void loop(){
  boolean botao = digitalRead(BOTAO);

  if(botao){
    Serial.println("Botao Solto!");
  } else {
    Serial.println("Botao Apertado!");
  }
  delay(1000);
}
