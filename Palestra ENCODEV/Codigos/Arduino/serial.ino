#define LED 13

void setup(){
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop(){
  Serial.println("Led aceso!");
  digitalWrite(LED, HIGH);
  delay(1000);
  Serial.println("Led apagado!");
  digitalWrite(LED, LOW);
  delay(1000);
}
