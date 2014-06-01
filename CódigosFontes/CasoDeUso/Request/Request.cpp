// Request.cpp
//
// Implementações da classe Request
//
// A Classe Request é responsável por armazenar a estrutura com
// as informações do pacote da rede assim como possuir as funções
// de empacotamente e desempacotamento. (Estrutura para vetor de
// bytes e vice versa).
// Sua implementação se baseia em um TAD (Tipo Abstrato de Dado)
// porque o C não suporta Classes.
// 
// Fernando Mendonça de Almeida, Agosto de 2013
//

#include <Arduino.h>

#include "Request.h"

#define TAM_MSG 29
#define COMECO_MSG 3

// Preenche uma estrutura Requisição com os valores recebidos por parâmetro
Request* assemblyRequest(Request *req, unsigned char addr1, unsigned char addr2, unsigned char id, unsigned char rOr, unsigned char rInfo, unsigned char *msg){
	int i;

	req->addr1 = addr1;
	req->addr2 = addr2;
	req->id = id;
	req->rOr = rOr;
	req->rInfo = rInfo;
	
	for(i=0; i<TAM_MSG; i++){
		req->msg[i] = msg[i];
	}
	
	return req;
}

// Preenche um vetor de bytes a partir de uma estrutura
unsigned char* fromRequest(unsigned char *reqSerial, Request *req){
	int i;
	
	// Instruções geradas com o programa Serializador (Deslocamentos de 0 bits foram removidos para otimizar o processamento)
	reqSerial[0] = ((unsigned char)(req->addr1));
	reqSerial[1] = ((unsigned char)(req->addr2));
	reqSerial[2] = ((unsigned char)(req->id << 4) & 0xf0) + ((unsigned char)(req->rOr << 3) & 0x8) + ((unsigned char)(req->rInfo << 0) & 0x7);
	
	for(i=0; i<TAM_MSG; i++){
		reqSerial[i+COMECO_MSG] = req->msg[i];
	}
	
	return reqSerial;
}

// Preenche uma estrutura a partir de um vetor de bytes
Request* toRequest(Request *req, unsigned char *reqSerial){
	int i;

	// Instruções geradas com o programa Serializador (Deslocamentos de 0 bits e máscaras completas, 0xff, foram removidos para otimizar processamento)
	req->addr1 = (((unsigned char)reqSerial[0]));
	req->addr2 = (((unsigned char)reqSerial[1]));
	req->id = (((unsigned char)reqSerial[2] >> 4) & 0xf);
	req->rOr = (((unsigned char)reqSerial[2] >> 3) & 0x1);
	req->rInfo = (((unsigned char)reqSerial[2]) & 0x7);
	
	for(i=0; i<TAM_MSG; i++){
		req->msg[i] = reqSerial[i+COMECO_MSG];
	}
	
	return req;
}

// Imprime as informações de uma requisição na porta Serial
void printRequest(Request *req){
	unsigned char addr1;
	unsigned char addr2;
	unsigned char id;
	unsigned char rOr;
	unsigned char rInfo;
	Serial.print("Addr1: ");
	Serial.print(req->addr1, DEC);
	Serial.println();
	Serial.print("Addr2: ");
	Serial.print(req->addr2, DEC);
	Serial.println();
	Serial.print("id: ");
	Serial.print(req->id, DEC);
	Serial.println();
	Serial.print("rOr: ");
	Serial.print(req->rOr, DEC);
	Serial.println();
	Serial.print("rInfo: ");
	Serial.print(req->rInfo, DEC);
	Serial.println();
	Serial.print("Body: ");
	Serial.print((char*)req->msg);
	Serial.println();
}

// Recebe o tipo de requisição e retorna-a como uma string
char* toHTTPRequest(HTTPRequest httpReq){
	switch(httpReq){
		case GET:
			return "GET";
			break;
		case POST:
			return "POST";
			break;
		case PUT:
			return "PUT";
			break;
		case DELETE:
			return "DELETE";
			break;
	}
	return "WRONG";
}
