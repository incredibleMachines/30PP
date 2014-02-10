var utils = require('../modules/Utils');


//Routes for our event functionality

exports.index = function(_Database){
 	//get all events and list them in the db
 	//retrieve all events

	return function(req,res){
	 	//retrieve all events
	 	_Database.getAll('events',function(err, _events){
	 		//error handling needs to happen
	 		_Database.getAll('assets',function(err,_assets){
		 		res.render('events/index', { current: req.url, title: 'Events', page_slug: 'events-index', events: _events, assets: _assets } );

	 		})
	 	});
	}
}
//post for adding an event.
exports.add = function(_Database){
	//add a new event into the database
	return function(req,res){
		//where does the user get sent?
		//AJAX res serves jsonp
		
		console.log('Event Add Request')
		//console.log(req.body);
		var post = req.body;
		/*******************
		Sample Post Body: 
		
			{ 	title: '',
				anim_type: 'anim_0'
				
				}
		*
		*/
		post.slug = utils.makeSlug(post.title);
		if(!post.hasOwnProperty('duration')) post.duration = null;
		post.scenes =[];
		post.created_at = new Date();
		console.log(post)
		
		_Database.add('events', post, function(err, doc){
			
			if(!err) res.jsonp(doc)
			else res.jsonp(500,err);
		})
		//res.render('events/index', { title: 'Event Added', page_slug: 'event-index event-added',events:[] });
	}
}

exports.single = function(_Database){

	return function(req,res){
		var slug = req.params.slug;
		_Database.getDocumentBySlug('events',slug,function(e,doc){
				
			if(!e){
				//check if there are scenes if there aren't return the page
				if(doc.scenes.length>0){ //we have a few scenes so lets format them.
				
					_Database.formatScenes(doc._id, doc.scenes, function(err,_scenes){
						if(!err){
							 _Database.getAll('files',function(_e,_files){
								 if(!_e) res.render('events/single', {current: req.url, title: doc.title, page_slug: 'events-single', event: doc, files:_files, error: null});
								 else res.render('events/single', {current: req.url, title: 'Get Files Event Error', page_slug: 'events-single error', event: doc, files:[], error: _e})
							 })
							 
						}else{ 
							res.render('events/single', {current: req.url, title: 'Bind Scenes Event Error', page_slug: 'events-single error', event: [], files:[], error: err})
						}
					})		 
				}else{ //we have no scenes yet!
					
					_Database.getAll('files',function(_e,_files){
						if(!_e) res.render('events/single',{current:req.url, title:doc.title, page_slug:'events-single', event: doc, files: _files, error: null});
						else res.render('events/single',{current:req.url, title: 'Get Files Event Error', page_slug: 'events-single error', event:doc,files:[],error:_e})
					})
					
				}
			}else{ 
				res.render('events/single', {current: req.url, title: 'Find Event Error', page_slug: 'events-single error', event: [], files:[], error: e})
			}
		})
		
	}
}

exports.delete = function(_Database){

	return function(req,res){
		res.jsonp({message:'delete'});
	}
}

exports.update = function(_Database){

	return function(req,res){
		res.jsonp({message:'update'});
		//res.render('events/index', { title: 'add event' });
	}
}

exports.emitOne = function(_Database){

	return function(req,res){
		res.render('events/index', { current: req.url, title: 'add event', page_slug: 'events-emit' });
	}
}