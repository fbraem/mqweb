angular.module('mqWebApp').provider('mqWebQueue', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			// Pass an object for using the query parameters of inquire
			// or a name of a queue to set the URI path. 
			inquire : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/api/queue/inquire/' + qmgr, {
							cache : false,
							params : arguments[0]
						});
				}
				else {
					return $http.get('/api/queue/inquire/' + qmgr + '/' + arguments[0], { cache : false } );
				}
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
