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


const int ldrPin = A0; // Pino do ldr
// variável que armazena o menor valor obtido do ldr
volatile unsigned int minLdrValue = 1023;
// variável que armazena o maior valor obtido do ldr
volatile unsigned int maxLdrValue = 0;

// valor atual do ldr
volatile unsigned int ldrValue = 0;

// variável que armazena se o nó deve funcionar no modo automático
volatile unsigned char autoLdr = 0;

// identificador único (para fazer requisições)
const unsigned char id = 0;

void setup(){
  // Inicializa a porta serial com velocidade de 57600, 8n1 (Configuração padrão)
  Serial.begin(57600);

  pinMode(3, OUTPUT); // Define o pino como saída

  // Inicializa o hardware da comunicação SPI
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  // Define o endereço de recebimento de pacotes como 'node2'
  Mirf.setRADDR((byte *) "node2");
  
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
    
    // addr1 -> endereço destino
    unsigned char address = req.addr1;
	// Se não for uma requisição, termine o processamento do pacote
	// Esse módulo não processa a resposta
    if(req.rOr != 0){
      Serial.println("Received Response!");
      return;
    }

    unsigned char responseCode = 2;
    unsigned char msgBuffer[PACKET_SIZE];

    unsigned int out;
    unsigned char pwm = 0;
    switch(req.rInfo){ // Verifica qual o método
      case POST:
		// Se for POST, atualiza o funcionamento automático ou não do nó
        autoLdr = req.msg[0];

        responseCode = 0;

        msgBuffer[0] = autoLdr;

        break;
      case GET:
		// Se for GET informa se o nó está funcionando de forma automática ou não
        responseCode = 0;

        msgBuffer[0] = autoLdr;

        break;
    }


	// Prepara o envio para o nó mstr1
    Mirf.setTADDR((byte *)"mstr1");
    
	// Configura o pacote com as informações de resposta
    unsigned char addr1 = 2;
    unsigned char addr2 = req.addr2;
    unsigned char id = req.id;
    assemblyRequest(&req, addr2, addr1, id, 1, responseCode, msgBuffer);
    Serial.println("RESPOSTA: ");
    //printRequest(&req);
    fromRequest((unsigned char*) &data, &req);

	// Envia o pacote
    Mirf.send(data);
  }
}

void acao(){
  // Armazena a informação da última chamada de ação
  static unsigned int lastTime = 0;

  int tempoAgora = millis();
  // Se passou 1 segundo, deve chamar a função novamente
  unsigned char verificacao = (tempoAgora-1000) > lastTime;
  /*Serial.print("Verificacao: ");
  Serial.println(autoLdr && verificacao);*/

  if(autoLdr && verificacao){ // Se estiver em modo automático e já passou mais de 1 segundo desde a última verificação
	// Atualize o último tempo
    lastTime = millis();

    Serial.println("SendData to node1");
    byte data[PACKET_SIZE];

//    if(autoLdr){
      Request req;

      byte msgBuffer[PACKET_SIZE];

      unsigned char out = ldrValue;
      Serial.print("ldr: ");
      Serial.println(ldrValue);
      Serial.print("minLdr: ");
      Serial.println(minLdrValue);
      Serial.print("maxLdr: ");
      Serial.println(maxLdrValue);
      
      // Define a % de PWM do node1 de acordo com o valor obtido de luminosidade (Considerando o menor e o maior valor medido
      out = (unsigned char)255*((float)ldrValue-(float)minLdrValue)/((float)maxLdrValue-(float)minLdrValue);
      
      out = 255 - out;
      
      msgBuffer[0] = out;
      msgBuffer[1] = out;
      msgBuffer[2] = out;
      
      
      Serial.print("out: ");
      Serial.println(out);


      /*msgBuffer[0] = 'g';
      msgBuffer[1] = (out/100)%10 + '0';
      msgBuffer[2] = (out/10)%10 + '0';
      msgBuffer[3] = (out)%10 + '0';*/
	  // Prepara para enviar o pacote para o node1
      Mirf.setTADDR((byte*) "node1");
	  
	  // Monta o pacote com as informações
      assemblyRequest(&req, 1, 2, id, 0, POST, msgBuffer);
      fromRequest((unsigned char*) &data, &req);
      
	  // Envia o pacote
      Mirf.send(data);
//    }
  }
  //i = (i+1)%10000;
}

void loop(){
  // Informa no led se está funcionando em modo automático
  digitalWrite(3, autoLdr);
  // Lê o nível de luminosidade
  ldrValue = analogRead(ldrPin);
  
  // Atualiza o menor valor
  if(ldrValue < minLdrValue){
    minLdrValue = ldrValue;
  }
  
  // Atualiza o maior valor
  if(ldrValue > maxLdrValue){
    maxLdrValue = ldrValue;
  }
  
  // Verifica se há algum pacote no módulo de comunicação sem fio
  checagem();
  // Executa a ação do nó
  acao();
} 
