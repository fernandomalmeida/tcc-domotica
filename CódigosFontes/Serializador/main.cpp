// main.cpp
//
// Classe principal do programa gerador de código para
// serialização e desserialização de um pacote.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


#define DEBUG
#undef DEBUG // Define que não é a versão de Teste

// Tamanho máximo para os campos, caso deseje um nome de campo maior alterar esse valor
#define TAM_BUFFER 32

// Inclusão das bibliotecas padrões
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>

// Inclusão das bibliotecas locais
#include "Filler.h"
#include "SubCampo.h"


using namespace std;

int getMask(int num); // Função comentada no final do arquivo

int main(){
	int numCampos;
	int numBits;
	
	Filler* fillerCampos;
	
	Filler* fillerAux;
	
	
	char bufferChar[TAM_BUFFER]; // Buffer para receber o nome do campo
	char nomeStruct[] = "req"; // Nome da estrutura (para geração de código)
	char nomeSerial[] = "reqSerial"; // Nome do vetor e bytes (para geração de código)
	int bufferInt; // Buffer para receber número de bits
	
	int i;
	int index;
	
	fillerCampos = new Filler(); // Filler inicial
	fillerAux = fillerCampos; // Filler auxiliar, irá receber sempre o Filler que pode ter um campo adicionado
	int success;
	numCampos = 0;
	while((success = scanf("%[^:\n]s", bufferChar))>0){ // Leitura do nome do campo (Lê a string até um ':' ou um '\n')
		scanf("%*c"); // Lê o ':', mas não armazena em nenhuma variável
		scanf("%d", &bufferInt); // Lê o número de bits do campo
		scanf("%*c"); // Lê a quebra de linha, mas não armazena em nenhuma variável
		numCampos += 1; // Incrementa o número de campos
		
		// Adiciona o novo campo ao Filler não totalmente preenchido
		fillerAux = fillerAux->adicionaCampo(bufferChar, bufferInt);
	}
	
#ifdef DEBUG
	printf("%s\n", nomeSerial);
#endif
	
	// Lê o número de bits totais para debug
	// Começa aqui [1]
	numBits = 0;
	
	fillerAux = fillerCampos;
	do{
		vector<SubCampo*> subCampos = fillerAux->getSubCampos();
		vector<SubCampo*>::iterator it;
		for(it = subCampos.begin(); it < subCampos.end(); it++){
			numBits += ((*it)->getTamanho() - (*it)->getBitInicial());
		}
		fillerAux = fillerAux->getProximo();
	} while(fillerAux != NULL);
	// Termina aqui [1]
	
#ifdef DEBUG
	printf("%d,%d\n", numCampos, numBits);
#endif
	
#ifdef DEBUG
	cout << "Serializacao" << endl << endl;
#endif
	
	// Faz a leitura como uma lista encadeada dos Fillers.
	fillerAux = fillerCampos;
	index = 0;
	do{ // Para cada Filler, faça:
		int tamanhoFaltante = fillerAux->getTamanhoAtual();
		vector<SubCampo*> subCampos = fillerAux->getSubCampos();
		vector<SubCampo*>::iterator it;
		
		cout << nomeSerial << "[" << index << "] = "; // Imprima o lado esquerdo da equação
		for(it = subCampos.begin(); it < subCampos.end(); it++){ // Para cada subcampo do Filler, faça:
			
			int aux = getMask((*it)->getTamanho() - (*it)->getBitInicial());
			
			if(it != subCampos.begin()){ // Se não é o primeiro subcampo, acrescente-o ao valor anterior
				cout << " + ";
			}
			
			if((*it)->getBitInicial() == 0){ // Se o bit inicial for o primeiro, então ainda há subcampos para ler
				// Acrescenta à equação o valor do subcampo na posição correta dentro do byte do vetor
				cout << "((unsigned char)(" << nomeStruct << "->" << (*it)->getNomeCampo().c_str() << " << " << (tamanhoFaltante - (*it)->getTamanho()) << ") & 0x" << hex << ((aux)<<(tamanhoFaltante - (*it)->getTamanho())) << ")";
#ifdef DEBUG
				printf("%s,%c,%d,%x\n", (*it)->getNomeCampo().c_str(), '<', (tamanhoFaltante - (*it)->getTamanho()),((aux)<<(tamanhoFaltante - (*it)->getTamanho())));
#endif
				tamanhoFaltante -= (*it)->getTamanho();
			} else { // Se o bit inicial não for o primeiro, não há mais subcampos para ler (o tamanhoFaltante se tornará nulo)
				// Acrescenta à equação o valor do subcampo na posição correta dentro do byte do vetor
				cout << "((unsigned char)(" << nomeStruct << "->" << (*it)->getNomeCampo().c_str() << " >> " << ((*it)->getBitInicial()) << ") & 0x" << hex <<  (aux) << ")";
#ifdef DEBUG
				printf("%s,%c,%d,%x\n", (*it)->getNomeCampo().c_str(), '>', (*it)->getBitInicial(), aux);
#endif
				tamanhoFaltante = 0;
			}
			
		}
		cout << ";" << endl;
#ifdef DEBUG
		printf("----\n");
#endif
		index++;
		fillerAux = fillerAux->getProximo();
	} while(fillerAux != NULL);
	
#ifdef DEBUG
	cout << "Desserializacao" << endl << endl;
#endif
	
	// Faz a leitura dos Fillers também como uma lista encadeada
	index = 0;
	fillerAux = fillerCampos;
	string nomeAtual = "";
	int primeiro = 1;
	do{ // Para cada Filler, faça:
		int tamanhoFaltante = fillerAux->getTamanhoAtual();
		vector<SubCampo*> subCampos = fillerAux->getSubCampos();
		vector<SubCampo*>::iterator it;
		for(it = subCampos.begin(); it < subCampos.end(); it++){ // Para cada SubCampo, faça:
			if(nomeAtual == "" || nomeAtual != (*it)->getNomeCampo()){ // Se houver mudança de campo
				if(!primeiro){
					cout << ";" << endl; // Quebre a linha, caso não seja o primeiro
				}
				if(nomeAtual == ""){
					primeiro = 0;
				}
				nomeAtual = (*it)->getNomeCampo();
				
				cout << nomeStruct << "->" << nomeAtual << " = "; // Imprima o lado esquerdo da equação
#ifdef DEBUG
				cout << "----" << endl;
				cout << nomeAtual << endl;
#endif
			} else {
				cout << " + "; // Se não houver mudança de campo, apenas some com o próximo elemento
			}
			
			int aux = getMask((*it)->getTamanho() - (*it)->getBitInicial());
			
			if((*it)->getBitInicial() == 0){
				// Acrescenta à equação a parte do campo pertencente ao byte na posição do index
				cout << "(((unsigned short int)" << nomeSerial << "[" << index << "] >> " << (tamanhoFaltante - (*it)->getTamanho()) << ") & 0x" << hex << (aux) << dec << ")";
#ifdef DEBUG
				printf("%s,%c,%d,%x\n", (*it)->getNomeCampo().c_str(), '>', (tamanhoFaltante - (*it)->getTamanho()),(aux));
#endif
				tamanhoFaltante -= (*it)->getTamanho();
			} else {
				// Acrescenta à equação a parte do campo pertencente ao byte na posição do index
				cout << "(((unsigned short int)" << nomeSerial << "[" << index << "] << " << ((*it)->getBitInicial()) << ") & 0x" << hex << (aux << (*it)->getBitInicial()) << dec << ")";
#ifdef DEBUG
				printf("%s,%c,%d,%x\n", (*it)->getNomeCampo().c_str(), '<', (*it)->getBitInicial(), (aux << (*it)->getBitInicial()));
#endif
				tamanhoFaltante = 0;
			}
			
		}
		index++;
		fillerAux = fillerAux->getProximo();
	} while(fillerAux != NULL);
	
	cout << endl << endl;
	
	
	return 0;
}

// Retorna uma máscara AND com 'num' número de bits
// Ex; getMask(3) -> 0b00000111
//	   getMask(5) -> 0b00011111
int getMask(int num){
	int ret = 0;
	int i;
	for(i=0; i<num; i++){
		ret = ret << 1;
		ret = ret + 1;
	}
	
	return ret;
}



