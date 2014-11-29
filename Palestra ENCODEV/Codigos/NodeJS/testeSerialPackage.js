var serialport = require("serialport")
var SerialPort = serialport.SerialPort;

var PackageModule = require("../Projeto/server/Package.js");
var HTTPRequest = PackageModule.HTTPRequest;
var Package = PackageModule.Package;


var serialPort = new SerialPort("/dev/ttyUSB0", {
	baudrate: 9600,
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false,
	parser: serialport.parsers.raw
	//parser: serialport.parsers.readline('\r\n')
});

var receivedData = "";
var receivedDataLength = 0;

serialPort.open(function(error){
	if(error){
		console.log('failed to open: '+error);
	} else {
		console.log('open');
		serialPort.on('data', function(data){
			for(i=0; i<data.length; i++){
				receivedData += String.fromCharCode(data.readUInt8(i));
				receivedDataLength++;
				//console.log('received: ' + receivedDataLength + ' expected: ' + PackageModule.SIZE_TOTAL_PACKAGE);

				if(receivedDataLength >= (PackageModule.SIZE_TOTAL_PACKAGE)){

					pByteArray = receivedData.substr(0, PackageModule.SIZE_TOTAL_PACKAGE-1);

					var p2 = new Package();

					p2.setFromByteArray(pByteArray);

					receivedData = "";
					receivedDataLength = 0;

					console.log('Package received: ' + p2.toString());

					p2.id += 1;

					//serialPort.write(p2.toByteArray() + '\r\n');
					serialPort.write(p2.toBuffer());
				}
			}
		});
	}
});
