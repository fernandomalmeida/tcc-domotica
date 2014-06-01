// SubCampo.cpp
//
// Implementação da classe SubCampo.
//
// A Classe SubCampo guarda o nome do campo, o bit inicial e seu tamanho.
// Sua função é guardar as informações de onde começa e termina cada campo
// dentro dos Preenchedores. As informações de deslocamento de bits são
// retiradas dessa estrutura.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


#include <iostream>
#include <string>
#include <sstream>

#include "SubCampo.h"

using namespace std;

SubCampo::SubCampo(string nomeCampo, int bitInicial, int tamanho){
	this->nomeCampo = nomeCampo;
	this->bitInicial = bitInicial;
	this->tamanho = tamanho;
}

string SubCampo::getNomeCampo(){
	return this->nomeCampo;
}

int SubCampo::getBitInicial(){
	return this->bitInicial;
}

int SubCampo::getTamanho(){
	return this->tamanho;
}

string SubCampo::toString(){
	stringstream ss;
	
	ss << "SubCampo:{\"nomeCampo\":\"" << this->nomeCampo << "\",\"bitInicial\":\"" << this->bitInicial << "\",\"tamanho\":\"" << this->tamanho << "\"}";
	
	return ss.str();
}
