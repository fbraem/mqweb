angular.module('mqWebApp').provider('mqWebListener', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			// Pass an object for using the query parameters of inquire
			// or a listener name to use the URI path
			inquire : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/listener/inquire/' + qmgr, {
							cache : false,
							params : arguments[0]
						});
				}
				else {
					return $http.get('/listener/inquire/' + qmgr + '/' + arguments[0], { cache : false } );
				}
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
