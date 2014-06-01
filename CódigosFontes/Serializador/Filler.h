// Filler.h
//
// Declarações da classe Filler (Preenchedor).
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


#ifndef _FILLER_H_
#define _FILLER_H_

#define TAM_MAX 8

#include <vector>
#include "SubCampo.h"

using namespace std;

class Filler {
	int id; // Identificador do Filler, é usado como índice do vetor de bytes que ele representa
	vector<SubCampo*> subCampos; // Vetor de subcampos pertencentes ao Filler
	int tamanhoAtual; // Tamanho atual do Filler, é um fator de decisão para saber como o novo
					  // subcampo será inserido
	Filler* prox; // Referência para o próximo Filler, serve para encadeá-los como uma Lista
	int tamanhoMaximo; // Tamanho máximo dos Fillers, é constante
	
	int getTamanhoLivre(); // Função comentada no arquivo .cpp correspondente
	
public:
	Filler(); // Função comentada no arquivo .cpp correspondente
	Filler(int id); // Função comentada no arquivo .cpp correspondente
	Filler* adicionaCampo(string nomeCampo, int tamanho); // Função comentada no arquivo .cpp correspondente
	
	vector<SubCampo*> getSubCampos(); // Função comentada no arquivo .cpp correspondente
	int getTamanhoAtual(); // Função comentada no arquivo .cpp correspondente
	Filler* getProximo(); // Função comentada no arquivo .cpp correspondente
	
	string toString(); // Função comentada no arquivo .cpp correspondente
};

#endif