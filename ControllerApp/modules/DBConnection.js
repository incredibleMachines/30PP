//Overarching Summary

var MongoClient = require('mongodb').MongoClient,
	Server = require('mongodb').Server,
	mongo = new MongoClient(new Server('localhost', 27017)),
	BSON = require('mongodb').BSONPure,
	colors = require('colors');
	
var database,
	collection = {};


//make a class of Mongo connect 
//ghetto constructor
exports.MongoConnect = function(){
	
	mongo.open(function(err,mongo){
		//connected to database
		console.log('connected to db'.grey);
		database = mongo.db('30_PP');
		//setup collection connections

		//collection.assets    = database.collection('assets');
		//collection.events    = database.collection('events');
		collection.clips	 = database.collection('clips');
		collection.users     = database.collection('users');
		collection.scenes    = database.collection('scenes');
		collection.files 	 = database.collection('files');
		//collection.locations = database.collection('locations');
		collection.renderqueue = database.collection('renderqueue')
		
	})
}

//_doc = mongo document to add
//_type = collection name
//_cb = callback(err,db_document{}) 

//returns mongo Document inserted
exports.add= function(_type,_doc,_cb){

	collection[_type].insert(_doc,function(e){
		
		if(!e) _cb(null,_doc);
		else _cb(e);
	})
}

//returns all the results within a collection
//_type = collection name
//_cb = callback(err,collection[])
//returns mongodb Collection as an Array
exports.getAll=function(_type,_cb){

	collection[_type].find().toArray(function(e,_data){
	
		if(!e) _cb(null,_data);
		else _cb(e);
	})
}
exports.queryCollection=function(_type,_query,_cb){
	queryCollection(_type,_query,_cb);
}

//_type = collection name
//_query = your formatted mongodb query
//_cb = callback(err,collection[])
//returns mongodb Collection as an Array
function queryCollection(_type, _query, _cb){
	
	collection[_type].find(_query).toArray(function(e,_data){
	
		if(!e) _cb(null,_data);
		else _cb(e);
	})

	
}
exports.queryCollectionWithOptions = function(_type,_query,_options,_cb){
	queryCollectionWithOptions(_type, _query, _options, _cb)
}
//_type = collection name
//_query = your formatted mongodb query
//_cb = callback(err,collection[])
//returns mongodb Collection as an Array
function queryCollectionWithOptions(_type, _query, _options, _cb){
	
	collection[_type].find(_query,_options).toArray(function(e,_data){
	
		if(!e) _cb(null,_data);
		else _cb(e);
	})

	
}

//remove a document
//_type = collection name
//_what = collection query 
//_cb = callback(e)
exports.remove = function(_type,_what,_cb){
	
	collection[_type].remove(_what,function(e){
		if(!e) _cb(null)
		else _cb(e)
	})
}

//update a document 
//_type = collection name
//_what = collection query
//_updateObj = the update operation which needs to take place
exports.update=function(_type,_what,_updateObj,_cb){
	
	collection[_type].update(_what,_updateObj,true,function(e){
		//needs to be tested and finished
		if(!e) _cb(null);
		else _cb(e)
	});
}

//update a document by providing a mongodb ID string
//_type = collection name
//_id = string as mongo id
//_updateObj = the update operation which needs to take place
//_cb = callback(err)
exports.updateByID=function(_type,_id,_updateObj,_cb){
	
	//convert _id to MongoObject
	//var o_id = new BSON.ObjectID(_id.toString());
	
	collection[_type].update({_id: makeMongoID(_id)},_updateObj,true,function(e){
		if(!e) _cb(null);
		else _cb(e)
	})
}
//_event_id=the event we are formatting
//_scenes=mongodb array of scenes
//_cb= callback(err,_scenes[])
exports.formatScenes = function(_event_id,_scenes,_cb){
	formatScenes(_event_id,_scenes,_cb);
}
//_scene_id=the scene we are formatting
//_clips=mongodb array of clips
//_cb= callback(err,_clips[])
function formatScenes(_scene_id,_clips,_cb){

		//get all scenes in an event
		queryCollection('clips', {scene_id: makeMongoID(_scene_id)}, function(e, _clip_docs){
			if(!e){
				var clip_counter = 0;
				_clip_docs.forEach(function(clip_doc,i){
					_clips.forEach(function(clip_id,j){
						if(clip_doc._id.toString() === clip_id.toString()){
							_clips[j] = clip_doc;
							//TO DO:
							//handle zone data and get the right file information
							clip_counter++;
							if(clip_counter == _clips.length){
								_cb(null,_clips)
							}
						}	
					})
				})
			}else{//if(!e)
				_cb(e,null)
			}//if(!e)
		})

}


//get all events and assets formatted for init command
//async function which parses all events and assets and reorders them and sends them back to the socket
//_cb=callback(err,_events[])
exports.formatInit=function(_cb){
	collection.events.find().toArray(function(e,_events){
		if(!e){
			if(_events.length>0){
				var events_counter = 0;
				_events.forEach(function(event,i){
					//console.log(i+' :: '+JSON.stringify(event))
					if(event.scenes.length>0){
						formatScenes(event._id,event.scenes,function(_err,_scenes){
							_events[i].scenes=_scenes;
							events_counter++;
							if(events_counter==_events.length) _cb(null,_events)
						})
					}else{ //no assets
						   //console.log('EVENTS['+i+'] Contains No Assets');
						   events_counter++;
						   if(events_counter==_events.length){
					   		//edge case - our last event doesn't have any assets
					   		 _cb(null,_events); 
						   }
					}
					
				})
			}else{//if(_events>0)
				//console.error('No Events');
				_cb({'error':'No Events Loaded into Database'});
			}
		}else{ //if(!e)
			//Handle Error
			console.error(e);
			_cb(e)
		}
	});	
}
//get a mongo document by collection and slug string
//_type = collection type
//_slug = single slug
//_cb = callback(err,_document)
exports.getDocumentBySlug = function(_type,_slug,_cb){
	collection[_type].findOne({slug:_slug},function(e,_doc){
		if(!e)_cb(null,_doc);
		else _cb(e);
	})
}
//making global might not be necessary
exports.getDocumentByID= function(_type,__id,_cb){
	getDocumentByID(_type,__id,_cb);
}

//get a mongo document by collection and string id 
//_type = collection type
//_id = mongodb id as string
//_cb = callback(err,_document)
function getDocumentByID(_type,__id,_cb){
	collection[_type].findOne({_id:makeMongoID(__id)},function(e,_doc){
		//does error handling happen here?
		if(!e) _cb(null,_doc)
		else _cb(e);
	})
	
}
//making global
exports.makeMongoID=function(_id){
	return makeMongoID(_id);
}
//create a mongoID Function
function makeMongoID(__id){

	if(typeof __id == "object") return __id;
	if(typeof __id == "string" && __id.length == 24) return new BSON.ObjectID(__id.toString());
	else return '';
}

//format asset

function formatAsset(_asset,cb){
	console.log('formatting asset: '+JSON.stringify(_asset));
	if(_asset.file!=''){
		getDocumentByID('files',_asset.file,function(e,_file){
			if(_file.location!=''){
				getDocumentByID('locations',_file.location,function(_e,_loc){
					_file.location = _loc;
					_asset.file=_file;
					cb(_asset)
				})
			}else{
				_asset.file=_file;
				cb(_asset)
			}
		})
	}else{
		
		cb(_asset)
	}
	
}




