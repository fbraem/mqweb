angular.module('mqWebApp').provider('mqWebQueue', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			inquire : function(args) {
				if ( typeof args == 'object' ) {
					return $http.get('/queue/inquire/' + qmgr, {
							cache : false,
							params : args
						});
				}
				else {
					return $http.get('/queue/inquire/' + qmgr + '/' + args, { cache : false } );
				}
			} 
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
