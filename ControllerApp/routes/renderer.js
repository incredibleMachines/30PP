//use underscore here to help us facilitate actions
var _ = require('underscore');

//require the async lib for us if necessary
var async = require('async');

var bDEBUG = true;

exports.index = function(_Database){
	
	return function(req,res){
		_Database.getAll('clips',function(e,_clips){
			//console.log(data)
			if(e){ res.jsonp({"Error": "Query Collection Error: Scenes"})
			}else{
				_Database.getAll('scenes',function(_e, _scenes){
					if(_e) res.jsonp({"Error": "Query Collection Error: Events"})
				
					else{ 	var _queue = _.where(_clips,{render: true})
							res.render('renderqueue/index', { current: req.url, title: 'Render Queue', page_slug: 'renderqueue-index', queue: _queue, clips: _clips, scenes: _scenes } );	
						}
				})
			}
		})
		
	}
}

//a function to facilitate the render process gathers all data and executes
exports.render = function(_Database, _AfterEffects){
	
	
	return function(req,res){
		res.jsonp({status:'Check Console for data, currently being implimented'})
		
		//create the order for our render objects and figure out what we need to do. 
		//will likely need to spawn off child objects and resave data.
		//file handling and naming protocol 
		//object structure and order
		
		_Database.queryCollection('scenes',{render:true},function(e,queue){
			
			if(e) console.error(" Error: Query Collection Error | Scenes ".inverse.red)
			else{
				_Database.getAll('events',function(_e,events){
					if(_e) console.error(" Error: Query Collection Error | Events ".inverse.red);
					else{
						console.log(" Render Queue ".inverse.green)
						console.log(queue)
						console.log(" Events ".inverse.green)
						console.log(events)
						
						//now that we have our event lets sort what needs to be rendered out
						sortRenderEvents(queue, events, function(results){
							//results to be rendered
							//console.log(results);
							
							//bind all the scenes and events we need, check each scene to find a match of render content							
							//sort out of all the scenes that are actually rendered together, and all the scenes that are rendered seperately
							//create json packets for each template that needs to be rendered - 
							//create/reference a skeleton structure of the event we are rendering based off scene [] in event object
							//create skeleton of events/scenes which reflects merged scenes, to be rendered scenes and prerendered scenes 
							//organize these items by the movies we have made
							//place everything in the correct order make temp file and concatinate file
							//render each scene, update db accordingly 
							
							
						})
						
					}
					
				})
				
			}
			
		})
		
		
	}
	
}




function sortRenderEvents(queue, events, cb){
	
	//use underscore to sort through the events and see which ones are in need of render
	var holder = [], 
	counter = 0;
	
	queue = JSON.stringify(queue)
	queue = JSON.parse(queue)
	
	events = JSON.stringify(events)
	events = JSON.parse(events)
	
	queue.forEach(function(item,i){
		if(bDEBUG) console.log(item.event_id.toString().green.inverse);
		
		var event = _.findWhere(events,{_id:item.event_id}) 
		var checkEvent = _.findWhere(holder,{_id: event._id});
		
		if( typeof checkEvent === 'undefined'){
			if(bDEBUG) console.log(' NO EVENT EXISTS IN QUEUE YET '.inverse.yellow)
			//push back to create more viable queue object, this will allow us to bind our scenes and assets
			
			holder.push(event) 
		}else{
			if(bDEBUG) console.log(' EVENT EXISTS IN RENDER QUEUE '.inverse.cyan);
		}

		counter++;
		if(counter == queue.length) cb(holder)
	})
	
	
}