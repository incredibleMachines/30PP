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
		
		_Database.queryCollection('clips',{render:true},function(e,queue){
			
			if(e) console.error(" Error: Query Collection Error | Clips ".inverse.red)
			else{
				_Database.getAll('scenes',function(_e,scenes){
					if(_e) console.error(" Error: Query Collection Error | Scenes ".inverse.red);
					else{
						//console.log(" Render Queue ".inverse.green)
						//console.log(queue)
						//console.log(" Scenes ".inverse.green)
						//console.log(scenes)
						
						//now that we have our event lets sort what needs to be rendered out
						sortRenderEvents(queue, scenes, function(results){
							//results to be rendered this contains an array of our scenes with clip objects which need to be rendered
							//if(bDEBUG) console.log("RESULTS")
							//if(bDEBUG) console.log(results);
							
							//if(bDEBUG) console.log("++++++++++++++++++++++++".inverse.cyan);
							results.forEach(function(result, i){
								if(bDEBUG) console.log("++++++++++++++++++++++++".inverse.yellow);
								console.log(result);
								result.clips.forEach(function(clip,j){
									if(bDEBUG) console.log("++++++++++++++++++++++++".inverse.red);
									console.log(clip)
									
									//figure out what the fuck needs to happen with this clip
									var obj = {};
									/*
									switch(parseInt(clip.layout)){
										
										case 0:
											//full immersion
											//MUST GET LAYER NAME FROM AE FILE TO ADD
											//TEMP TEMP TEMP TEMP
											obj.template 	= "FullImmersion.aep"
											obj.type 	 	= result.type
											obj.script   	= "FullImmersion.jsx"
											obj.output 	 	= result.slug+"_"+clip.slug+".mov"
											obj.data	 	= {};
											obj.data.Source_Image 	= clip.zones[0].file	
											obj.order 		= null
											obj.clips 		= [_Database.makeMongoID(clip._id)]
											obj.scene_id    = _Database.makeMongoID(result._id)
											_Database.add('renderqueue',obj,function(e){})	
											
										break;
										case 1:
											//single wall
											var text_type;
											
											obj.data = {}

											
											if(clip.zones[1].text){
												
												if(typeof clip.zones[1].text === 'object'){
													//multitext
													text_type = "Multiple"
													
													for(var k =0; k<clip.zones[1].text.length;k++){
														var key = "Source_Text_"+k
														obj.data[key] = clip.zones[1].text[k]
													}
													
												}else{
													//single text
													text_type = "Single"
													obj.data.Source_Text = clip.zones[1].text
													obj.data.Source_Image_Wall = clip.zones[1].file
												}
												
											}else{
												//if no text, we will use the same template as single
												//we will write an empty string into the appropriate text field
												text_type = "Single"
												obj.data.Source_Text = ""
												obj.data.Source_Image_Wall = clip.zones[1].file
												
											}
											var location_type;
											if(clip.zones[0].locations){
												
												if(clip.zones[0].locations.length> 1){
													location_type= "Multiple"
													for(var k =0; k<clip.zones[0].locations.length;k++){
														var key = "Source_Location_"+k
														obj.data[key] = clip.zones[0].locations[k]
													}
												}else{
													location_type= "Single"
													obj.data.Source_Location = clip.zones[0].locations[0]
												}
												
											}else{
												location_type = "None"
												obj.data.Source_Location = null
												obj.data.Source_Image_Sculpture = clip.zones[0].file
											}
											
											obj.template = "SingleWall_"+text_type+"_"+location_type+".aep"
											obj.type = result.type
											obj.script = "SingleWall_"+text_type+"_"+location_type+".jsx"
											obj.output = result.slug+"_"+clip.slug+".mov"
											obj.order = null
											obj.clips = [_Database.makeMongoID(clip._id)]
											obj.scene_id    = _Database.makeMongoID(result._id)
											
											_Database.add('renderqueue',obj,function(e){})
											
										break;
										case 2:
											//double wall
											obj.data = {}
											var location_type;
											if(clip.zones[0].locations){
												
												if(clip.zones[0].locations.length> 1){
													location_type= "Multiple"
													for(var k =0; k<clip.zones[0].locations.length;k++){
														var key = "Source_Location_"+k
														obj.data[key] = clip.zones[0].locations[k]
													}
												}else{
													location_type= "Single"
													obj.data.Source_Location = clip.zones[0].locations[0]
												}
												
											}else{
												location_type = "None"
												obj.data.Source_Location = null
												obj.data.Source_Image_Sculpture = clip.zones[0].file
											}
											//only iterate between our wall zones 
											//index 1 and 2 
											var text_type_arr=[];
											for(var z = 1; z < clip.zones.length; z++){
												
												var text_key = "Source_Text_"+z
												var img_key = "Source_Image_Wall_"+z

												if(clip.zones[z].text){
													
													if(typeof clip.zones[z].text === "object"){
														//multi text option
														text_type_arr.push("Multiple")
														
														for(var k = 0; k<clip.zones[z].text.length; k++){
															var key = text_key+"_"+k
															obj.data[key] = clip.zones[z].text[k]
														}
														
													}else{
														//single text 
														text_type_arr.push("Single")
														obj.data[text_key]= clip.zones[z].text
														obj.data[img_key] = clip.zones[z].file
														
													}
													
												}else{
													text_type_arr.push("Single")
													obj.data[text_key] = ""
													obj.data[img_key] = clip.zones[z].file
												}
												
											}
											
											var text_type = (_.contains(text_type_arr,"Multiple"))? 'Multiple' : 'Single'
											
											obj.template = "DoubleWall_"+text_type+"_"+location_type+".aep"
											obj.type = result.type
											obj.scene_id = _Database.makeMongoID(result._id)
											obj.script =  "DoubleWall_"+text_type+"_"+location_type+".jsx"
											obj.output = result.slug+"_"+clip.slug+".mov"
											obj.clips = [_Database.makeMongoID(clip._id)]
											
											_Database.add('renderqueue',obj,function(e){})
											
										break;
										
									}
									*/
									
									
								})

							})
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

function parseRenderQueue(result,cb){
	console.log(JSON.stringify(result))
	cb()
}

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
	
	
/*
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
*/
	
	
}