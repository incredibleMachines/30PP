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

//_a = mongo document to add
//_type = collection name
//_cb = callback() 
exports.add= function(_doc,_type,_cb){

	collection[_type].insert(_doc,function(e){
		
		if(!e) _cb(null,_doc);
		else _cb(_a,e);
	})
}

//returns all the results within a collection
//_type = collection name
//_cb = callback()
exports.getAll=function(_type,_cb){

	collection[_type].find().toArray(function(e,_data){
	
		if(!e) _cb(null,_data);
		else _cb(e);
	})
}
