//= require foundation
$(document).foundation();

var app = angular.module("app", []);

app.controller("controleArduino", ['$scope', '$http', '$interval', function($scope, $http, $interval){
	$scope.led1 = false;
	$scope.led2 = false;
	$scope.botao = '75%';
	$scope.buzzer = false;

	$scope.change = function(element){
		//alert(element);
		if(element == 'led1'){
			var url = ('http://localhost:8080/led1?on='+(($scope.led1)?('1'):('0')));
			$http.post(url, {});
		} else if(element == 'led2'){
			var url = ('http://localhost:8080/led2?on='+(($scope.led2)?('1'):('0')));
			$http.post(url, {});
		} else if(element == 'buzzer'){
			var url = ('http://localhost:8080/buzzer?on='+(($scope.buzzer)?('1'):('0')));
			$http.post(url, {});
		}
	}

	var update = function(){
		var url = 'http://localhost:8080/';
		$http.get(url+'botao').success(function(data, status, headers, config){
			$scope.botao = Math.round((100*data.value)/1024).toString() + '%';
		});
	}

	update();

	$interval(function(){
		//alert('teste');
		update();
	}, 400);
}]);
