angular.module('mqWebApp').provider('mqWebMessage', {
	qmgr : '',
	
	queue : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			getQueue : function() {
				return queue;
			},
			// Browse a message
			// Pass an object for using the query parameters of browse
			// or a message id to use the URI path
			browse : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/api/message/browse/' + qmgr + '/' + queue, {
							cache : false,
							params : arguments[0]
						});
				}
				else {
					return $http.get('/api/message/browse/' + qmgr + '/' + queue + '/' + arguments[0], { cache : false } );
				}
			},
			
			// Dump a message
			// Pass the message id as argument
			dump : function() {
				return $http.get('/api/message/dump/' + qmgr + '/' + queue + '/' + arguments[0], { cache : false } );
			},
			
			// Browse event messages
			// Pass an object for using the query parameters of browse
			// or a message id to use the URI path
			events : function() {
				if ( typeof arguments[0] == 'object' ) {
					return $http.get('/api/message/event/' + qmgr + '/' + queue, {
						cache : false,
						params : arguments[0]
						});
				}
				else {
					return $http.get('/api/message/event/' + qmgr + '/' + queue + '/' + arguments[0], { cache : false });
				}
			}
		}
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	},

	setQueue : function(value) {
		queue = value;
	}

});
