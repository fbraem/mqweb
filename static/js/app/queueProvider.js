angular.module('mqWebApp').provider('queue', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			inquire : function(name) {
				return $http.get('/queue/inquire/' + qmgr,
					{	cache : false,
						params: {
							queueName: name
						}
					})
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}
	
});
