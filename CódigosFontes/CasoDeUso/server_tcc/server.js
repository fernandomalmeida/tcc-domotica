// server.js
// 
// Script do servidor que roda no nó de fronteira.
// Ele trata as requisições HTTP recebidas pela porta 8080 compactando-as e enviando-as pela porta Serial
// As resposta recebidas pela porta serial são enviadas para os clientes que realizaram as requisições
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


var serialport = require("serialport");
var http = require("http");
var url = require("url");
var util = require("util");


var node1 = require("./node1_service.js");
var node2 = require("./node2_service.js");
var r = require("./request.js");

var HTTPRequest = r.HTTPRequest;
var Request = r.Request;

var SerialPort = serialport.SerialPort;

var portName = "/dev/ttyACM0";

var receivedData = "";
var receivedDataLength = 0;

var responseContainer = {};

var id = 0;

var tratarPacote = function(packetRequest){
	if(packetRequest.reqOrRes = 1){
		var index = String(packetRequest.addr2) + String(packetRequest.id);
		var res = responseContainer[index];
		
		var responseCode = 200 + 100*packetRequest.rInfo;
		
		if(res){
			res.writeHead(responseCode, {'Content-Type': 'application/json', 'Access-Control-Allow-Origin':'*'});
		
			console.log("Packet received");
			console.log(packetRequest.toString());
			switch(packetRequest.addr2){
				case 1:
					res.write(node1.descompactaResposta(packetRequest.msg));
					break;
				case 2:
					res.write(node2.descompactaResposta(packetRequest.msg));
					break;
			}
			res.write("\n");
			res.end();
		}
	}
}

var myPort = new SerialPort(portName, {
	baudrate: 57600,
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false
});

myPort.on("open", function() {
	console.log("Open serial communication");
	
	myPort.on("data", function(data){
		for(i=0; i<data.length; i++){
			receivedData += String.fromCharCode(data.readUInt8(i));
			receivedDataLength++;
			if(receivedDataLength >= 32){
				reqSerial = receivedData.substr(0, 31);
				var req = new Request();
				req.toRequest(reqSerial);
				receivedDataLength = 0;
				receivedData = "";
				
				tratarPacote(req);
			}
		
		}
	});
});

var req = http.createServer(function (request, response){
	console.log("HTTP Request received");
	
	var method = request.method;
	var url_parts = url.parse(request.url, true);
	
	var met = HTTPRequest.fromString(method);
	console.log(met);

	var addr1 = 0;
	console.log(url_parts.pathname);
	switch(url_parts.pathname){
		case "/node1":
			addr1 = 1;
			body = node1.compactaRequisicao(url_parts);
			break;
		case "/node2":
			addr1 = 2;
			body = node2.compactaRequisicao(url_parts);
			break;
	}

	var req = new Request(addr1, 0, id, 0, met, body);
	
	var index = String(addr1) + String(id);
		console.log("indexReq: "+String(index));
	
	id = id+1;
	console.log("Server");
	console.log(req.toString());
	
	var buf = req.toBuffer();
	
	myPort.write(buf);
	console.log("Send to mstr1");
	
	responseContainer[index] = response;
	
}).listen(8080);

req.on("error", function(err){
	console.log("Error:");
	console.log(err);
});

console.log("Starting httpCompactado.js");

