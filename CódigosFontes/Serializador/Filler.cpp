// Filler.cpp
//
// Implementação da classe Filler (Preenchedor).
//
// A Classe Filler é responsável por receber as informações de
// campos e organizar a estrutura na forma de subcampos.
// Os subcampos são organizados em bytes, separando os campos
// quando necessário.
// Cada instância de Filler representa 1 byte, quando ele se
// preenche então é criada uma nova instância com o restante
// que faltou para ser preenchido. Ao final do processamento é
// possível processá-lo como uma Lista Encadeada.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


#include <iostream>
#include <sstream>
#include <string>

#include "Filler.h"

using namespace std;

// Construtor da Classe Filler, inicializa os campos com os valores padrão
Filler::Filler(){
	//this->subCampos;
	this->id = 0;
	this->tamanhoAtual = 0;
	this->tamanhoMaximo = TAM_MAX;
	this->prox = NULL;
}

// Construtor da Classe Filler, inicializa os campos com os valores padrão
// e o id com o valor do parâmetro de entrada
Filler::Filler(int id){
	//this->subCampos;
	this->id = id;
	this->tamanhoAtual = 0;
	this->tamanhoMaximo = TAM_MAX;
	this->prox = NULL;
}

// Retorna o valor do espaço que ainda falta preencher no filler.
int Filler::getTamanhoLivre(){
	return (this->tamanhoMaximo - this->tamanhoAtual);
}

// Adiciona um novo campo no Filler, recebe como parâmetro o nome do campo e o seu tamanho.
Filler* Filler::adicionaCampo(string nomeCampo, int tamanho){
	if(tamanho + this->tamanhoAtual >= this->tamanhoMaximo){ // Inserção é maior que o que deveria
		// Cria um novo subcampo com o espaço que falta
		SubCampo* sCampo = new SubCampo(nomeCampo, tamanho - this->getTamanhoLivre(), tamanho);
		
		this->subCampos.push_back(sCampo); // Insere o subcampo no Filler
		
		this->prox = new Filler(this->id+1); // Cria um novo Filler para inserir o resto dos bits do campo inserido
		Filler* proximo = this->prox; // Encadeia o Filler para processamento futuro
		if(tamanho - this->getTamanhoLivre() > 0){ // Verifica se ainda há bits para inserir
			// Adiciona o restante do campo no próximo Filler (Recursão)
			proximo = this->prox->adicionaCampo(nomeCampo, (tamanho - this->getTamanhoLivre()));
		}
		
		// Reduz o espaço disponível do Filler atual para 0
		this->tamanhoAtual += this->getTamanhoLivre();
		return proximo; // Retorna a referência do próximo Filler com espaço para preencher
	} else { // Inserção não estoura a capacidade do Filler
		// Cria um novo subcampo com as informações recebida nos parâmetros
		SubCampo* sCampo = new SubCampo(nomeCampo, 0, tamanho);
		// Insere novo subcampo ao Filler atual
		this->subCampos.push_back(sCampo);
		
		// Reduz espaço disponível do Filler atual pelo tamanho do campo inserido
		this->tamanhoAtual += tamanho;
		
		// Retorna o próprio Filler pois ainda é possível inserir mais campos.
		return this;
	}
	
}

// Função get padrão
vector<SubCampo*> Filler::getSubCampos(){
	return this->subCampos;
}

// Função get padrão
int Filler::getTamanhoAtual(){
	return this->tamanhoAtual;
}

// Função get padrão
Filler* Filler::getProximo(){
	return this->prox;
}

// Função para debug, retorna o Filler em formato de String.
string Filler::toString(){
	/*vector<SubCampo> subCampos;
	int tamanhoAtual;
	Filler* prox;*/
	
	stringstream ss;
	
	ss << "Filler:{\"id\":" << this->id << ",\"tamanhoAtual\":\"" << this->tamanhoAtual << "\"";
	
	if(!this->subCampos.empty()){
		ss << ",\"subCampos\":[";
		vector<SubCampo*>::iterator it;
		for(it = this->subCampos.begin(); (it+1) < this->subCampos.end(); it++){
			ss << (*it)->toString() << ",";
		}
		ss << (*it)->toString() << "]";
	}
	if(prox != NULL){
		ss << ",\"prox\":" << prox->toString();
	}
	ss << "}";
	
	return ss.str();
}
