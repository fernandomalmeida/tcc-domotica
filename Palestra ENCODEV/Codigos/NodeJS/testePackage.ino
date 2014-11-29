#include <Package.h>

Package p;

void setup(){
  Serial.begin(9600);
  
  createPackage(&p, 1, 2, 1, 1, 0, (char*)"Teste de Pacote");
}

void loop(){
  printPackage(&p);
  
  delay(1000);
}
