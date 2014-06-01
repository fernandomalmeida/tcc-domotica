// Request.h
//
// Declarações da classe Request
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

#ifndef _REQUEST_H_
#define _REQUEST_H_

// Enumeração para ler de forma mais fácil qual a requisição do pacote
typedef enum _HTTPRequest{
  GET = 0,
  POST,
  PUT,
  DELETE
} HTTPRequest;

// Estrutura que armazena as informações do pacote
typedef struct _Request{
	unsigned char addr1; // Endereço1
	unsigned char addr2; // Endereço 2
	unsigned char id; // Identificador de pacote
	unsigned char rOr; // Requisição ou Resposta?
	unsigned char rInfo; // Informação da requisição/resposta
	
	unsigned char msg[28]; // Mensagem do pacote
} Request;

// Cria uma requisição com os dados passados por parâmetro, é preciso inicializar tas as informações
Request* assemblyRequest(Request *req, unsigned char addr1, unsigned char addr2, unsigned char id, unsigned char rOr, unsigned char rInfo, unsigned char *msg);
// Cria um vetor de bytes a partir de uma Requisição
unsigned char* fromRequest(unsigned char *reqSerial, Request *req);
// Preenche uma Requisição a partir de um vetor de bytes
Request* toRequest(Request *req, unsigned char *reqSerial);
// Imprime uma requisição na porta Serial (Debug)
void printRequest(Request *req);
// Função auxiliar para debug, ao passar o tipo de requisição
char* toHTTPRequest(HTTPRequest httpReq);

#endif