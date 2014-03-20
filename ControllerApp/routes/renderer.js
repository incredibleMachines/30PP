var utils = require('../modules/Utils');

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
exports.render = function(_Database, _AfterEffects, EVENT_TYPES, SCENE_TYPES){
	
	
	return function(req,res){
		res.jsonp({status:'Check Console for data, currently being implimented'})
		
		parseRenderQueue(_Database, function(result){
			//console.log("parseRenderQueue")
			//console.log(result)
			formatJSONForAE(result, EVENT_TYPES, SCENE_TYPES, function(formattedOutput){
				
				console.log(JSON.stringify(formattedOutput))
				
			})
			//organize data for each template
			//create the JSON and render out the 
			//console.log(JSON.stringify(result))
		})
	}
}
//Function to parse all items that need rendering
//This function relies on the sortRenderQueue Function to do the dirty work
function parseRenderQueue(_Database, cb){
	//get all scenes, clips and files
	//find all the clips that need to be rendered
	//determine the scenes that need to be rendered
	
	_Database.getAll('scenes',function(e,scenes){
		_Database.getAll('clips',function(_e,clips){
			_Database.getAll('files',function(__e,files){
				scenes = JSON.stringify(scenes)
				scenes = JSON.parse(scenes)
				clips = JSON.stringify(clips)
				clips = JSON.parse(clips)
				files = JSON.stringify(files)
				files = JSON.parse(files)
				sortRenderQueue(scenes,clips,files,function(result){
					cb(result)
				})			
				
			})
		})
	})

}

//A Function which takes DB Collections and organizes the appropriate scenes,clips and files

function sortRenderQueue(scenes,clips,files,cb){
	//make our queue find which clips need to be rendered
	
	
	async.waterfall([ function(callback){
		//console.log('here')
		//first create an object of our render queue by finding clips that are up for 
		//rendering and then locating the scenes which contain those clips
		var clip_queue = _.where(clips,{render:true})
		var scene_queue = [];
		var clip_queue_counter = 0;
		clip_queue.forEach(function(item,i){
		
			var checkScene = _.findWhere(scene_queue,{_id: item.scene_id});
			if(typeof checkScene === 'undefined'){
				var scene = _.findWhere(scenes,{_id:item.scene_id})
				scene_queue.push(scene)
			}
			
			clip_queue_counter++
			if(clip_queue_counter === clip_queue.length){
			//console.log(scene_queue)
			//if we have gone through all the items in our clip_queue lets bind our scene_queue with the correct clips
			//pass the scene_queue off to our next 
			callback(null,scene_queue)
			}
		})
		
	},function(scene_queue,callback){
		//console.log(scene_queue)
		//go through our scene queue and populate our clips and files
		var scene_counter = 0;
		scene_queue.forEach(function(scene,i){
			
			var clip_counter = 0;
			scene.clips.forEach(function(clip,j){
				//console.log(clip)
				scene_queue[i].clips[j] = _.findWhere(clips,{_id:clip})
				//check each zone to see if there is a file object, if there is populate it
				//console.log(scene_queue[i].clips[j])
				var zone_counter = 0;
				
				scene_queue[i].clips[j].zones.forEach(function(zone,k){
					//check each zone and bind the file information if that zone has a file contained
					zone_counter++;
					if(zone.file!==null){
						//console.log("HAS FILE".inverse.red)
						zone.file = _.findWhere(files,{_id:zone.file})
					}
					//if we are at the end of the zones for the clip
					if(zone_counter == scene_queue[i].clips[j].zones.length){
						
						clip_counter++
						//if we've gone through all the clips in a scene
						if(clip_counter === scene.clips.length){
							
							scene_counter++
							//if we've gone through all the scenes
							if(scene_counter === scene_queue.length){
								callback(null,scene_queue)
							}//end (scene_counter === scene_queue.length)
							
						}//end if(clip_counter === scene.clips.length)	
						
					} //end if(zone_counter == scene_queue[i].clips[j].zones.length)
					
				})//end scene_queue[i].clips[j].zones.forEach(function(zone,k){})
				
							
			})//end scene.clips.forEach(function(clip,j){})
			
		}) //end scene_queue.forEach(function(scene,i){})
		
	}],function(err,result){
		
		//console.log(JSON.stringify(result))
		cb(result)
	});
		
}

