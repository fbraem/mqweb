angular.module('mqWebApp').provider('mqWebQueueManager', {
	qmgr : '',
	
	$get : ['$http', function($http) {
		return {
			getQueueManager : function() {
				return qmgr;
			},
			inquire : function() {
					return $http.get('/api/qmgr/inquire/' + qmgr, {
							cache : false
						});
				}
			} 
	}],
	
	setQueueManager : function(value) {
		qmgr = value;
	}

});
