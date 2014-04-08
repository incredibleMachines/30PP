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
		/*_Database.queryCollection('events',{anim_type:'sales'},function(err,_sales){
			var sales_counter = 0;
			//console.log(_sales);
			//if we have sales content continue
			if(_sales.length>0){
				_sales.forEach(function(sale,i){
					var temp = {};
					temp[sale.title] = '/api/play/'+sale.slug;
					structure.sales.play_single.push(temp);
					sales_counter++;
					if(sales_counter == _sales.length){
						
						_Database.queryCollection('events',{anim_type:'ambient'},function(err,_ambients){
							
							var ambients_counter =0;
							if(_ambients.length > 0){
								_ambients.forEach(function(ambient,j){
									var tempA = {};
									tempA[ambient.title]='/api/play/'+ambient.slug;
									structure.ambient.play_single.push(tempA);
									ambients_counter++;
									if(ambients_counter==_ambients.length){
										
										res.jsonp(structure)
									}
									
								})	
							}else{
								res.jsonp(structure)
							}
						})
					}
				})
			}else{ //we have no sales content - check for ambient content
				_Database.queryCollection('events',{anim_type:'ambient'},function(err,_ambients){
							
					var ambients_counter =0;
					if(_ambients.length > 0){
						_ambients.forEach(function(ambient,j){
							var tempA = {};
							tempA[ambient.title]='/api/play/'+ambient.slug;
							structure.ambient.play_single.push(tempA);
							ambients_counter++;
							if(ambients_counter==_ambients.length){
								
								res.jsonp(structure)
							}
							
						})	
					}else{
						res.jsonp(structure)
					}
				})
			}
		})*/
	}
}
exports.sendEvents = function(_type,_Database,_Websocket){
	return function(req,res){
	console.log('sending event')
	_Websocket.status(function(status){
							if(status ==true){
								_Websocket.socket(function(socket){
									socket.send(JSON.stringify(socketCommand))
									res.jsonp(socketCommand)

									//res.jsonp({success: 'Sent '+_type+' Event'})
								})
							}
						})
	}	
}

/*
Control Commands:

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




/* play single event --
	ex:
	/api/play/gastronomy
	/api/play/parks-and-leisure
	/api/play/default

*/
exports.sendSingle = function(_Database, _Websocket){

	return function(req,res){
		console.log("send single");
		console.log("req: "+req);
		var slug = req.params.slug;
		console.log("slug: "+slug);		
		
		_Database.getDocumentBySlug('timeline',slug, function(e, _event){
		
			var socketCommand = 
			{
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
		})
	}
}