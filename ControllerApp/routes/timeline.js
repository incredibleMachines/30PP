/* to display all files to be concatenated, run ffmpeg? */

var utils 	= require('../modules/Utils');
var _ 		= require('underscore');

exports.index = function(_Database){

	/*  TODO: MAKE THIS QUERY THE ACTUAL TIMELINE COLLECTION */
	
	return function(req,res){
	 	//retrieve all events
	 	
	 	//get request
	 	var type = req.query.type;
	 	
	 	if(typeof type === 'undefined' || type === "") type = 'default';
	 	console.log(utils.toTitleCase(type))
	 	var title = utils.reverseSlug(type)
	 	//query scenes by event type 
	 	
	 	_Database.queryCollectionWithOptions('files', {type: type}, {sort: 'order'}, function(e,_files){
			if(!e){ 
				_Database.getAll('clips',function(__e,_clips){
					if(!__e){
						_Database.getAll('scenes',function(___e,_scenes){
							if(!___e){
								var _queue = _.where(_clips,{render: true})
								console.log("_queue: "+_queue);
								res.render('renderqueue/timeline', { current:req.url, title:'Concat', page_slug:'renderqueue-timeline', 
								files:_files, clips:_clips, queue:_queue, scenes:_scenes, error:null });
							}else{
								res.render('renderqueue/timeline',{current: req.url, title: 'Concat', page_slug:'renderqueue-timeline', 
								file:_files,clips:_clips, scenes:null, queue:null, error:'Return Scenes Error' });	
							} 			
						})
					} else {
						res.render('renderqueue/timeline', { current:req.url, title:'Concat Error', page_slug:'clips-index error',
							files:null, clips:null, scenes:null, error:'Return Clips Error' });
					}
				})
					
			}else{ 
				res.render('renderqueue/timeline', { current:req.url, title:'Concat Error', page_slug:'files-index error',
					files:null, clips:null, error:'Return Files Error' });
			}
		})
	}
}


/* function to create and store fake data in timeline collection */
exports.make = function(_Database, EVENT_TYPES, cb){
	
	var e = null; //error for entire function
/* 	console.log("EVENT_TYPES: "+ EVENT_TYPES); */

	var startTime = 0; //this is for calculating starttimes!
	
	EVENT_TYPES.forEach(function(event, i){

		var thisTimelineEvent 		= {};
		thisTimelineEvent.title 	= event;
		thisTimelineEvent.slug 		= utils.makeSlug(event);
		thisTimelineEvent.duration	= 0;
		thisTimelineEvent.scenes	= new Array;
		
		_Database.queryCollection('scenes', {type:thisTimelineEvent.slug}, function(__e,scenes){
			if(!__e){
				if(scenes.length>0){
					scenes.forEach(function(scene, j){
						//console.log("found scene: "+scene.title+ "  type: "+scene.type);
						thisTimelineEvent.duration += 5001; //TODO: get scene's duration, add that instead
						thisTimelineEvent.scenes.push(scene);
					})
				}
				thisTimelineEvent.start_time = startTime; //TODO: add up all previous event durations
				thisTimelineEvent.concat_queue = i;
				startTime += parseInt(thisTimelineEvent.duration);
				
				_Database.add('timeline', thisTimelineEvent, function(_e){
					if(!_e){}	//everything is great
					else e = _e; //TODO: handle this better
				})
			} else {
				e = __e; //TODO: handle this better
				console.log("query scenes error");
			}
		})
	})
	
	if(!e) cb(null); //TODO: handle this better
	else cb(e);
}


