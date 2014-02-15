angular.module('mqWebApp').provider('mqWebListenerStatus', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			// Pass an object for using the query parameters of inquire
			// or a listener name
			inquire : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/lsstatus/inquire/' + qmgr, {
							cache : false,
							params : arguments[0]
						});
				}
				else {
					if ( arguments.length == 0 ) {
						return $http.get('/lsstatus/inquire/' + qmgr, { cache : false } );
					}
					else if ( arguments.length > 0 ) {
						return $http.get('/lsstatus/inquire/' + qmgr + '/' + arguments[0], { cache : false } );
					}
				}
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
