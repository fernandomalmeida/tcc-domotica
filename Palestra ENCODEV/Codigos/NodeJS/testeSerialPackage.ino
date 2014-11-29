#include <Package.h>

#define MAX_BUFFER SIZE_TOTAL_PACKAGE

unsigned char buffer[MAX_BUFFER] = "";
int tamBuffer = 0;

Package p;

void setup(){
  Serial.begin(9600);
  
  createPackage(&p, 1, 2, 1, 1, 0, (char*)"Teste de Pacote");
  PackageToByteArray(&p, buffer);
  Serial.write(buffer, MAX_BUFFER);

}

void callbackSerial(unsigned char buffer[], int tamBuffer){
  ByteArrayToPackage(buffer, &p);

  p.id += 1;

  PackageToByteArray(&p, buffer);
  //Serial.println((char*)buffer);
  Serial.write(buffer, MAX_BUFFER);
  //Serial.print(buffer);
}

void loop(){
  if(Serial.available()){
    char tmp = Serial.read();
    
    buffer[tamBuffer] = tmp;
    tamBuffer++;
    
    if( (buffer[tamBuffer-2] == '\r' && buffer[tamBuffer-1] == '\n') ||
        tamBuffer >= MAX_BUFFER){
      callbackSerial(buffer, tamBuffer);
      for(tamBuffer--;tamBuffer>0; tamBuffer--){
        buffer[tamBuffer] = '\0';
      }
      buffer[0] = '\n';
      delay(1000);
    }
  }
}
