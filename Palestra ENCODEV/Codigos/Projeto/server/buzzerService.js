var packRequest = function(requestUrl){
	var url = requestUrl;

	var body = "";

	var on = url.query.on;

	body += '000';
	body += (on == '1')?('1'):('0');

	return body;
}

var unpackResponse = function(body){
	obj = {};

	var on = body[3];

	obj['on'] = on;

	return JSON.stringify(obj);
}

module.exports.packRequest = packRequest;
module.exports.unpackResponse = unpackResponse;