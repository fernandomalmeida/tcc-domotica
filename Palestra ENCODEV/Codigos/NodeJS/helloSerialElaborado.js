var serialport = require("serialport")
var SerialPort = serialport.SerialPort;

var serialPort = new SerialPort("/dev/ttyUSB0", {
	baudrate: 9600,
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false,
	parser: serialport.parsers.readline('\r\n')
});

serialPort.open(function(error){
	if(error){
		console.log('failed to open: '+error);
	} else {
		console.log('open');
		serialPort.on('data', function(data){
			console.log('data received: '+data);
			if(data == 'Ping'){
				serialPort.write('Pong'+'\r\n');
			} else {
				serialPort.write('Ping'+'\r\n');
			}
		});
	}
});
