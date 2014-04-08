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

exports.index = function(_Database,EVENT_TYPES){
	return function(req,res){
		//res.jsonp({success:'success'})
		var structure = {};
		structure.sales = {play_all: '/api/play/sales', play_single:[]};
		structure.ambient = {play_all: '/api/play/ambient', play_single:[]};
		structure.commands = {pause:'/api/control/pause',resume:'/api/control/resume',end:'/api/control/end'}
		
		EVENT_TYPES.forEach(function(type,i){
			
			if(type !== "Default" && type !== "Ambient"){
			
				//check if type is Ambient or not
				//for now assumes that all content is sales
				var temp={}
				temp[type] = '/api/'+utils.makeSlug(type)
				structure.sales.play_single.push(temp[type])		
			}		
		})
		res.jsonp(structure)
	}
}



/* 
	Play Sales, Ambient ex:

	/api/play/ambient
	/api/play/sales  (-- or should this be default?)
	
	TODO: decide difference between sales and default, who gets added to what, etc.
*/
exports.sendEvents = function(_type,_Database,_Websocket){
	return function(req,res){
	
		console.log("sending event type: "+ _type);
		
		if(_type === "sales") _type = "default"; 
		//or should it remain "sales"? TODO: decide what is added together for sales duration
		//--> ambient is it's own event...
		
		_Database.getDocumentBySlug('timeline',_type, function(e, _event){
			if(_event){
				var socketCommand = {
					"command": "play",
					"event": {
						"title": _event.title,
						"duration": _event.duration,
						"start_time": _event.start_time	
					}
				};
				_Websocket.status(function(status){
					if(status ==true){
						_Websocket.socket(function(socket){
							socket.send(JSON.stringify(socketCommand))
							res.jsonp({success:{socketCommand: socketCommand}})
						})
					}else{
						res.jsonp({error:'PlayerApp Not Connected', socketCommand: socketCommand})
					}
				})
			} else {
				res.jsonp({error:'Event Cannot Be Found', request: _type});
			}
		})
	}	
}


/* 
	Play single event,  ex:

	/api/play/gastronomy
	/api/play/parks-and-leisure
	/api/play/default
*/
exports.sendSingle = function(_Database, _Websocket){

	return function(req,res){
		var slug = req.params.slug;
		//console.log("send single");
		//console.log("req: "+req);
		//console.log("slug: "+slug);		
		
		_Database.getDocumentBySlug('timeline',slug, function(e, _event){
			if(_event){
				var socketCommand = {
					"command": "play",
					"event": {
						"title": _event.title,
						"duration": _event.duration,
						"start_time": _event.start_time	
					}
				};
			
				console.log("socketCommand: "+JSON.stringify(socketCommand));
				
				_Websocket.status(function(status){
					if(status ==true){
						_Websocket.socket(function(socket){
							socket.send(JSON.stringify(socketCommand))
							res.jsonp({success: {socketCommand: socketCommand}});
						})
					}else{
						res.jsonp({error:'PlayerApp Not Connected', socketCommand: socketCommand})
					}
				})			
			} else {
				res.jsonp({error:'Event Not Found', requested: slug})
			}
		})
	}
}



/* Control Commands, ex:

	/api/control/pause
	/api/control/resume
	/api/control/end
*/
exports.control = function(_Websocket){
	return function(req,res){
		//get control from url parameter
		var control = req.params.ctrl
		
		if(control == 'pause'||control=='resume'||control=='end'){
			_Websocket.status(function(status){ //check socket status
				//console.log(status)
				if(status==true){ //if we're connected
					_Websocket.socket(function(socket){ //access the websocket async
						//console.log(socket)
						socket.send(JSON.stringify({command: control}))
						res.jsonp({success:'Sent Command '+control})
	
					})
				}else{
					res.jsonp({error:'PlayerApp Not Connected', command: control})
				}
			})
		}else{
			res.jsonp({error:'Command Not Recognized: '+control})
		}
	}
}