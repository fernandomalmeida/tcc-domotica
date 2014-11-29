var packRequest = function(requestUrl){
	var url = requestUrl;

	var body = "";

	return body;
}

var unpackResponse = function(body){
	obj = {};

	obj['value'] = body.substr(0, 4);

	console.log('body: ' + body);

	return JSON.stringify(obj);
}

module.exports.packRequest = packRequest;
module.exports.unpackResponse = unpackResponse;