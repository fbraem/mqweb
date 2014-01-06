var mqWebApp = angular.module('mqWebApp',[]);

mqWebApp.directive('mqConfig', ['MQWEB_CONFIG', function(config) {
	return {
		restrict: 'E',
		replace: true,
		link : function(scope, element, attrs) 
		{ 
			if ( attrs['name'] )
			{
				element.text(config[attrs['name']]);
			}
		}
	};
}]);

mqWebApp.directive('mqTemplate',
		['MQWEB_CONFIG',
			function (config) {
				return {
				 restrict: 'E',
				 templateUrl: '/static/html/' + config['html']
				};
}]);

mqWebApp.filter('default', function () {   
	return function (input, value) { 
		if (input !== null && input !== undefined && (input !== '' || angular.isNumber(input))) { 
			return input; 
		}     
		return value || '';   
	}; 
});
