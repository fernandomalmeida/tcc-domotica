var serialport = require("serialport")
var SerialPort = serialport.SerialPort;

var serialPort = new SerialPort("/dev/ttyUSB0", {
	baudrate: 9600,
	dataBits: 8,
	parity: "none",
	stopBits: 1,
	flowControl: false,
});

serialPort.open(function(error){
	if(error){
		console.log('failed to open: '+error);
	} else {
		console.log('open');
		serialPort.on('data', function(data){
			console.log('data received: '+data);
		});
	}
});
