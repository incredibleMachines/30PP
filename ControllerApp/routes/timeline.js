/* to display all files to be concatenated, run ffmpeg? */

var utils 	= require('../modules/Utils');
var _ 			= require('underscore');
var ffmpeg  = require('../modules/FFmpeg');
var async	= require('async');

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
									allScenes.push(sce); /* push this scene into allScenes */
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





/* function to MAKE the timeline collection based on all scenes in the database */
exports.make = function(_Database, EVENT_TYPES, cb){

	var timelineStartTime = 0; //for calculating all start times
	var timelineSceneOrder = 0; //for calculating global order number
	var timelineEventOrder = 0;

	_Database.queryCollection('timeline',{}, function(e, timelineContents){

		if(timelineContents.length<1){ //nothing here, let's MAKE MAKE MAKE

			async.eachSeries(EVENT_TYPES, function(EVT_TYPE, _cb){ //first function

				//console.log("event title: " + EVT_TYPE);
				var thisTimelineEvent 		= {};
				thisTimelineEvent.title 	= EVT_TYPE;
				thisTimelineEvent.slug 		= utils.makeSlug(EVT_TYPE);
				thisTimelineEvent.duration	= 0; //durations are for scenes only now.
				thisTimelineEvent.scenes	= new Array;

				_Database.queryCollection('scenes', {type:thisTimelineEvent.slug}, function(_e,scenes){

					if(scenes.length>0){
						scenes = _.sortBy(scenes, function(scene){
							return scene.order; //sort by the order # of the SCENE COLLECTION OBJECT
						});

						async.eachSeries(scenes, function(scene, __cb){

							// console.log("scene number: "+scene.order);
							// console.log("scene type: "+scene.type);
							// console.log("found scene: "+scene.title+ "  type: "+scene.type);
							var thisDuration = 47000;
							if (scene.title === "Gastronomy") thisDuration = 55000; //gastronomically large!
							var thisStartTime = timelineStartTime;
							var thisConcatSlug = scene.type+"_"+utils.makeSlug(scene.title);
							var sceneObj = { //** everything that is being stored in the scene timeline objects
								scene_id: 		scene._id,
								type: 				scene.type,
								title: 	 		scene.title,
								//order: 	 	scene.order,
								concat_slug:  thisConcatSlug,
								concat_queue: timelineSceneOrder,
								start_time: 	thisStartTime,
								duration: 		thisDuration
							}
							timelineStartTime += thisDuration; //increment startTime.
							timelineSceneOrder += 1;  //increment the 'global' timeline SCENE order position
							thisTimelineEvent.scenes.push(sceneObj);
							thisTimelineEvent.duration += thisDuration;
							__cb(null);
						}); //end async scenes

						// thisTimelineEvent.start_time = startTime;
						thisTimelineEvent.concat_queue = timelineEventOrder;
						timelineEventOrder += 1;
						// startTime += parseInt(thisTimelineEvent.duration);

						_Database.add('timeline', thisTimelineEvent, function(__e){
							if(!__e){
								console.log("added this timeline event: " + thisTimelineEvent.title);
							}	//everything is great
							else console.log("error adding to timeline collection: "+ thisTimelineEvent);
						})
					} //end if scenes.length>0

				}); //end queryCollection('scenes')

				_cb(null);
			}); //end async EVENT_TYPES

		} //else timelineContents is > 0
		cb(null); //FINAL REAL CALLBACK
	}); //end queryCollection('timeline')
}





/*function to call FFMPEG and concatenate all video files */
exports.concat = function(_Database, EVENT_TYPES, cb){
	return function(req,res){
		res.jsonp({error: "Not Implemented Yet!"});
		// ffmpeg.concat(function(e){
		// 	if(e) console.log("concat error: "+ e);
		// 	else res.jsonp({success:'concat success'});
		// })
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
		console.log("req sceneId: "+sceneId);
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
						//console.log("FOUND SCENEID MATCH");
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


/* attempt to update all start_times after any scene's duration has been changed.
	for the future.
*/
exports.updateForFuture = function(_Database){

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

		var allConcatScenes = new Array();

		populateScenes(processAllTimelineScenes);

		function populateScenes (callback){
			console.log("hit populateScenes");
			_Database.getAll('timeline',function(e, tEvents){
				async.each(tEvents, function(evt, cb){
					if(evt.scenes.length>0){
						async.each(evt.scenes, function(sce, _cb){
							allConcatScenes.push(sce);
							_cb(null);
						});
					}
					cb(null);
				});
				callback();
			});
		}

		function processAllTimelineScenes() {
			allConcatScenes = _.sortBy(allConcatScenes, function(scene){
				return scene.concat_queue; //sort by the order # of the SCENE COLLECTION OBJECT
			});

			async.eachSeries(allConcatScenes, scene, function(e){
				if(scene.scene_id == sceneId){
					_Database.update('timeline', {_id : evt._id, "scenes.scene_id": scene.scene_id}, obj, function(e){
						if(!e){
							console.log("successfully updated timeline with new scene duration");
							res.redirect('/timeline');
						}else console.log("ERROR UPDATING DB WITH NEW DURATION");
					})
				}
			})
		}
	}//end function(req,res)
}
