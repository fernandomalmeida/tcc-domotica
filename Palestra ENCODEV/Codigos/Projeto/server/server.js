var http = require("http");
var url = require("url");
var util = require('util');

var serialport = require("serialport")
var SerialPort = serialport.SerialPort;

var PackageModule = require("./Package.js");
var HTTPRequest = PackageModule.HTTPRequest;
var Package = PackageModule.Package;


var receivedData = "";
var receivedDataLength = 0;

var led = require("./ledService.js");
var button = require("./buttonService.js");
var buzzer = require("./buzzerService.js");

var currentId = 0;

var responseContainer = {};


var serialPort = new SerialPort("/dev/ttyUSB0", {
	baudrate: 9600,
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false,
	parser: serialport.parsers.raw
	
});

serialPort.open(function(error){
	if(error){
		console.log('failed to open: '+error);
	} else {
		console.log('open');
		serialPort.on('data', function(data){
			for(i=0; i<data.length; i++){
				receivedData += String.fromCharCode(data.readUInt8(i));
				receivedDataLength++;

				if(receivedDataLength >= (PackageModule.SIZE_TOTAL_PACKAGE)){

					pByteArray = receivedData.substr(0, PackageModule.SIZE_TOTAL_PACKAGE-1);

					var p2 = new Package();

					p2.setFromByteArray(pByteArray);

					receivedData = '';
					receivedDataLength = 0;

					var resource = p2.body.substr(0, 2);
					var body = p2.body.split('\n')[0].substr(2);

					var res = '';

					switch(resource){
						case 'l1':
							res = led.unpackResponse(body);
							break;
						case 'l2':
							res = led.unpackResponse(body);
							break;
						case 'bt':
							res = button.unpackResponse(body);
							break;
						case 'bz':
							res = buzzer.unpackResponse(body);
							break;
					}

					//console.log(resource);

					//console.log('Package received: ' + p2.toString());

					var response = responseContainer[p2.id];

					if(response){
						response.writeHead(200, {	'Content-Type': 'application/json',
													'Access-Control-Allow-Origin':'*',
													'Access-Control-Allow-Headers':'Origin, X-Requested-With, Content-Type, Accept'
												});
						
						response.write(res);
						response.end();
					}

				}
			}
		});

	}
});

var server = http.createServer(function (request, response){
	var method = request.method;
	var url_parts = url.parse(request.url, true);

	//console.log(method);
	//console.log(url_parts);

	currentId += 1;

	var sender = 0;
	var receiver = 0;
	var id = currentId;
	var isRequest = 1;
	var info = HTTPRequest.fromString(method);
	var body = '';

	if(info < 0){
		response.writeHead(200, {	'Content-Type': 'application/json',
													'Access-Control-Allow-Origin':'*',
													'Access-Control-Allow-Headers':'Origin, X-Requested-With, Content-Type, Accept'
												});
		response.end();
		return;
	}

	switch(url_parts.pathname){
		case '/led1':
			body += 'l1';
			body += led.packRequest(url_parts);
			break;
		case '/led2':
			body += 'l2';
			body += led.packRequest(url_parts);
			break;
		case '/botao':
			body += 'bt';
			break;
		case '/buzzer':
			body += 'bz';
			body += buzzer.packRequest(url_parts);
			break;
	}

	var p = new Package(
			sender,
			receiver,
			id,
			isRequest,
			info,
			body
		);

	//console.log(p.toString());

	serialPort.write(p.toBuffer());

	responseContainer[id] = response;

});

server.listen(8080);

console.log("Iniciando o Servidor");