//function which takes formatted Scenes and outputs a JSON Array of 
//formattedScenes is a binding of the MongoDB Scene,Clip and File documents
//cb
function formatJSONForAE(formattedScenes,EVENT_TYPES,SCENE_TYPES,cb){
	
	/* Sample JSON for AE Object */
	/* Referenced From: https://docs.google.com/a/incrediblemachines.net/document/d/12J7cS7iemVJnBjicObgC-fdH1XKCbKJfY2_XMl1dwiA/edit
		Create one Render Object Per Scene
		{	type: default_parks
			template: default_parks.aep,
			script: default_parks.jsx,
			output: default_parks.mov,
			data: {
					source_seq_image_L_1:
					source_seq_image_R_1:
					source_seq_text_L_1:
					source_seq_text_R_1:
					source_multitext_text_1:
					source_multitext_text_2:
					source_location_1:
					source_location_2:
			},
			scene: mongoID
			clips:[mongoID]
		}
	*/
	//sort through all the scenes, by type 
	
	async.waterfall([function(cb){
		//sort all our scenes by event type
		var scenesByType = {};
		//console.log(formattedScenes)
		var typeCounter=0;
		EVENT_TYPES.forEach(function(event_type){
			var slug = utils.makeSlug(event_type)
			var temp = _.where(formattedScenes, {type: slug})
			typeCounter++
			if(temp.length>0){
				scenesByType[slug] = temp;
				//console.log(event_type)
				//console.log(slug)	
				//console.log(scenesByType[slug])
			}
			if(typeCounter==EVENT_TYPES.length){
				//order each of the scenes by order
				_.each(scenesByType,function(scene,slug){
					scenesByType[slug] = _.indexBy(scenesByType[slug], 'order');
				})
				
				console.log(scenesByType)
				
				cb(null,scenesByType)		
			}
			 
		})
		
		
	},function(scenesByType,callback){
		//create our JSON Objects for AE with all the items ordered together
		
		//default each scene and organize
		var holder = [];
		_.each(scenesByType,function(scenes,slug){
			var groupHolder = [];
			//sort out each scene by slug
			if(slug === 'default'){
					
				//console.log(scenesByType[slug])
				
				var length = scenesByType[slug].length;
				//console.log(length)
				
				 
				_.each(scenesByType[slug],function(scene,order){
					//assumes that _.each will iterate in the correct order
					
					var type = slug+'_'+scene.slug	
					var temp = _.findWhere(groupHolder, {type: type })
					
					if( typeof temp === 'undefined'){
						var newObj = {type: type}
						groupHolder.push(newObj)
						//currentGroup = temp;
					}
					
					var currentGroup = _.findWhere(groupHolder,{type:type})
		
					currentGroup.template = type+".aep"
					currentGroup.script = type+".jsx"
					
					_.each(scene.clips,function(clip,index){
						
						console.log(scene.slug)	
						console.log(order)		
						console.log(clip)
						
						switch(clip.zones.length){
							case 1:
								if(!currentGroup.hasOwnProperty('source_image_slr')) currentGroup.source_image_slr = []	
								
								currentGroup.source_image_slr.push(clip.zones[0].file.path)
								
							break;
							case 2:
							
								if(clip.zones[0].locations){
									
									if(!currentGroup.hasOwnProperty('source_location')) currentGroup.source_location = []
									currentGroup.source_location.push(clip.zones[0].locations)
									/*
									for(var i =0; i<clip.zones[0].locations.length;i++){
										currentGroup.source_location.push(clip.zones[0].locations[i])
									}*/
										
								}else{
									if(!currentGroup.hasOwnProperty('source_image_s')) currentGroup.source_image_s = []
									currentGroup.source_image_s.push(clip.zones[0].file.path)
								}
								if(clip.zones[1].file){
									if(!currentGroup.hasOwnProperty('source_image_lr')) currentGroup.source_image_lr = []
									currentGroup.source_image_lr.push(clip.zones[1].file.path)
								}
								if(clip.zones[1].text){
									if(typeof clip.zones[1].text === 'string'){
										if(!currentGroup.hasOwnProperty('source_multitext_text')) currentGroup.source_single_text = []
										currentGroup.source_single_text.push(clip.zones[1].text)
									}else{
										if(!currentGroup.hasOwnProperty('source_multitext_text')) currentGroup.source_multitext_text = []
										currentGroup.source_multitext_text.push(clip.zones[1].text)
										/*
										for(var i=0; i<clip.zones[1].text.length; i++){
											currentGroup.source_multitext_text.push(clip.zones[1].text[i])
										}
										*/
									}
									
								}
								
							
							break;
							case 3:
								if(clip.zones[0].locations){
									
									if(!currentGroup.hasOwnProperty('source_location')) currentGroup.source_location = []
									currentGroup.source_location.push(clip.zones[0].locations)
									/*
									for(var i =0; i<clip.zones[0].locations.length;i++){
										currentGroup.source_location.push(clip.zones[0].locations[i])
									}*/
										
								}else{
									if(!currentGroup.hasOwnProperty('source_image_s')) currentGroup.source_image_s = []
									currentGroup.source_image_s.push(clip.zones[0].file.path)
								}
								if(clip.zones[1].file){
									if(!currentGroup.hasOwnProperty('source_sequence_image_l')) currentGroup.source_sequence_image_l = []
									currentGroup.source_sequence_image_l.push(clip.zones[1].file.path)
								}
								if(clip.zones[1].text){
									if(typeof clip.zones[1].text === 'string'){
										if(!currentGroup.hasOwnProperty('source_sequence_text_l')) currentGroup.source_sequence_text_l = []
										currentGroup.source_sequence_text_l.push(clip.zones[1].text)
									}else{
										if(!currentGroup.hasOwnProperty('source_multitext_text_l')) currentGroup.source_multitext_text_l = []
										currentGroup.source_multitext_text_l.push(clip.zones[1].text)
										/*
										for(var i=0; i<clip.zones[1].text.length; i++){
											currentGroup.source_multitext_text.push(clip.zones[1].text[i])
										}
										*/
									}
									
								}
								if(clip.zones[2].file){
									if(!currentGroup.hasOwnProperty('source_sequence_image_r')) currentGroup.source_sequence_image_r = []
									currentGroup.source_sequence_image_r.push(clip.zones[2].file.path)
								}
								if(clip.zones[2].text){
									if(typeof clip.zones[2].text === 'string'){
										if(!currentGroup.hasOwnProperty('source_sequence_text_r')) currentGroup.source_sequence_text_r = []
										currentGroup.source_sequence_text_r.push(clip.zones[2].text)
									}else{
										if(!currentGroup.hasOwnProperty('source_multitext_text_r')) currentGroup.source_multitext_text_r = []
										currentGroup.source_multitext_text_r.push(clip.zones[2].text)
										/*
										for(var i=0; i<clip.zones[1].text.length; i++){
											currentGroup.source_multitext_text.push(clip.zones[1].text[i])
										}
										*/
									}
									
								}
							break;
						}
						
						
					})
							
				})
				
				//console.log(groupHolder)
				callback(null,groupHolder)	
			}else{
				//custom video type
			}
			
		})
		
		//for each other type custom organize
		//ignoring custom types for now
		
		
	}],function(err,result){
		
		cb(result)
	})

	
}


