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
