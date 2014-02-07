angular.module('mqWebApp').provider('mqWebChannelStatus', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			// Pass an object for using the query parameters of inquire
			// or a channel name
			inquire : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/chstatus/inquire/' + qmgr, {
							cache : false,
							params : arguments[0]
						});
				}
				else {
					if ( arguments.length == 0 ) {
						return $http.get('/chstatus/inquire/' + qmgr, { cache : false } );
					}
					else if ( arguments.length > 0 ) {
						return $http.get('/chstatus/inquire/' + qmgr + '/' + arguments[0], { cache : false } );
					}
				}
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
