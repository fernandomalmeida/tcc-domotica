// tcc_mstr1.ino
// 
// Código Fonte do Nó mstr1, o nó mestre da rede
//
// Fernando Mendonça de Almeida, Agosto de 2013
//

#include <Request.h>

#define PACKET_SIZE 32

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

const int ledDebug = 3;

void setup(){
  // Inicializa a porta serial com velocidade de 57600, 8n1 (Configuração padrão)
  Serial.begin(57600);

  // Altera o modo do pino do led debug para saída
  pinMode(ledDebug, OUTPUT);
  // apaga o led de debug
  digitalWrite(ledDebug, LOW);
  
  // Inicializa o hardware da comunicação SPI
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  // Define o endereço de recebimento de pacotes como 'mstr1'
  Mirf.setRADDR((byte *) "mstr1");
  
  // Define o tamanho do pacote para 32 bytes (máximo)
  Mirf.payload = PACKET_SIZE*sizeof(unsigned char);
  
  // Configura a biblioteca de comunicação com o módulo de comunicação sem fio NRF24L01
  Mirf.config();
}

void checagem(){
  if(!Mirf.isSending() && Mirf.dataReady()){ // Se o módulo não estiver enviado informações e houver algum pacote para processar
    byte data[PACKET_SIZE];
	// Lê o pacote que está no módulo
    Mirf.getData(data);
    
    Request req;
	// Guardar as informações do pacote em uma estrutura
    toRequest(&req, (unsigned char*) &data);
    
    // addr1 -> endereço destino
    unsigned char address = req.addr1;

    switch(address){
      case 1: // Se o endereço de destino for o node1
        // Envia o pacote para node1
        Mirf.setTADDR((byte*) "node1");
        Mirf.send(data);
        break;
      case 2: // Se o endereço de destino for o node2
        // Envia o pacote para node2
        Mirf.setTADDR((byte*) "node2");
        Mirf.send(data);
        break;
      default:
        // Caso contrário, envia o pacote para o nó de fronteira
        req.rOr = 1;
        fromRequest((unsigned char*) data, &req);
        for(int i=0; i<PACKET_SIZE; i++){
          Serial.write(data[i]);
        }
    }
  }
}

void checagemSerial(){ // Verifica se o nó de fronteira enviou alguma informação
  if(Serial.available() == PACKET_SIZE){ // Se há pacotes disponíveis na porta Serial
    Request req;
    unsigned char reqSerial[PACKET_SIZE];
    
	// Lê o pacote da porta serial
    Serial.readBytes((char*)reqSerial, PACKET_SIZE);
    
	// Guarda o pacote em uma estrutura
    toRequest(&req, (unsigned char*)reqSerial);
    
    unsigned char address = req.addr1;
	
	// verifica para quem o pacote está endereçado
    switch(address){
      case 1: // Se estiver endereçado para o node1
		// Envia para o node1
        Mirf.setTADDR((byte*) "node1");
        Mirf.send((byte*) reqSerial);
        digitalWrite(ledDebug, HIGH);
        break;
      case 2: // Se estiver endereçado para o node2
		// Envia para o node2
        Mirf.setTADDR((byte*) "node2");
        Mirf.send((byte*) reqSerial);
        break;
      default: // Se estiver endereçado para outro
		// Envia para o nó de fronteira
        req.rOr = 1;
        req.addr1 = req.addr2;
        req.addr2 = address;
        fromRequest((unsigned char*) reqSerial, &req);
        for(int i=0; i<PACKET_SIZE; i++){
          Serial.write(reqSerial[i]);
        }
    }
  }
}

void loop(){
  // Verifica se há algum pacote no módulo de comunicação sem fio
  checagem();
  // Verifica se há algum pacote na porta Serial
  checagemSerial();
} 
