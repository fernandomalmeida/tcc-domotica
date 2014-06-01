// tcc_node1.ino
// 
// Código Fonte do Nó node1
//
// Fernando Mendonça de Almeida, Agosto de 2013
//

#include <Request.h>

#define PACKET_SIZE 32

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

const int ledPinR = 5; // Pino referente à cor vermelha do led RGB
const int ledPinG = 3; // Pino referente à cor verde do led RGB
const int ledPinB = 6; // Pino referente à cor azul do led RGB
volatile unsigned char pwmR = 10; // Valor inicial para o brilho do tom vermelho do led RGB
volatile unsigned char pwmG = 10; // Valor inicial para o brilho do tom verde do led RGB
volatile unsigned char pwmB = 10; // Valor inicial para o brilho do tom azul do led RGB

void setup(){
  // Inicializa a porta serial com velocidade de 57600, 8n1 (Configuração padrão)
  Serial.begin(57600);

  pinMode(ledPinR, OUTPUT); // Define o pino como saída
  pinMode(ledPinG, OUTPUT); // Define o pino como saída
  pinMode(ledPinB, OUTPUT); // Define o pino como saída
  
  // Inicializa o hardware da comunicação SPI
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  // Define o endereço de recebimento de pacotes como 'node1'
  Mirf.setRADDR((byte *) "node1");
  
  // Define o tamanho do pacote para 32 bytes (máximo)
  Mirf.payload = PACKET_SIZE*sizeof(unsigned char);
   
  // Configura a biblioteca de comunicação com o módulo de comunicação sem fio NRF24L01
  Mirf.config();

  Serial.println("Listening...");
}

void checagem(){
  if(!Mirf.isSending() && Mirf.dataReady()){ // Se o módulo não estiver enviado informações e houver algum pacote para processar
    byte data[PACKET_SIZE];
	// Lê o pacote que está no módulo
    Mirf.getData(data);
    
    Request req;
	// Guardar as informações do pacote em uma estrutura
    toRequest(&req, (unsigned char*) &data);
    
    Serial.println("PACOTE RECEBIDO:");
    printRequest(&req);
    // addr1 -> endereço destino
    unsigned char address = req.addr1;
	
	// Se não for uma requisição, aborte a mensagem
	// (Esse nó não pode receber uma resposta
    if(req.rOr != 0)
      return;

    unsigned char responseCode = 2;
    unsigned char msgBuffer[PACKET_SIZE];

    unsigned int out;
    switch(req.rInfo){ // Verifica qual o método
      case POST:
		// Se for POST, atualize os valores de PWM com os recebidos no pacote
        pwmR = req.msg[0];
        pwmG = req.msg[1];
        pwmB = req.msg[2];

        responseCode = 0;
		
		// Enivie como resposta os valores atualizados
        msgBuffer[0] = pwmR;
        msgBuffer[1] = pwmG;
        msgBuffer[2] = pwmB;
        msgBuffer[3] = '\0';

        break;
      case GET:
		// Se for GET, envie os valores atuais de PWM
        responseCode = 0;

        msgBuffer[0] = pwmR;
        msgBuffer[1] = pwmG;
        msgBuffer[2] = pwmB;
        msgBuffer[3] = '\0';

        break;
    }

	// Prepara o envio para o nó mstr1
    Mirf.setTADDR((byte *)"mstr1");
    
	// Configura o pacote com as informações de resposta
    unsigned char addr1 = 1;
    unsigned char addr2 = req.addr2;
    unsigned char id = req.id;
    assemblyRequest(&req, addr2, addr1, id, 1, responseCode, msgBuffer);
    Serial.println("RESPOSTA: ");
    printRequest(&req);
    fromRequest((unsigned char*) &data, &req);
	
	// Envia o pacote
    Mirf.send(data);
  }
}

void loop(){
  // Escreve o nível de pwm nos três pinos do led RGB
  analogWrite(ledPinR, pwmR);
  analogWrite(ledPinG, pwmG);
  analogWrite(ledPinB, pwmB);
  // Verifica se há algum pacote no módulo de comunicação sem fio
  checagem();
} 
