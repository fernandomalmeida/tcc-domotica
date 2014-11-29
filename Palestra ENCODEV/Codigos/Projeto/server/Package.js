var HTTPRequest = {
	GET:0,
	POST:1,
	PUT:2,
	DELETE:3,
	fromString: function(method){
		var met;
		if(method == "GET"){
			met = HTTPRequest.GET;
		} else if(method == "POST"){
			met = HTTPRequest.POST;
		} else if(method == "PUT"){
			met = HTTPRequest.PUT;
		} else if(method == "DELETE"){
			met = HTTPRequest.DELETE;
		} else {
			met = -1;
		}
		return met;
	},
	toString: function(httpReq){
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

PACKAGE_SIZE_HEADER_PACKAGE = 5;
PACKAGE_SIZE_BODY_PACKAGE = 25;
PACKAGE_SIZE_TOTAL_PACKAGE = PACKAGE_SIZE_HEADER_PACKAGE + PACKAGE_SIZE_BODY_PACKAGE;

var Package = function(
		sender,
		receiver,
		id,
		isRequest,
		info,
		body
	){
	this.sender = sender || 0;
	this.receiver = receiver || 0;
	this.id = id || 0;
	this.isRequest = isRequest || 0;
	this.info = info || 0;
	this.body = body || "";

	this.toString = function(){
		var str = ""
		
		str += "sender: " + this.sender + "\n";
		str += "receiver: " + this.receiver + "\n";
		str += "id: " + this.id + "\n";
		str += "isRequest: " + this.isRequest + "\n";
		str += "info: " + this.info + "\n";
		str += "body: " + this.body + "\n";

		return str;
	};

	this.toByteArray = function(){
		var str = "";
		str += String.fromCharCode(this.sender);
		str += String.fromCharCode(this.receiver);
		str += String.fromCharCode(this.id);
		str += String.fromCharCode(this.isRequest);
		str += String.fromCharCode(this.info);
		str += this.body.substring(0, this.SIZE_TOTAL_PACKAGE);

		return str;
	}

	this.setFromByteArray = function(pByteArray){
		this.sender = parseInt(pByteArray.charCodeAt(0), 10);
		this.receiver = parseInt(pByteArray.charCodeAt(1), 10);
		this.id = parseInt(pByteArray.charCodeAt(2), 10);
		this.isRequest = parseInt(pByteArray.charCodeAt(3), 10);
		this.info = parseInt(pByteArray.charCodeAt(4), 10);
		this.body = pByteArray.substr(5);
	};

	this.toBuffer = function(){
		var buf = new Buffer(PACKAGE_SIZE_TOTAL_PACKAGE);

		var pByteArray = this.toByteArray();
		for(var i=0; i<pByteArray.length; i++){
			buf.writeUInt8(pByteArray.charCodeAt(i), i);
		}

		return buf;
	};

	return this;
}



module.exports.HTTPRequest = HTTPRequest;
module.exports.Package = Package;

module.exports.SIZE_HEADER_PACKAGE = PACKAGE_SIZE_HEADER_PACKAGE;
module.exports.SIZE_BODY_PACKAGE = PACKAGE_SIZE_BODY_PACKAGE;
module.exports.SIZE_TOTAL_PACKAGE = PACKAGE_SIZE_TOTAL_PACKAGE;
