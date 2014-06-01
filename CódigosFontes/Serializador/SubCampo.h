// SubCampo.h
//
// Declarações da classe SubCampo.
//
// A Classe SubCampo guarda o nome do campo, o bit inicial e seu tamanho.
// Sua função é guardar as informações de onde começa e termina cada campo
// dentro dos Preenchedores. As informações de deslocamento de bits são
// retiradas dessa estrutura.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


#ifndef _SUB_CAMPO_H_
#define _SUB_CAMPO_H_

#include <string>

using namespace std;

class SubCampo{
	string nomeCampo;
	int bitInicial;
	int tamanho;
	
public:
	SubCampo(string nomeCampo, int bitInicial, int tamanho);
	string getNomeCampo();
	int getBitInicial();
	int getTamanho();
	string toString();
};

#endif