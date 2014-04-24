/* to display all files to be concatenated, run ffmpeg? */

var utils 	= require('../modules/Utils');
var _ 			= require('underscore');
var ffmpeg  = require('../modules/FFmpeg');
var async	 = require('async');

/******* AMBIENT EVENT *********/
//adding this to timeline manually.
var ambientEvent = {
	"title" : "Ambient",
	"slug" : "ambient",
	"concat_queue" : 0, //always at queue 0
	"duration" : 62000, //need to calculate this manually
	"start_time" : 0,
	"scenes" : [
		{
			"type" : "ambient",
			"title" : "Gradient",
			"slug" : "ambient_gradient",
			"concat_queue" : 0, //if another scene is added, will be '1'
			"start_time" : 0, //need to manually calc the start_time of the next ambient event
			"duration" : 62000
		}
	]
}
//****************************/

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
						if(evt.scenes.length === 0){/* check in case there are no scenes in this event */
							tEventsCounter++;
							//console.log("tEventsCounter: "+tEventsCounter);
							if(tEventsCounter === tEvents.length){ /* if there are AND we are at eventsCounter total */
								res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
									scenes:allScenes, error:null });/* lemme get that page render */
							}
						} else { /* we have scenes ! */
							evt.scenes.forEach(function(sce,i){
								_Database.getDocumentBySlug('timeline', sce.slug, function(e, scene){
									//console.log(scene);
									eScenesCounter++;
									allScenes.push(sce); /* push this scene into allScenes */
									if(eScenesCounter === evt.scenes.length){
										tEventsCounter++;
										//console.log("tEventsCounter: "+tEventsCounter);
									}
									if(tEventsCounter === tEvents.length){
										res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
										scenes:allScenes, error:null });
									}
								})
							})
						} //end else we have scenes
					})
				} else { /*tEvents.length <=0, need to make timeline. */
					res.render('renderqueue/timeline', { current:req.url, title:'Timeline', page_slug:'renderqueue-timeline',
					scenes:allScenes, error:null });
				}
			} else console.log("error on queryCollection timeline");
		}) //end queryCollectionWithOptions('timeline')
	}
}



/* function to generate the timeline collection in the database */
exports.make = function(_Database, EVENT_TYPES, cb){

	var timelineStartTime = 0; //for calculating all start times
	var timelineSceneOrder = 0; //for calculating global order number
	var timelineEventOrder = 0;

	var allTimelineEvents = new Array(); //holds ALL db event objects
	var allTimelineScenes = new Array(); //holds ALL db scene objects

	async.waterfall([

		function(callback){ //check if the timeline already exists
			_Database.queryCollection('timeline',{}, function(e, timelineContents){
				if(timelineContents.length<1){
					allTimelineEvents.push(ambientEvent); //INSERT AMBIENT HERE, AT THE VERY TOP
					var orderUpdate = ambientEvent.scenes.length;
					var startTimeUpdate = 0;
					for(var i=0; i<ambientEvent.scenes.length; i++){ //go through ambient event scenes
						startTimeUpdate += ambientEvent.scenes[i].duration; //update the real startTime
						console.log("this duration: ")
						console.log(ambientEvent.scenes[i].duration);
					}
					timelineSceneOrder += orderUpdate;
					timelineEventOrder += 1; //only 1 ambient Event ever.
					callback(null); //next fall.
				} else {
					console.log("timeline already exists. clear with mongo: db.timeline.remove() first");
					cb(null); //exit. done done.
				}
			});
		},

		function(callback){ //create timelineEvents

			async.eachSeries(EVENT_TYPES, function(EVT_TYPE, _cb){ //first function

				//console.log("event title: " + EVT_TYPE);
				var thisTimelineEvent 			 = {};
				thisTimelineEvent.title 		 = EVT_TYPE;
				thisTimelineEvent.slug 			= utils.makeSlug(EVT_TYPE);
				thisTimelineEvent.duration	 = 0; //durations are for scenes only now.
				thisTimelineEvent.scenes		 = new Array();
				allTimelineEvents.push(thisTimelineEvent);
				_cb(null);

			}, function (err){
				if(err) console.log("eachSeries error: "+err);
				else{
					 callback(null);
				}
			});
		},

		function(callback){ //get all scenes

			_Database.getAll('scenes', function(_e,scenes){

				if(_e) console.log("error querying for scenes");
				else{
					allTimelineScenes = scenes.slice(0);
					console.log("all scenes length: "+ allTimelineScenes.length);
					// console.log("===========================");
					// console.log(JSON.stringify(allTimelineScenes));
					// console.log("===========================");
					callback(null);
				}
			});
		},

		function(callback){ //query for all scenes in each event

			async.eachSeries(allTimelineEvents, function(evt, _cb){
				var theseEventScenes = new Array();
				theseEventScenes = _.where(allTimelineScenes, {type: evt.slug});

				var scenes = new Array();
				if(theseEventScenes.length>0){
					scenes = _.sortBy(theseEventScenes, function(scene){
						return scene.order; //sort by the order # of the SCENE COLLECTION OBJECT
					});
					// console.log("ordered scene object for this event: ");
					// console.log(JSON.stringify(scenes));
					// console.log("+++++++++++++++++++++++++++++++");
					async.eachSeries(scenes, function(scene, __cb){
						var thisDuration = 47000;
						if (scene.title === "Gastronomy") thisDuration = 55000; //gastronomically large!
						if (scene.title === "Leisure") thisDuration = 52000;
						var thisStartTime = timelineStartTime+parseInt(ambientEvent.duration);
						var thisConcatSlug = scene.type+"_"+utils.makeSlug(scene.title);
						var sceneObj = { //** everything that is being stored in the scene timeline objects
							scene_id: 		scene._id,
							type: 				scene.type,
							title: 	 		scene.title,
							slug: 				thisConcatSlug, //previously "concat_slug"
							concat_queue: timelineSceneOrder,
							start_time: 	thisStartTime,
							duration: 		thisDuration
						}
						timelineStartTime += thisDuration; //increment startTime.
						timelineSceneOrder += 1;  //increment the 'global' timeline SCENE order position
						// console.log("thisSCENE: ")
						// console.log(sceneObj);
						// console.log("-----------------------------")
						evt.scenes.push(sceneObj);
						evt.duration += thisDuration;

						__cb(null);
					}); //end async scenes
					evt.start_time = timelineStartTime+parseInt(ambientEvent.duration); //new
					evt.concat_queue = timelineEventOrder;
					timelineEventOrder += 1;
				} //end if scenes.length>0
				_cb(null);
			}, function(err){
				if(err) console.log("eachSeries error: "+err);
				else{
					// console.log("alltimelineEvents before last FALL: ");
					// console.log(JSON.stringify(allTimelineEvents));
					callback(null);
				}
			});
		},

		function(callback){ //go through each event, create timelineEvents
			allTimelineEvents.forEach(function(evt, i){
				_Database.add('timeline', evt, function(__e){
					if(!__e){
						console.log("added this timeline event: " + evt.title);
					}	//everything is great
					else console.log("error adding to timeline collection: "+ evt);
				})
			})
			callback(null);
		}
	], function (err, result){

		if(err) console.log("err on last timeline make FALL: "+err);
		else cb(null); //DONE DONE DONE
	});
}




