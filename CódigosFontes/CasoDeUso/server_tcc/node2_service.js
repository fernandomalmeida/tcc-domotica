// node2_service.js
// 
// Esse arquivo fornece as funções de compactação e descompactação do serviço fornecido pelo node2.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


var compactaRequisicao = function(requestUrl){
	var url = requestUrl;

	var body = "";
	var auto = url.query.auto || 0;


	body += String.fromCharCode(auto);

	return body;
}

var descompactaResposta = function(body){
	obj = {};

	obj.auto = parseInt(body.charCodeAt(0), 10);

	return JSON.stringify(obj);
}

module.exports.compactaRequisicao = compactaRequisicao;
module.exports.descompactaResposta = descompactaResposta;