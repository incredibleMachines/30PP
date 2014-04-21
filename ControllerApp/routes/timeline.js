/* to display all files to be concatenated, run ffmpeg? */

var utils 	= require('../modules/Utils');
var _ 		= require('underscore');

exports.index = function(_Database){

	return function(req,res){
		_Database.queryCollectionWithOptions('timeline', {}, {sort: 'concat_queue'}, function(e,tEvents){

			if(!e){
				var allScenes = new Array();
				var tEventsCounter = 0;

				if(tEvents.length>0){
					tEvents.forEach(function(evt, j){
						//console.log("evt.scenes.length: ");
						//console.log(evt.scenes.length);
						//console.log(evt.scenes)

						var eScenesCounter = 0;
						/* check in case there are no scenes in this event */
						if(evt.scenes.length === 0){
							tEventsCounter++;
							//console.log("tEventsCounter: "+tEventsCounter);
							if(tEventsCounter === tEvents.length){ /* if there are AND we are at eventsCounter total */
								/* lemme get that page render */
								res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
								scenes:allScenes, error:null });
							}
						} else {
							evt.scenes.forEach(function(sce,i){
								//console.log("scene_id: "+i);
							  //console.log(sce.scene_id);
								_Database.getDocumentByID('scenes', sce.scene_id, function(e, scene){
									//console.log(scene);
									scene.duration = sce.duration; //stick the duration from timeline scene obj into there
									eScenesCounter++;
									allScenes.push(scene); /* push this scene into allScenes */
									if(eScenesCounter === evt.scenes.length){
										tEventsCounter++;
										//console.log("tEventsCounter: "+tEventsCounter);
									}
									if(tEventsCounter === tEvents.length){
										//console.log("allScenes: ");
										//console.log(allScenes);
										// scenes = _.sortBy(scenes,function(scene){
										//
										// 	if(!file.last_edited) return new Date(file.created_at)
										// 	else return new Date(file.last_edited);
										// })

										res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
										scenes:allScenes, error:null });
									}
								})
							})
						}
					})
				} else {
					//tEvents.length <=0, need to make timeline.
					res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
					scenes:allScenes, error:null });
				}
			} else console.log("error on queryCollection timeline");
		}) //end queryCollectionWithOptions('timeline')
	}
}


/* function to update the timeline database with a new duration value for any scene */
exports.update = function(_Database){

	return function(req, res){
		//console.log("timeline.update req.body: ");
		//console.log(req.body);
		var post = req.body;
		var newDuration = parseInt(post.duration);
		var sceneId	= post.scene_id;

		if(isNaN(newDuration)){
			console.log("NAN DETECTED!  Setting duration to 0.");
			newDuration = 0;
		}

		var obj = {
			$set: {
				"scenes.$.duration": newDuration
			}
		}

		/*step into every timeline object*/
		_Database.getAll('timeline',function(e, tEvents){
			//console.log("num tEvents: "+tEvents.length);
			tEvents.forEach(function(evt, i){
				//console.log("num evt.scenes: "+evt.scenes.length);
				evt.scenes.forEach(function(sce,j){
					//console.log("this sce.scene_id= "+sce.scene_id);
					if(sce.scene_id == sceneId){
						// _Database.update('timeline',{scene_id: _Database.makeMongoID(sceneId)},obj,function(e){
						_Database.update('timeline', {_id : evt._id, "scenes.scene_id": sce.scene_id}, obj, function(e){
								if(!e){
									console.log("successfully updated timeline with new scene duration");
									res.redirect('/timeline');
								}else console.log("ERROR UPDATING DB WITH NEW DURATION");
						})
					}
				})
			})
		})
	}
}



/* function to create and populate 'timeline' collection */
exports.make = function(_Database, EVENT_TYPES, cb){

	//return function(req, res){
		var e = null; //error for entire function
	/* console.log("EVENT_TYPES: "+ EVENT_TYPES); */

		var startTime = 0; //this is for calculating starttimes!

		//first wipe the timeline collection:
		// _Database.remove('timeline',{},function(e){
		// 	if(!e) console.log("remove timeline success");
		// 	else console.log("error removing timeline");
		// })

		_Database.queryCollection('timeline',{}, function(_e, anything){
			if(!e){
				console.log(anything);
				if(anything.length<1){ //check to make sure we don't already have a timeline!

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
										//thisTimelineEvent.duration += 5001; //DEPRECATED: now only scenes have durations.
										//scene.duration = 5101;//Math.floor(20000 + Math.floor((Math.random()*10000)+1));
										var thisDuration = 30001;
										var sceneObj = {
											scene_id: scene._id,
											order: scene.order,
											duration: thisDuration
										}
										thisTimelineEvent.scenes.push(sceneObj);
										thisTimelineEvent.duration += scene.duration;
									})
								}
								thisTimelineEvent.start_time = startTime;
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

				} else {
					//res.redirect('/timeline');
					console.log("DID NOT MAKE - TIMELINE CURRENTLY EXISTS");
					cb("did not MAKE because timeline already exists.   mongo db.timeline.remove() before re-make");
				}
			} else {
				console.log("ERROR QUERYING FOR TIMELINE...");
			}
		})
	//}
}

/*function to call FFMPEG and concatenate all video files */
exports.concat = function(_Database, EVENT_TYPES, cb){
	return function(req,res){
		res.jsonp({error: "Not Implemented Yet!"});
	}
}