/*function to call FFMPEG and concatenate all video files */
exports.concat = function(_Database, EVENT_TYPES, cb){
	return function(req,res){
		//res.jsonp({error: "Not Implemented Yet!"});
		ffmpeg.concat(_Database,function(e){
			if(e) console.log("concat error: "+ e);
			else res.jsonp({success:'concat success'});
		})
	}
}



/* function to update the timeline database with a new duration value for any scene */
exports.update = function(_Database){

	var allTimelineEvents = new Array();
	var allTimelineScenes = new Array();

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

		async.waterfall([
			function(callback){ //update the specific scene with new duration
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
										console.log("successfully updated SCENE new scene duration");
										//res.redirect('/timeline');
										callback(null);
									}else console.log("ERROR UPDATING DB WITH NEW DURATION");
								})
							}
						})//end scenes.forEach
					})//end events.forEach
				}) //end database.getAll('timline')
			},

			function(callback){ //get all events
				_Database.getAll('timeline',function(e, tEvents){
					if(e) console.log("error querying for scenes");
					else{
						allTimelineEvents = tEvents.slice(0);
						console.log("all events length: "+ allTimelineEvents.length);

						allTimelineEvents = _.sortBy(allTimelineEvents, function(evt){
							return evt.concat_queue; //sort by the order # of the SCENE COLLECTION OBJECT
						});

						callback(null);
					}
				});
			},

			function(callback){ //update EVENT durations (FUTURE:can be combined with following waterfall)
				var eventStartTime = 0;
				async.eachSeries(allTimelineEvents, function(evt, cb){
					var eventDuration = 0;
					evt.scenes.forEach(function(scene){
						eventDuration += scene.duration;
					})
					evt.duration = eventDuration;
					if(evt.scenes.length>0)evt.start_time = evt.scenes[0].start_time;
					cb(null);
				}, function(err){
					if(err)console.log("update error"+err);
					else callback(null);
				});
			},

			function(callback){ //update all start_times
				var eventStartTime = 0;
				var sceStartTime = 0;
				async.eachSeries(allTimelineEvents, function(evt, cb){
					evt.start_time = eventStartTime;
					//eventStartTime += parseInt(evt.duration);
					if(evt.scenes.length>0){
						async.eachSeries(evt.scenes, function(sce, _cb){
							sce.start_time = eventStartTime;
							eventStartTime += sce.duration;
							_cb(null);
						}, function(_err){
						if(_err)console.log("update error"+_err);
						else cb(null);
						});
					} else cb(null);
				}, function(err){
					if(err)console.log("update error"+err);
					else callback(null);
				});
			},

			function(callback){ //DELETE OLD TIMELINE OBJECT
				_Database.remove("timeline", {}, function(e){
					if(e)console.log("db remove error"+e);
					else{
							callback(null);
					}
				});
			},


			function(callback){ //ADD NEW TIMELINE BACK TO DB
				allTimelineEvents.forEach(function(evt, i){
					_Database.add('timeline', evt, function(__e){
						if(!__e){
							console.log("added this timeline event: " + evt.title);
						}	//everything is great
						else console.log("error adding to timeline collection: "+ evt);
					})
				});
				callback(null);
			}
		], function(err, result){
			if(err) console.log ("error at end of update waterfall: "+err);
			else res.redirect('/timeline');
		});
	}
}