/*
function parseRenderQueue(result,cb){
	console.log(JSON.stringify(result))
	cb()
}
*/
/*
function sortRenderEvents(queue, scenes, cb){
	
	//use underscore to sort through the events and see which ones are in need of render
	var holder = [], 
	counter = 0
	
	queue = JSON.stringify(queue)
	queue = JSON.parse(queue)
	
	scenes = JSON.stringify(scenes)
	scenes = JSON.parse(scenes)
	
	var scenesHolder = _.clone(scenes)
	
	queue.forEach(function(item,i){
	
		scenesHolder.forEach(function(scene,j){
			
			if(scene._id === item.scene_id){
				scene.clips.forEach(function(clip,k){
					
					if(clip === item._id){
						
						scene.clips[k] = item
						
						counter++
						
						if(counter === queue.length) cb(scenesHolder)
						
					}
					
				})
				
			}
		})
		
		
	})
	
	

	queue.forEach(function(item,i){
		if(bDEBUG) console.log(item.scene_id.toString().green.inverse);
		
		var scene = _.findWhere(scenes,{_id:item.scene_id}) 
		//var checkScene = _.findWhere(holder,{_id: scene._id});
		
		scene.clips.forEach(function(_clip,j){

			if(_clip === item._id){
				if(bDEBUG) console.log(" CLIP Match ".inverse.red)
				scene.clips[j] = queue
				counter++;
				if(counter==queue.length){
					cb(scenes)
				
					//console.log(scenes)
				}
			}
			
		})
		
	})

	
	
}*/