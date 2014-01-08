var MongoClient = require('mongodb').MongoClient,
	Server = require('mongodb').Server,
	mongo = new MongoClient(new Server('localhost', 27017)),
	BSON = require('mongodb').BSONPure,
	colors = require('colors');
	
var database,
	collection = {};


//make a class of Mongo connect
exports.MongoConnect = function(){
	
	mongo.open(function(err,mongo){
		//connected to database
		console.log('connected to db'.grey);
		database = mongo.db('30_PP');
		//setup collection connections

		collection.assets = database.collection('assets');
		collection.events = database.collection('events');
		collection.users  = database.collection('users');
		
	})

}

//_doc = mongo document to add
//_type = collection name
//_cb = callback() 

//returns mongo Document inserted
exports.add= function(_type,_doc,_cb){

	collection[_type].insert(_doc,function(e){
		
		if(!e) _cb(null,_doc);
		else _cb(e);
	})
}

//returns all the results within a collection
//_type = collection name
//_cb = callback()
//returns mongodb Collection as an Array
exports.getAll=function(_type,_cb){

	collection[_type].find().toArray(function(e,_data){
	
		if(!e) _cb(null,_data);
		else _cb(e);
	})
}

//update a document 
//_type = collection name
//_what = collection query
//_updateObj = the update operation which needs to take place
exports.update=function(_type,_what,_updateObj,_cb){
	
	collection[_type].update(_what,_updateObj,true,true,function(e){
		
	})
	
}
//update a document by providing a mongodb ID string
exports.updateById=function(_type,_id,_updateObj,_cb){
	
	//convert _id to MongoObject
	var o_id = new BSON.ObjectID(_id.toString());
	
	collection[_type].update({_id:o_id},_updateObj,true,function(e){
		if(!e) _cb(null);
		else _cb(e)
		
	})
	
	
}
