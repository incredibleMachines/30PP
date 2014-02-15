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
		//res.jsonp({success:'success'})
		var structure = {};
		structure.sales = {play_all: '/api/play/sales', play_single:[]};
		structure.ambient = {play_all: '/api/play/ambient', play_single:[]};
		structure.commands = {pause:'/api/control/pause',resume:'/api/control/resume',end:'/api/control/end'}
		_Database.queryCollection('events',{anim_type:'sales'},function(err,_sales){
			var sales_counter = 0;
			_sales.forEach(function(sale,i){
				var temp = {};
				temp[sale.title] = '/api/play/'+sale.slug;
				structure.sales.play_single.push(temp);
				sales_counter++;
				if(sales_counter == _sales.length){
					
					_Database.queryCollection('events',{anim_type:'ambient'},function(err,_ambients){
						
						var ambients_counter =0;
						_ambients.forEach(function(ambient,j){
							var tempA = {};
							tempA[ambient.title]='/api/play/'+ambient.slug;
							structure.ambient.play_single.push(tempA);
							ambients_counter++;
							if(ambients_counter==_ambients.length){
								
								res.jsonp(structure)
							}
							
						})	
						
					})
				}
			})
		})
	}
}
exports.sendEvents = function(_type,_Database,_Websocket){
	return function(req,res){
	console.log('sending event')
		_Database.queryCollection('events',{anim_type:_type},function(err,_data){
			var event_counter =0;
			_data.forEach(function(_event,i){
				_Database.formatScenes(_event._id,_event.scenes,function(_err,_scenes){
					_data[i].scenes = _scenes;
					event_counter++;
					if(event_counter == _data.length) {
						var socketCommand = {command: _type, events:_data}
						//currently responds on the frontend for debugging only. 
						//This should send the message to the websocket and then confirm success to api
						_Websocket.status(function(status){
							if(status ==true){
								_Websocket.socket(function(socket){
									socket.send(JSON.stringify(socketCommand))
									res.jsonp(socketCommand)

									//res.jsonp({success: 'Sent '+_type+' Event'})
								})
							}else{
								res.jsonp({error:'PlayerApp Not Connected'})	
							}
							
						})
					}
				})
			})
		})
	}	
}

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
						res.jsonp({success:'Sent Command: '+control})
	
					})
				}else{
					res.jsonp({error:'PlayerApp Not Connected'})
				}
			})
		}else{
			res.jsonp({error:'Command Not Recognized: '+control})
		}
		
	}
}


exports.sendSingle = function(_Database, _Websocket){
	return function(req,res){
		var slug = req.params.slug;
		_Database.getDocumentBySlug('events',slug,function(e,_event){
			
			if(!e){
				_Database.formatScenes(_event._id,_event.scenes,function(_e,_scenes){
					if(!_e){ 
						_event.scenes = _scenes;
						var socketCommand = {command: _event.anim_type+'-single', events:[_event]}
						//currently responds on the frontend for debugging only. 
						//This should send the message to the websocket and then confirm success to api
						_Websocket.status(function(status){
							if(status ==true){
								_Websocket.socket(function(socket){
									socket.send(JSON.stringify(socketCommand))
									res.jsonp(socketCommand)
									//res.jsonp({success: 'Sent '+_type+' Event'})
								})
							}else{
								res.jsonp({error:'PlayerApp Not Connected'})	
							}
							
						})
							
					}else{
						console.error("Format Scenes Error: "+_e)
					}
				})
			}else{
				console.error("Get Document By Slug Error: "+e)
			}
			
		})
		//res.jsonp({success: 'not implimented yet'})
	}
}