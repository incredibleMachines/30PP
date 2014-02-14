var utils = require('../modules/Utils');


/* 
	{ 	sales: { play_all: '/api/sales',
				 play_single: [ {Event_Title: '/api/slug'}, 
				 				{Event_Title:'/api/slug'} 
				 			  ] 
				},
		ambient:{ play_all: '/api/ambient',
				  play_single: [ {Event_Title: '/api/slug'}, 
				 				{Event_Title:'/api/slug'} 
				 			  ]
				}
		commands: { pause: '/api/pause',
					end: '/api/end',
					resume: '/api/resume'			
		}
	}
	
	
*/

exports.index = function(_Database){
	return function(req,res){
		res.jsonp({success:'success'})	
	}
}

exports.pause = function(_Websocket){
	return function(req,res){
		if(_Websocket.status){
			_Websocket.socket.send({command:'pause'})
			res.jsonp({success:'Sent Pause'})
		}else{
			res.jsonp({failure: 'PlayerApp Not Connected'})
		}
	}
}
exports.resume = function(_Websocket){
	return function(req,res){
		if(_Websocket.status){
			_Websocket.socket.send({command:'resume'})
			res.jsonp({success:'Sent Resume'})
		}else{
			res.jsonp({failure:'PlayerApp Not Connected'})
		}
	}
}

exports.end = function(_Websocket){
	return function(req,res){
		if(_Websocket.status){
			_Websocket.socket.send({command:'end'})
			res.jsonp({success:'Sent End'})
		}else{
			res.jsonp({failure:'PlayerApp Not Connected'})
		}
	}
}


exports.sendSlug = function(_Database, _Websocket){
	return function(req,res){
		res.jsonp({success: 'not implimented yet'})
	}
}