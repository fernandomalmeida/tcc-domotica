#define MAX_BUFFER 100

char buffer[MAX_BUFFER] = "";
int tamBuffer = 0;
void setup(){
  Serial.begin(9600);
  
  Serial.println("Ping");
}

void callbackSerial(char buffer[], int tamBuffer){
    Serial.print(buffer);
}

void loop(){
  if(Serial.available()){
    char tmp = Serial.read();
    static char last = '\0';
    
    buffer[tamBuffer] = tmp;
    tamBuffer++;
    
    //if( (buffer[tamBuffer-2] == '\r' && buffer[tamBuffer-1] == '\n') ||
    if( (last == '\r' && tmp == '\n') ||
        tamBuffer >= MAX_BUFFER){
      callbackSerial(buffer, tamBuffer);
      for(tamBuffer--;tamBuffer>0; tamBuffer--){
        buffer[tamBuffer] = '\0';
      }
      buffer[0] = '\n';
      delay(1000);
    }

    last = tmp;
  }
}
