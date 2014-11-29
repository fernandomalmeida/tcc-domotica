var http = require("http");
var url = require("url");

var server = http.createServer(function (request, response){
	var method = request.method;
	var url_parts = url.parse(request.url, true);

	console.log(method);
	console.log(url_parts);

	response.write(method + '\n' + url_parts);
	response.end();
});

server.listen(8080);

console.log("Iniciando o Servidor");