// node1_service.js
// 
// Esse arquivo fornece as funções de compactação e descompactação do serviço fornecido pelo node1.
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


var compactaRequisicao = function(requestUrl){
	var url = requestUrl;

	var body = "";
	var r = url.query.r || 0;
	var g = url.query.g || 0;
	var b = url.query.b || 0;

	r = r%256;
	g = g%256;
	b = b%256;

	body += String.fromCharCode(r);
	body += String.fromCharCode(g);
	body += String.fromCharCode(b);

	return body;
}

var descompactaResposta = function(body){
	obj = {};

	obj.r = parseInt(body.charCodeAt(0), 10);
	obj.g = parseInt(body.charCodeAt(1), 10);
	obj.b = parseInt(body.charCodeAt(2), 10);

	return JSON.stringify(obj);
}

module.exports.compactaRequisicao = compactaRequisicao;
module.exports.descompactaResposta = descompactaResposta;