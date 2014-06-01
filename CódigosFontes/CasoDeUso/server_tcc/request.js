// request.js
// 
// Esse arquivo fornece a classe básica de Requisição, semelhante
// à escrita em C, porém adaptada para o JavaScript
//
// Fernando Mendonça de Almeida, Agosto de 2013
//


var HTTPRequest = {
	GET:0,
	POST:1,
	PUT:2,
	DELETE:3,
	fromString:function(method){
		var met;
		if(method == "GET"){
			met = HTTPRequest.GET;
		} else if(method == "PUT"){
			met = HTTPRequest.PUT;
		} else if(method == "POST"){
			met = HTTPRequest.POST;
		} else if(method == "DELETE"){
			met = HTTPRequest.DELETE;
		} else {
			met = -1;
		}
		return met;
	},
	toString:function(httpReq){
		switch(httpReq){
			case HTTPRequest.GET:
				return "GET";
				break;
			case HTTPRequest.POST:
				return "POST";
				break;
			case HTTPRequest.PUT:
				return "PUT";
				break;
			case HTTPRequest.DELETE:
				return "DELETE";
				break;
		}
	}
};

var Request = function(addr1, addr2, id, reqOrRes, rInfo, msg){
	var req = {};
	req.addr1 = addr1 || 0;
	req.addr2 = addr2 || 0;
	req.id = id || 0;
	req.reqOrRes = reqOrRes || 0;
	req.rInfo = rInfo || 0;
	req.msg = msg || "";
	
	req.toString = function(){
		var str = "";
		str += "addr1: " + req.addr1 + "\n";
		str += "addr2: " + req.addr2 + "\n";
		str += "id: " + req.id + "\n";
		str += "reqOrRes: " + req.reqOrRes + "\n";
		str += "rInfo: " + req.rInfo + "\n";
		str += "msg: " + req.msg + "\n";
		return str;
	};
	
	req.toSerialize = function(){
		var str = "";
		var auxInt;
		
		auxInt = ((req.addr1));
		str += String.fromCharCode(auxInt);
		
		auxInt = ((req.addr2));
		str += String.fromCharCode(auxInt);
		
		auxInt = ((req.id << 4) & 0xf0) + ((req.rOr << 3) & 0x8) + ((req.rInfo) & 0x7);
		str += String.fromCharCode(auxInt);
		
		str += (req.msg).substring(0, 29);
		
		return str;
	};
	
	req.toRequest = function(reqSerial){
		req.addr1 = parseInt(((reqSerial.charCodeAt(0))), 10);
		req.addr2 = parseInt(((reqSerial.charCodeAt(1))), 10);
		req.id = parseInt(((reqSerial.charCodeAt(2) >> 4) & 0xf), 10);
		req.rOr = parseInt(((reqSerial.charCodeAt(2) >> 3) & 0x1), 10);
		req.rInfo = parseInt(((reqSerial.charCodeAt(2)) & 0x7), 10);
		
		req.msg = reqSerial.substr(3);
		
	};
	
	req.toBuffer = function(){
		var buf = new Buffer(32);
		
		var reqSerial = req.toSerialize();
		for(var i=0; i<reqSerial.length; i++){
			buf.writeUInt8(reqSerial.charCodeAt(i), i);
		}
		
		return buf;
	};
	
	return req;
}

module.exports.HTTPRequest = HTTPRequest;
module.exports.Request = Request;